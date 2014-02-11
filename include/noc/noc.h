#ifndef _NOC_NOC_H_
#define _NOC_NOC_H_

#include <noc/forward.h>

#include <memory>

#include <systemc>

#include <noc/router.h>
#include <noc/arbiter.h>
#include <noc/packet.h>


namespace noc
{

class noc
    : public ::sc_core::sc_module
{
public:

  // Inputs.
  sc_core::sc_in<bool> clock;
  std::vector<sc_core::sc_signal<packet> > inputs;
  std::vector<sc_core::sc_signal<bool> > inputs_activated;

  // Outputs.
  std::vector<sc_core::sc_signal<bool> > inputs_acknoledge;
  std::vector<sc_core::sc_signal<packet> > outputs;
  std::vector<sc_core::sc_signal<arbiter_mode> > arbiter_modes;

  // Measure.
  std::vector<sc_core::sc_signal<uint8_t> > fifo_choosen_index;

private:

  typedef sc_core::sc_fifo<packet> fifo;

  router* _routers;
  arbiter* _arbiters;
  fifo* _fifos;

  unsigned _nb_input;
  unsigned _nb_output;
  unsigned _fifo_depth;

public:

  typedef noc SC_CURRENT_USER_MODULE;

  noc(::sc_core::sc_module_name name,
      unsigned nb_input,
      unsigned nb_output,
      unsigned fifo_depth = 16);

  virtual ~noc(void);

private:

  fifo& get_fifo(unsigned input, unsigned output)
  { return _fifos[_nb_input*output + input]; }
};

} // noc

#endif // _NOC_NOC_H_
