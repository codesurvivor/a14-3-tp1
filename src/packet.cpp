#include <noc/packet.h>


namespace sc_core
{


std::ostream& operator<< (
    std::ostream& stream,
    ::noc::packet const& p)
{
  stream << p.data << "@" << p.address;
  return stream;
}


std::ostream& operator << (
    std::ostream& stream,
    ::noc::packet&& p)
{ return operator<< (stream, p); }


} // sc_core
