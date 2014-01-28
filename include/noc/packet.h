#ifndef _ROUTER_PACKET_H_
#define _ROUTER_PACKET_H_

#include <systemc>
#include <sstream>


namespace noc
{

/**
 * @brief Transmitted packet on the network.
 *
 * An empty packet has src_x == src_y == dest_x == dest_y == -1.
 */
struct packet
{
  uint8_t address;
  int data;

  explicit inline packet(uint8_t add = 0, int data = 0)
    : address(add), data(data) {}

}; // struct packet

} // noc

namespace sc_core
{

inline bool operator==(const ::noc::packet& p1, const ::noc::packet& p2)
{ return (p1.address == p2.address && p1.data == p2.data); }

std::ostream& operator<< (::std::ostream& stream, ::noc::packet const& p);

std::ostream& operator<< (::std::ostream& stream, ::noc::packet&& p);

inline void sc_trace(
    sc_trace_file* file,
    ::noc::packet const& pack,
    std::string const& name)
{
  // Write address.
  {
    std::ostringstream ss;
    ss << name << ".address";
    sc_trace(file, pack.address, ss.str(), 8);
  }

  // Write data.
  {
    std::ostringstream ss;
    ss << name << ".data";
    sc_trace(file, pack.data, ss.str(), 32);
  }
}

} // sc_core

#endif // _ROUTER_PACKET_H_
