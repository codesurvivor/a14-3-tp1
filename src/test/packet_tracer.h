#ifndef _TEST_PACKET_TRACER_H_
#define _TEST_PACKET_TRACER_H_

#include <noc/forward.h>

#include <map>
#include <set>
#include <thread>
#include <mutex>

#include <systemc>

#include <noc/packet.h>


namespace test
{

class packet_tracer
    : public sc_core::sc_module
{
public:

  typedef packet_tracer SC_CURRENT_USER_MODULE;

  sc_core::sc_in<bool> clock;

  sc_core::sc_in<noc::packet>* inputs;

  packet_tracer(
      sc_core::sc_module_name const& name,
      unsigned input_nb);

  ~packet_tracer(void);

  void prepare_packet(noc::packet& p);
  void prepare_packet_insertion_in_fifo(noc::packet& p);

private:

  // Inputs properties.

  unsigned _input_nb;

  // Threads.

  void clock_counter(void);

  void input_watcher(void);

  unsigned get_watcher_id(void);
  std::mutex _watcher_mutex;
  unsigned _watcher_last_id;

  // Packet watching.

  void declare_packet_life_time(unsigned long time);
  void declare_packet_transit_time(unsigned long time);

  unsigned long _current_clock_time;

  typedef std::map<unsigned, unsigned> packet_time_counter;
  packet_time_counter _packet_life_time_counts;
  packet_time_counter _packet_transit_time_counts;

  struct packet_extra
  {
    packet_extra(void)
      : creation_date(0)
      , fifo_insertion_date(0) {}

    unsigned long creation_date;
    unsigned long fifo_insertion_date;
  };

  typedef std::set<packet_extra*> packet_extra_set;
  packet_extra_set _packet_extra_set;
};

} // test

#endif // _TEST_PACKET_TRACER_H_
