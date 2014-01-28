#include <router/packet.h>


std::ostream& operator<< (std::ostream& stream, packet const& p)
{
  stream << p.data << "@" << p.address;
  return stream;
}
