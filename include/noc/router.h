#ifndef _ROUTER_ROUTER_H_
#define _ROUTER_ROUTER_H_

#include <noc/router.h>

#include <list>
#include <sstream>
#include <memory>
#include <functional>

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

  typedef std::function<void(packet&)> marker_callback;

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

public:

  router(::sc_core::sc_module_name name, unsigned output);

  ~router(void);

  /*!
   * @brief Set the packet marker callback.
   *
   * The marker callback is called when a packet is pushed in a fifo.
   */
  void set_marker_callback(marker_callback const& callback)
  { _callback = callback; }

protected:

  /// Read a packet from the link.
  void read_packet(void);

private:

  /// Specify the functionality of router per clock cycle.
  void main(void);

  /// Number of ouput ports.
  const unsigned _router_output_ports;

  /// Fifo insertion packet marking callback.
  marker_callback _callback;
};

} // noc

#endif // _ROUTER_ROUTER_H_
