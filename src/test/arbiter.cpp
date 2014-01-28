#include <memory>

#include <systemc>

#include <noc/packet.h>
#include <noc/arbiter.h>
#include <noc/traffic_generator.h>


SC_MODULE(packet_wrapper)
{
  public:
  ::sc_core::sc_in<bool> clock;
  ::sc_core::sc_in<uint8_t> addr;
  ::sc_core::sc_in<int> data;
  ::sc_core::sc_fifo_out<noc::packet> packs;

  void process()
  {
    while(true)
    {
      wait();
      packs.write(noc::packet(addr.read(), data.read()));
    }
  }

  SC_CTOR(packet_wrapper)
  {
    SC_THREAD(process);
    sensitive << clock.pos();
  }
};

int sc_main(int argc, char *argv[])
{
  sc_core::sc_clock clock;
  sc_core::sc_signal<int> arbType;
  sc_core::sc_signal<uint8_t> choice;
  sc_core::sc_signal<noc::packet> out;
  sc_core::sc_fifo<noc::packet> fifos[4];

  sc_core::sc_signal<bool> stream_activated[4];
  sc_core::sc_signal<uint8_t> stream_addr[4];
  sc_core::sc_signal<int> stream_data[4];

  auto ab = std::make_shared<noc::arbiter>("arbiter", 4, 8);
  ab->clock(clock);
  ab->arbType(arbType);
  ab->out(out);
  ab->choice_out(choice);

  arbType.write(noc::arbiter_mode::FIXED);

  std::shared_ptr<noc::stream_generator> tg[4];
  std::shared_ptr<packet_wrapper> pw[4];

  for (unsigned i = 0; i < 4; ++i)
  {
    ab->f[i].bind(fifos[i]);

    // Set stream_generator array.
    {
      std::stringstream ss;
      ss << "stream_generator" << i;

      tg[i] = std::make_shared<noc::stream_generator>(ss.str().c_str());
      tg[i]->set_address_range(0, 4);
      tg[i]->set_period(3*(i+1));

      tg[i]->clock(clock);
      tg[i]->activated(stream_activated[i]);
      tg[i]->address(stream_addr[i]);
      tg[i]->data(stream_data[i]);
    }

    // Set packet_wrapper array.
    {
      std::stringstream ss;
      ss << "packet_wrapper" << i;

      pw[i] = std::make_shared<packet_wrapper>(ss.str().c_str());

      // Activate packet wrapper only when a packet is emitted!
      pw[i]->clock(tg[i]->activated);
      pw[i]->addr(stream_addr[i]);
      pw[i]->data(stream_data[i]);
      pw[i]->packs(fifos[i]);
    }
  }

  sc_core::vcd_trace_file* const file =
      (sc_core::vcd_trace_file*) sc_core::sc_create_vcd_trace_file("output");

  sc_core::sc_trace(file, choice, "arbiter_choice");
  sc_core::sc_trace(file, clock , "clock"         );

  for (unsigned i = 0; i < 4; ++i)
  {
    std::stringstream ss;
    ss << "stream_act" << i;
    sc_core::sc_trace(file, stream_activated[i], ss.str().c_str());
  }

  sc_core::sc_start(1000, sc_core::SC_NS);

  sc_core::sc_close_vcd_trace_file(file);

  return 0;
}
