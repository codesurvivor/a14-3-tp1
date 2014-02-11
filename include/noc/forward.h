#ifndef _NOC_FORWARD_H_
#define _NOC_FORWARD_H_

namespace noc
{

enum arbiter_mode : unsigned char
{
  RAND       = 0,
  LRU        = 1,
  FIFO       = 2,
  FIXED      = 3,
  ROUNDROBIN = 4
};

struct packet;
struct abstract_traffic_generator;

class arbiter;
class router;
class noc;

} // noc

#endif // _NOC_FORWARD_H_
