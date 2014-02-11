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
    read_packet();
  }
}


void router::read_packet(void)
{
  packet p = input.read();
  if (fifo->num_free() > 0)
  {

    /* ^  _____
     * |_|     |__
     * ---------------------> router.activated_in
     * ^   _____
     * |__|     |_continue
     * ---------------------> router.acknoledge
     */

    fifo[p.address].write(p);
    acknoledge.write(true);
    wait(activated_in.negedge_event());
    acknoledge.write(false);
  }
}


} // noc
