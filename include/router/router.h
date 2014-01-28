#ifndef _ROUTER_ROUTER_H_
#define _ROUTER_ROUTER_H_

#include <list>
#include <sstream>
#include <memory>

#include <systemc>

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
  sc_core::sc_in<bool>* activated_in;

  /// Address input.
  sc_core::sc_in<uint8_t>* address_in;

  /// Data input.
  sc_core::sc_in<int>* data_in;

  /// Internal FIFO.
  sc_core::sc_fifo_out<packet>* fifo;

  /// Specify the functionality of router per clock cycle.
  void main(void);

  template <unsigned _Input, unsigned _Output>
  router(::sc_core::sc_module_name name)
    : _x(-1)
    , _y(-1)
    , _router_input_ports(_Input)
    , _router_output_ports(_Output)
  {
    SC_METHOD(main);

    sensitive << clock.pos();

    activated_in = new sc_core::sc_in<bool>   [_Input];
    data_in      = new sc_core::sc_in<int>    [_Input];
    address_in   = new sc_core::sc_in<uint8_t>[_Input];

    fifo = new sc_core::sc_fifo_out<int>[_Output];
  }

  /// Use this function to set the coordinates of the router.
  void set_xy(int x, int y);

protected:

  /// Read a packet from the link.
  void read_packet(int iport);

  /// Location of the router.
  int _x, _y;

private:

  /// Number of input ports.
  const unsigned _router_input_ports;

  /// Number of ouput ports.
  const unsigned _router_output_ports;
}; // router

#endif // _ROUTER_ROUTER_H_
