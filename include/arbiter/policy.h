#ifndef _ROUTER_POLICY_H_
#define _ROUTER_POLICY_H_

#include <systemc>

struct packet;
class router;

class abstract_routing_policy
{
public:

  friend class router;

  /**
   * @brief Choose the index of the fifo to consume.
   *
   * If -1 is return, then has to be done.
   */
  virtual int choose(void) = 0;

private:

  void set_fifo(sc_core::sc_fifo<packet>* fifos, unsigned fifo_nb)
  {
    _fifos = fifos;
    _fifo_nb = fifo_nb;
  }

  unsigned _fifo_nb;
  sc_core::sc_fifo<packet>* _fifos;
};


#endif // _ROUTER_POLICY_H_
