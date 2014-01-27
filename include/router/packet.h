#ifndef _ROUTER_PACKET_H_
#define _ROUTER_PACKET_H_

#include <systemc>

/**
 * @brief Transmitted packet on the network.
 *
 * An empty packet has src_x == src_y == dest_x == dest_y == -1.
 */
struct packet
{
  uint8_t address;
  int data;

  packet(uint8_t add = 0, int data = 0)
    : address(add)
    , data(data)
  {}

}; // struct packet

std::ostream& operator<< (std::ostream& stream, packet const& p)
{
  stream << p.data << "@" << p.address;
  return stream;
}

#endif // _ROUTER_PACKET_H_
