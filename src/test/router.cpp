#include <router/router.h>

#include <cassert>
#include <sstream>


#include <router/packet.h>
#include <traffic/traffic_generator.h>


SC_MODULE(fifo_spy)
{
  sc_core::sc_fifo_in<packet> fifo[4];
  sc_core::sc_out<packet> out[4];
  sc_core::sc_in<bool> clock;

  SC_CTOR(fifo_spy)
  {
    SC_METHOD(spy);
    sensitive << clock.pos();
  }

  void spy(void)
  {
    for (int i = 0; i < 4; ++i)
    {
      if (fifo[i].num_available() > 0)
        out[i].write(fifo[i].read());
    }
  }
};

int sc_main(int argc, char** argv)
{
  // Declare clock.
  sc_core::sc_clock clock;

  stream_generator* tg1 = new stream_generator("stream_generator");
  {
    tg1->set_address_range(0, 4);
    tg1->set_period(10);
    tg1->clock(clock);
  }

  burst_generator* tg2 = new burst_generator("burst_generator");
  {
    tg2->set_address_range(0, 4);
    tg2->set_burst(100, 2, 10);
    tg2->clock(clock);
  }


  sc_core::sc_signal<bool> stream_activated, burst_activated;
  {
    tg1->activated(stream_activated);
    tg2->activated(burst_activated);
  }

  sc_core::sc_signal<int> stream_data, burst_data;
  {
    tg1->data(stream_data);
    tg2->data(burst_data);
  }

  sc_core::sc_signal<uint8_t> stream_addr, burst_addr;
  {
    tg1->address(stream_addr);
    tg2->address(burst_addr);
  }

  router * routr1 = new router("router1", 4, 1);
  sc_core::sc_fifo<packet> fifos1[4];
  fifo_spy spy1("spy1");
  {
    spy1.clock(clock);
    for (int i = 0; i < 4; ++i)
    {
      routr1->fifo[i](fifos1[i]);
      spy1.fifo[i](fifos1[i]);
    }

    routr1->clock(clock);
    for(int i=0;i<4;i++)
    {
      routr1->activated_in[i](stream_activated);
      routr1->address_in[i](stream_addr);
      routr1->data_in[i](stream_data);
    }
  }


  router * routr2 = new router("router2", 4, 1);
  sc_core::sc_fifo<packet> fifos2[4];
  fifo_spy spy2("spy2");
  {
    spy2.clock(clock);
    for (int i = 0; i < 4; ++i)
    {
      routr2->fifo[i](fifos2[i]);
      spy2.fifo[i](fifos2[i]);
    }

    routr2->clock(clock);
    for(int i = 0; i < 4; ++i)
    {
      routr2->activated_in[i](burst_activated);
      routr2->address_in[i](burst_addr);
      routr2->data_in[i](burst_data);
    }
  }

  sc_core::vcd_trace_file* const file =
      (sc_core::vcd_trace_file*) sc_core::sc_create_vcd_trace_file("output");

  sc_core::sc_trace(file, tg1->activated, "stream_active" );
  sc_core::sc_trace(file, tg1->address  , "stream_address");
  sc_core::sc_trace(file, tg1->data     , "stream_data"   );

  sc_core::sc_trace(file, tg2->activated, "burst_active" );
  sc_core::sc_trace(file, tg2->address  , "burst_address");
  sc_core::sc_trace(file, tg2->data     , "burst_data"   );

  for (int i = 0; i < 4; ++i)
  {
    std::ostringstream ss;
    ss << "fifo1." << i;
    sc_core::sc_trace(file, spy1.out[i], ss.str());
  }

  for (int i = 0; i < 4; ++i)
  {
    std::ostringstream ss;
    ss << "fifo2." << i;
    sc_core::sc_trace(file, spy2.out[i], ss.str());
  }

  sc_core::sc_start(1000, sc_core::SC_NS);

  sc_core::sc_close_vcd_trace_file(file);

  return 0;
}

