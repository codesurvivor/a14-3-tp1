#include <noc/router.h>

#include <cassert>

#include <noc/packet.h>


namespace noc
{


void router::main(void)
{
  acknoledge.write(false);

  while (true)
  {
    wait(activated_in.posedge_event());
    wait(clock.posedge_event());
    read_packet();
  }
}


router::router(sc_core::sc_module_name name, unsigned output)
  : ::sc_core::sc_module(name)
  , clock("clock")
  , activated_in("input_activated")
  , input("input")
  , acknoledge("acknoledge")
  , _router_output_ports(output)
  , fifos(nullptr)
{
  acknoledge.initialize(false);

  // Create the fifos.
  {
    fifos = reinterpret_cast<sc_core::sc_fifo_out<packet>* >(
          std::malloc(sizeof(sc_core::sc_fifo_out<packet>)*_router_output_ports));

    for (unsigned i = 0; i < _router_output_ports; ++i)
    {
      std::stringstream ss;
      ss << "fifo" << i;
      new(fifos+i) sc_core::sc_fifo_out<packet>(ss.str().c_str());
    }
  }

  // Declare threads.
  {
    SC_THREAD(main);
  }
}


router::~router(void)
{
  // Destroy fifos.
  {
    for (unsigned i = 0; i < _router_output_ports; ++i)
      fifos[i].~sc_fifo_out<packet>();
    std::free(fifos);
  }
}


void router::read_packet(void)
{
  packet p = input.read();

  // Wait for a free space in the corresponding fifo.
  while (fifos[p.address].num_free() == 0)
    wait(clock.posedge_event());

  // Put the packet in the right fifo.
  {
    fifos[p.address].write(p);

    // Acknoledge with shake hand protocol.
    {
      //  ------------
      //  |fifo ready|
      //  ------------
      //       |
      // ^     v__
      // |_____|  |_continue...
      // ---------------------> router.acknoledge
      // ^  _____
      // |_|     |__
      // ---------------------> router.activated_in

      acknoledge.write(true);
      wait(activated_in.negedge_event());
      wait(clock.negedge_event());
      acknoledge.write(false);
    }
  }
}


} // noc
