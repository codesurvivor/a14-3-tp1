#include <noc/router.h>

#include <cassert>
#include <sstream>
#include <memory>

#include <noc/packet.h>
#include <noc/traffic_generator.h>


#define OUTPUT_NB 8
#define INPUT_NB 4

class fifo_spy
    : public ::sc_core::sc_module
{
public:

  sc_core::sc_fifo_in<noc::packet> fifo[OUTPUT_NB];
  sc_core::sc_out<noc::packet> out[OUTPUT_NB];
  sc_core::sc_in<bool> clock;

  SC_CTOR(fifo_spy)
  {
    SC_THREAD(spy);
    sensitive << clock.pos();
  }

  void spy(void)
  {
    while (true)
    {
      wait();
      for (int i = 0; i < OUTPUT_NB; ++i)
      {
        if (fifo[i].num_available() > 0)
        {
          noc::packet p = fifo[i].read();
          out[i].write(p);

//          std::cout
//              << "available(" << i << "): "
//              << (fifo[i].num_available() + 1) << " -> "
//              << "@[" << std::hex << unsigned(p.address) << "] = "
//              << std::dec << p.data << std::endl;
        }
      }
    }
  }
};

int sc_main(int argc, char** argv)
{
  // Declare clock.
  sc_core::sc_clock clock(
        "clock",
        sc_core::sc_time(CLOCK_PERIOD_NS, sc_core::SC_NS));

  noc::stream_generator tg1("stream_generator");
  {
    tg1.set_address_properties(0, OUTPUT_NB);
    tg1.set_period(5);
    tg1.set_offset(0);
    tg1.clock(clock);
  }

  noc::burst_generator tg2("burst_generator");
  {
    tg2.set_address_properties(0, OUTPUT_NB);
    tg2.set_burst(100, 2, 10);
    tg2.set_offset(0);
    tg2.clock(clock);
  }

  sc_core::sc_signal<bool>
      stream_activated, burst_activated,
      stream_ack, burst_ack;
  {
    tg1.activated(stream_activated);
    tg2.activated(burst_activated);

    tg1.acknoledge(stream_ack);
    tg2.acknoledge(burst_ack);
  }

  sc_core::sc_signal<noc::packet> stream_packet, burst_packet;
  {
    tg1.output(stream_packet);
    tg2.output(burst_packet);
  }

  noc::router router1("router1", OUTPUT_NB);

  sc_core::sc_fifo<noc::packet> fifos1[OUTPUT_NB];
  fifo_spy spy1("spy1");
  {
    router1.clock(clock);
    spy1.clock(clock);

    for (int i = 0; i < OUTPUT_NB; ++i)
    {
      router1.fifos[i](fifos1[i]);
      spy1.fifo[i](fifos1[i]);
    }

    router1.activated_in(stream_activated);
    router1.acknoledge(stream_ack);
    router1.input(stream_packet);
  }

  noc::router router2("router2", OUTPUT_NB);

  sc_core::sc_fifo<noc::packet> fifos2[OUTPUT_NB];
  fifo_spy spy2("spy2");
  {
    router2.clock(clock);
    spy2.clock(clock);

    for (int i = 0; i < OUTPUT_NB; ++i)
    {
      router2.fifos[i](fifos2[i]);
      spy2.fifo[i](fifos2[i]);
    }

    router2.activated_in(burst_activated);
    router2.acknoledge(burst_ack);
    router2.input(burst_packet);
  }

  sc_core::sc_signal<noc::packet> spy1_out[OUTPUT_NB];
  sc_core::sc_signal<noc::packet> spy2_out[OUTPUT_NB];
  {
    for (int i = 0; i < OUTPUT_NB; ++i)
    {
      spy1.out[i].bind(spy1_out[i]);
      spy2.out[i].bind(spy2_out[i]);
    }
  }

  sc_core::vcd_trace_file* const file =
      (sc_core::vcd_trace_file*) sc_core::sc_create_vcd_trace_file("router");

  sc_core::sc_trace(file, clock        , "clock"        );

  sc_core::sc_trace(file, tg1.activated, "stream_active");
  sc_core::sc_trace(file, tg1.output   , "stream_output");

  sc_core::sc_trace(file, tg2.activated, "burst_active" );
  sc_core::sc_trace(file, tg2.output   , "burst_output" );

  for (int i = 0; i < OUTPUT_NB; ++i)
  {
    std::ostringstream ss;
    ss << "fifo_1_" << i;
    sc_core::sc_trace(file, spy1_out[i], ss.str());
  }

  for (int i = 0; i < OUTPUT_NB; ++i)
  {
    std::ostringstream ss;
    ss << "fifo_2_" << i;
    sc_core::sc_trace(file, spy2_out[i], ss.str());
  }

  sc_core::sc_start(1000, sc_core::SC_NS);

  sc_core::sc_close_vcd_trace_file(file);

  return 0;
}

