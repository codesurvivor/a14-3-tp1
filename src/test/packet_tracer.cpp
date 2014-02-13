#include "packet_tracer.h"


namespace test
{


packet_tracer::packet_tracer(
    const sc_core::sc_module_name &name,
    unsigned input_nb,
    const std::string &output_file)
  : sc_core::sc_module(name)
  , clock("clock")
  , _input_nb(input_nb)
  , _watcher_last_id(0)
  , _current_clock_time(0)
  , _output_file(output_file)
{
  // Initialize inputs.
  {
    inputs = reinterpret_cast<sc_core::sc_in<noc::packet>*>(
          std::malloc(sizeof(sc_core::sc_in<noc::packet>) * _input_nb));

    for (unsigned i = 0; i < _input_nb; ++i)
    {
      std::stringstream ss;
      ss << "input" << i;
      new(inputs + i) sc_core::sc_in<noc::packet>(ss.str().c_str());
    }
  }

  // Declare threads.
  {
    // Declare the clock counter thread.
    SC_THREAD(clock_counter);
    sensitive << clock.pos();

    // Declare the _input_nb input_watcher threads.
    for (unsigned i = 0; i < _input_nb; ++i)
    {
      std::stringstream ss;
      ss << "input_watcher" << i;
      ::sc_core::sc_process_handle handle =
          ::sc_core::sc_get_curr_simcontext()->create_thread_process(
            ss.str().c_str(), false,
            SC_MAKE_FUNC_PTR(SC_CURRENT_USER_MODULE,
                             input_watcher), this, 0 );
      sensitive << handle;
      sensitive_pos << handle;
      sensitive_neg << handle;
      sensitive << clock.pos();
    }
  }
}


packet_tracer::~packet_tracer(void)
{
  // Destroy inputs.
  {
    for (unsigned i = 0; i < _input_nb; ++i)
      inputs[i].~sc_in<noc::packet>();

    std::free(inputs);
  }

  // Compute, save and display statistics.
  {
    std::stringstream ss;
    std::ofstream simple_stat(_output_file + "_simple_stat.data");

    ss << "---------------------------------------------------------\n";

    // Maximal packet life time.
    {
      assert(
            _packet_life_time_counts.rbegin() !=
          _packet_life_time_counts.rend());
      ss << "| Max packet life time      | "
         << _packet_life_time_counts.rbegin()->first
         << " clock steps."
         << std::endl;

      simple_stat << _packet_life_time_counts.rbegin()->first << ',';
    }

    ss << "---------------------------------------------------------\n";

    // Mean packet life time.
    {
      packet_time_counter::const_iterator it =
          _packet_life_time_counts.begin(),
          itEnd = _packet_life_time_counts.end();

      unsigned long sum = 0;
      unsigned count = 0;
      for (; it != itEnd; ++it)
      {
        sum += it->first * it->second;
        count += it->second;
      }

      ss << "| Mean packet life time     | "
         << (double(sum)/count)
         << " clock steps."
         << std::endl;

      simple_stat << (double(sum)/count) << ',';
    }

    ss << "---------------------------------------------------------\n";

    // Maximal packet trasit time.
    {
      assert(
            _packet_transit_time_counts.rbegin() !=
          _packet_transit_time_counts.rend());
      ss << "| Max packet transit time   | "
         << _packet_transit_time_counts.rbegin()->first
         << " clock steps."
         << std::endl;

      simple_stat << _packet_transit_time_counts.rbegin()->first << ',';
    }

    ss << "---------------------------------------------------------\n";

    // Mean packet transit time.
    {
      packet_time_counter::const_iterator it =
          _packet_transit_time_counts.begin(),
          itEnd = _packet_transit_time_counts.end();

      unsigned long sum = 0;
      unsigned count = 0;
      for (; it != itEnd; ++it)
      {
        sum += it->first * it->second;
        count += it->second;
      }

      ss << "| Mean packet transit time  | "
         << (double(sum)/count)
         << " clock steps."
         << std::endl;

      simple_stat << (double(sum)/count);
    }

    ss << "---------------------------------------------------------\n";
    simple_stat.flush();

    // Put statistics in file.
    {
      std::ofstream stat_file(_output_file + "_stat.data");
      stat_file << ss.str();
      stat_file.flush();
    }

    // Display it.
    {
      std::cout << ss.str();
      std::cout.flush();
    }
  }

  // Save per packet informations.
  {
    std::ofstream packet_info_file(_output_file + "_packet_trace.data");

    for (packet_info const& info : _full_trace)
    {
      packet_info_file
          << std::get<0>(info) << ','
          << std::get<1>(info) << ','
          << std::get<2>(info) << std::endl;
    }

    packet_info_file.flush();
  }

  // Destroy remaining packets.
  {
    for (packet_extra* p : _packet_extra_set)
      delete p;
  }
}


void packet_tracer::prepare_packet(noc::packet &p)
{
  std::lock_guard<std::mutex> lock(_watcher_mutex);

  packet_extra* extra = new packet_extra();
  {
    extra->creation_date = _current_clock_time;

    _packet_extra_set.insert(extra);
    p.extra = extra;
  }
}


void packet_tracer::prepare_packet_insertion_in_fifo(noc::packet &p)
{
  assert(p.extra);
  packet_extra* extra = reinterpret_cast<packet_extra*>(p.extra);

  std::lock_guard<std::mutex> lock(_watcher_mutex);
  extra->fifo_insertion_date = _current_clock_time;
}


void packet_tracer::clock_counter(void)
{
  while (true)
  {
    wait();
    {
      std::lock_guard<std::mutex> lock(_watcher_mutex);
      ++_current_clock_time;
    }
  }
}


inline void
packet_tracer::declare_packet_life_time(unsigned long time)
{
  packet_time_counter::iterator it =
      _packet_life_time_counts.find(time);

  if (it != _packet_life_time_counts.end())
    ++it->second;
  else
    _packet_life_time_counts[time] = 1;
}


inline void
packet_tracer::declare_packet_transit_time(unsigned long time)
{
  packet_time_counter::iterator it =
      _packet_transit_time_counts.find(time);

  if (it != _packet_transit_time_counts.end())
    ++it->second;
  else
    _packet_transit_time_counts[time] = 1;
}


void packet_tracer::input_watcher(void)
{
  unsigned id = get_watcher_id();

  while(true)
  {
    wait(inputs[id].value_changed_event());
    {
      noc::packet const& p = inputs[id].read();
      assert(p.extra);

      packet_extra* extra =
          reinterpret_cast<packet_extra*>(p.extra);
      {
        // Protect _current_clock_time and _packet_extra_set.
        std::lock_guard<std::mutex> lock(_watcher_mutex);

        declare_packet_life_time(
              _current_clock_time - extra->creation_date);
        declare_packet_transit_time(
              _current_clock_time - extra->fifo_insertion_date);

        _full_trace.push_back(
              packet_info(
                extra->creation_date,
                extra->fifo_insertion_date,
                _current_clock_time));

        assert(_packet_extra_set.count(extra));
        _packet_extra_set.erase(extra);
      }

      delete extra;
    }
  }
}


inline unsigned
packet_tracer::get_watcher_id(void)
{
  std::lock_guard<std::mutex> lock(_watcher_mutex);
  return _watcher_last_id++;
}


} // test
