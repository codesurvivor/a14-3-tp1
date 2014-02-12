#include <noc/traffic_generator.h>


namespace noc
{


abstract_traffic_generator::abstract_traffic_generator(
    sc_core::sc_module_name name)
  : ::sc_core::sc_module(name)
  , clock("clock")
  , acknoledge("acknoledge")
  , activated("activated")
  , output("output")
  , _address_min(0)
  , _address_max(1)
  , _clock_count(0)
{
  activated.initialize(false);
  output.initialize(packet());

  SC_THREAD(clock_counter);
}


void abstract_traffic_generator::emit_random_package(void)
{
  packet p;
  p.address = get_address();
  p.data = rand();

  // ^  _____
  // |_|     |___continue...
  // -----------------------> traffic_generator.activated
  // ^      ___
  // |_____|   |_
  // -----------------------> traffic_generator.acknoledge

  output.write(p);

  // Signal that the packet is ready.
  wait(clock.negedge_event());
  activated.write(true);

  // With for acknolegment from the receiver.
  wait(acknoledge.posedge_event());
  wait(clock.posedge_event());
  activated.write(false);

  // Continue.
  wait(acknoledge.negedge_event());
}


void abstract_traffic_generator::clock_counter(void)
{
  while (true)
  {
    wait(clock.posedge_event());
    ++_clock_count;
//    std::cout << "count: " << _clock_count << std::endl;
  }
}


uint8_t abstract_traffic_generator::get_address(void)
{
  --_current_address_life_time;
  uint8_t res = _current_address;
  if (!_current_address_life_time) choose_new_random_address();
  return res;
}


void abstract_traffic_generator::choose_new_random_address(void)
{
  _current_address =
      (rand() % (_address_max - _address_min)) + _address_min;
  _current_address_life_time = (rand() % _address_max_life_time);
}


stream_generator::stream_generator(sc_core::sc_module_name name)
  : abstract_traffic_generator(name)
  , _period(1)
{
  SC_THREAD(generate);
  dont_initialize();

  sensitive << clock.pos();
}


void stream_generator::generate(void)
{
  while(true)
  {
    wait();

    if (get_clock_count() >= _period)
    {
      reset_clock_count();
      emit_random_package();
    }
  }
}


burst_generator::burst_generator(sc_core::sc_module_name name)
  : abstract_traffic_generator(name)
  , _current_packet_count(0)
  , _long_period(1)
  , _short_period(1)
  , _burst_length(1)
{
  SC_THREAD(generate);
  dont_initialize();

  sensitive << clock.pos();
}


void burst_generator::generate(void)
{
  while(true)
  {
    wait();

    if (get_clock_count() < _burst_length &&
        (get_clock_count() / _short_period) >= _current_packet_count)
    {
      ++_current_packet_count;
      emit_random_package();
    }

    if (get_clock_count() >= _long_period)
    {
      reset_clock_count();
      _current_packet_count = 0;
    }
  }
}


} // noc
