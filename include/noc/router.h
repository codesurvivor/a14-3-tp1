#ifndef _ROUTER_ROUTER_H_
#define _ROUTER_ROUTER_H_

#include <noc/router.h>

#include <list>
#include <sstream>
#include <memory>

#include <systemc>


namespace noc
{

class packet;
class abstract_routing_policy;

class router
    : ::sc_core::sc_module
{
public:
  typedef router SC_CURRENT_USER_MODULE;

  /// Port for the global clock.
  sc_core::sc_in<bool> clock;

  /// Input activation port.
  sc_core::sc_in<bool> activated_in;

  /// Address input.
  sc_core::sc_in<packet> input;

  /// Internal FIFO.
  sc_core::sc_fifo_out<packet>* fifos;

  /// Acknoledgment.
  sc_core::sc_out<bool> acknoledge;

  /// Specify the functionality of router per clock cycle.
  void main(void);

  router(::sc_core::sc_module_name name, unsigned output);

  ~router(void);

protected:

  /// Read a packet from the link.
  void read_packet(void);

private:

  /// Number of ouput ports.
  const unsigned _router_output_ports;
};

} // noc

#endif // _ROUTER_ROUTER_H_
