#include <sstream>
#include <memory>

#include <systemc>

#include <noc/traffic_generator.h>


int sc_main(int, char**)
{
  noc::stream_generator tg1("stream_generator");
  tg1.set_address_properties(10, 100);
  tg1.set_period(10);

  noc::burst_generator tg2("burst_generator");
  tg2.set_address_properties(2, 10);
  tg2.set_burst(100, 5, 20);

  sc_core::sc_clock clock(
        "clock",
        sc_core::sc_time(CLOCK_PERIOD_NS, sc_core::SC_NS));

  tg1.clock.bind(clock);
  tg2.clock.bind(clock);
  tg1.acknoledge.bind(clock);
  tg2.acknoledge.bind(clock);

  sc_core::sc_signal<bool> stream_activated, burst_activated;
  tg1.activated.bind(stream_activated);
  tg2.activated.bind(burst_activated);

  sc_core::sc_signal<noc::packet> stream_packet, burst_packet;
  tg1.output.bind(stream_packet);
  tg2.output.bind(burst_packet);

  sc_core::vcd_trace_file* const file =
      (sc_core::vcd_trace_file*) sc_core::sc_create_vcd_trace_file("traffic");

  sc_core::sc_trace(file, clock        , "clock"         );

  sc_core::sc_trace(file, tg1.activated, "stream_active" );
  sc_core::sc_trace(file, tg1.output   , "stream_packet" );

  sc_core::sc_trace(file, tg2.activated, "burst_active"  );
  sc_core::sc_trace(file, tg2.output   , "burst_output"  );

  sc_core::sc_start(1000, sc_core::SC_NS);

  sc_core::sc_close_vcd_trace_file(file);

  return 0;
}

