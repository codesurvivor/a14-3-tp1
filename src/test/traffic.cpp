#include <sstream>

#include <systemc>

#include <traffic/traffic_generator.h>


int sc_main(int argc, char** argv)
{
  stream_generator* tg1 = new stream_generator("stream_generator");
  tg1->set_address_range(10, 100);
  tg1->set_period(10);
  burst_generator* tg2 = new burst_generator("burst_generator");
  tg2->set_address_range(2, 10);
  tg2->set_burst(100, 2, 10);

  sc_core::sc_clock clock;
  tg1->clock(clock);
  tg2->clock(clock);

  sc_core::sc_signal<bool> stream_activated, burst_activated;
  tg1->activated(stream_activated);
  tg2->activated(burst_activated);

  sc_core::sc_signal<int> stream_data, burst_data;
  tg1->data(stream_data);
  tg2->data(burst_data);

  sc_core::sc_signal<uint8_t> stream_addr, burst_addr;
  tg1->address(stream_addr);
  tg2->address(burst_addr);

  sc_core::vcd_trace_file* const file =
      (sc_core::vcd_trace_file*) sc_core::sc_create_vcd_trace_file("output");

  sc_core::sc_trace(file, tg1->activated, "stream_active" );
  sc_core::sc_trace(file, tg1->address  , "stream_address");
  sc_core::sc_trace(file, tg1->data     , "stream_data"   );

  sc_core::sc_trace(file, tg2->activated, "burst_active" );
  sc_core::sc_trace(file, tg2->address  , "burst_address");
  sc_core::sc_trace(file, tg2->data     , "burst_data"   );

  sc_core::sc_start(1000, sc_core::SC_NS);

  sc_core::sc_close_vcd_trace_file(file);

  return 0;
}

