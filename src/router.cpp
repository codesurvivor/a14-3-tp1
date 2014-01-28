#include <noc/router.h>

#include <cassert>

#include <noc/packet.h>


namespace noc
{


void router::main(void)
{
  // To identify PE.
  assert(_x != -1);
  assert(_y != -1);

  for (int iport = 0; iport < _router_input_ports; ++iport)
    if (activated_in[iport].read())
      read_packet(iport);
}


void router::set_xy(int x, int y)
{
  // Set once only.
  assert(_x == -1);
  assert(_y == -1);

  // Must use a legal location.
  assert(x != -1);
  assert(y != -1);

  _x = x;
  _y = y;
}


void router::read_packet(int iport)
{
  assert(iport < _router_input_ports);

  packet p;
  p.data      = data_in[iport].read();
  p.address   = address_in[iport].read();

  fifo[p.address].write(p);
}


} // noc
