#include <router/router.h>

#include <cassert>
#include <sstream>
#include <memory>


#include <router/packet.h>
#include <traffic/traffic_generator.h>

#define OUTPUT_NB 8
#define INPUT_NB 4

SC_MODULE(fifo_spy)
{
  sc_core::sc_fifo_in<packet> fifo[OUTPUT_NB];
  sc_core::sc_out<packet> out[OUTPUT_NB];
  sc_core::sc_in<bool> clock;

  SC_CTOR(fifo_spy)
  {
    SC_METHOD(spy);
    sensitive << clock.pos();
  }

  void spy(void)
  {
    for (int i = 0; i < OUTPUT_NB; ++i)
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

  auto tg1 = std::make_shared<stream_generator>("stream_generator");
  {
    tg1->set_address_range(0, OUTPUT_NB);
    tg1->set_period(10);
    tg1->set_offset(0);
    tg1->clock(clock);
  }

  auto tg2 = std::make_shared<burst_generator>("burst_generator");
  {
    tg2->set_address_range(0, OUTPUT_NB);
    tg2->set_burst(100, 2, 10);
    tg2->set_offset(0);
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

  auto routr1 = std::make_shared<router>("router1", INPUT_NB, OUTPUT_NB);
  routr1->set_xy(1,0);
  sc_core::sc_fifo<packet> fifos1[OUTPUT_NB];
  fifo_spy spy1("spy1");
  {
    routr1->clock(clock);
    spy1.clock(clock);

    for (int i = 0; i < OUTPUT_NB; ++i)
    {
      routr1->fifo[i](fifos1[i]);
      spy1.fifo[i](fifos1[i]);
    }

    for(int i = 0; i < INPUT_NB; i++)
    {
      routr1->activated_in[i](stream_activated);
      routr1->address_in[i](stream_addr);
      routr1->data_in[i](stream_data);
    }
  }


  auto routr2 = std::make_shared<router>("router2", INPUT_NB, OUTPUT_NB);
  routr2->set_xy(1,1);
  sc_core::sc_fifo<packet> fifos2[OUTPUT_NB];
  fifo_spy spy2("spy2");
  {
    routr2->clock(clock);
    spy2.clock(clock);

    for (int i = 0; i < OUTPUT_NB; ++i)
    {
      routr2->fifo[i](fifos2[i]);
      spy2.fifo[i](fifos2[i]);
    }

    for(int i = 0; i < INPUT_NB; ++i)
    {
      routr2->activated_in[i](burst_activated);
      routr2->address_in[i](burst_addr);
      routr2->data_in[i](burst_data);
    }
  }

  sc_core::sc_signal<packet> spy1_out[OUTPUT_NB];
  sc_core::sc_signal<packet> spy2_out[OUTPUT_NB];
  {
    for (int i = 0; i < OUTPUT_NB; ++i)
    {
      spy1.out[i](spy1_out[i]);
      spy2.out[i](spy2_out[i]);
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

  for (int i = 0; i < OUTPUT_NB; ++i)
  {
    std::ostringstream ss;
    ss << "fifo1." << i;
    sc_core::sc_trace(file, spy1_out[i], ss.str());
  }

  for (int i = 0; i < OUTPUT_NB; ++i)
  {
    std::ostringstream ss;
    ss << "fifo2." << i;
    sc_core::sc_trace(file, spy2_out[i], ss.str());
  }

  sc_core::sc_start(1000, sc_core::SC_NS);

  sc_core::sc_close_vcd_trace_file(file);

  return 0;
}

