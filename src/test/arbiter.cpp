#include <memory>

#include <systemc>

#include <noc/packet.h>
#include <noc/arbiter.h>
#include <noc/traffic_generator.h>


class packet_wrapper
    : public ::sc_core::sc_module
{
  public:
  ::sc_core::sc_in<bool> clock;
  ::sc_core::sc_in<noc::packet> packet;
  ::sc_core::sc_fifo_out<noc::packet> packs;

  void process(void)
  {
    while(true)
    {
      wait();
      noc::packet myPacket(packet.read());

      if (packs.num_free() > 0)
        packs.write(myPacket);
    }
  }

  SC_CTOR(packet_wrapper)
  {
    SC_THREAD(process);
    sensitive << clock.pos();
  }
};


int sc_main(int argc, char **argv)
{
  sc_core::sc_clock clock(
        "clock",
        sc_core::sc_time(CLOCK_PERIOD_NS, sc_core::SC_NS));

  sc_core::sc_signal<noc::arbiter_mode> arb_type;
  sc_core::sc_signal<uint8_t> choice;
  sc_core::sc_signal<noc::packet> output;
  sc_core::sc_fifo<noc::packet> fifos[4];

  sc_core::sc_signal<bool> stream_activated[4];
  sc_core::sc_signal<noc::packet> stream_packet[4];

  noc::arbiter ab("arbiter", 4, 16);
  ab.clock(clock);
  ab.arb_type(arb_type);
  ab.output(output);
  ab.choice_out(choice);

  arb_type.write(noc::arbiter_mode::RAND);

  std::shared_ptr<noc::stream_generator> tg[4];
  std::shared_ptr<packet_wrapper> pw[4];

  for (unsigned i = 0; i < 4; ++i)
  {
    ab.fifos[i].bind(fifos[i]);

    // Set stream_generator array.
    {
      std::stringstream ss;
      ss << "stream_generator" << i;

      tg[i] = std::make_shared<noc::stream_generator>(ss.str().c_str());
      tg[i]->set_address_properties(0, 4);
      tg[i]->set_period(3*(i+1));

      tg[i]->clock.bind(clock);
      tg[i]->activated.bind(stream_activated[i]);
      tg[i]->acknoledge.bind(clock);
      tg[i]->output.bind(stream_packet[i]);
    }

    // Set packet_wrapper array.
    {
      std::stringstream ss;
      ss << "packet_wrapper" << i;

      pw[i] = std::make_shared<packet_wrapper>(ss.str().c_str());

      // Activate packet wrapper only when a packet is emitted!
      pw[i]->clock(tg[i]->activated);
      pw[i]->packet(stream_packet[i]);
      pw[i]->packs(fifos[i]);
    }
  }

  sc_core::vcd_trace_file* const file =
      (sc_core::vcd_trace_file*) sc_core::sc_create_vcd_trace_file("arbiter");

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
