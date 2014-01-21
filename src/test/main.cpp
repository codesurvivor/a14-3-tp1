#include <sstream>

#include <systemc.h>

#include <traffic/traffic_generator.h>
//#include <arbiter/arbiter.h>

int sc_main(int argc, char** argv)
{
  abstract_traffic_generator* tg1 = new stream_generator("stream_generator");
  abstract_traffic_generator* tg2 = new burst_generator("burst_generator");
  return 0;
}

