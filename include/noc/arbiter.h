#ifndef _ARBITER_ARBITER_H_
#define _ARBITER_ARBITER_H_

#include <noc/forward.h>

#include <thread>
#include <mutex>

#include <systemc>

#include <noc/packet.h>


namespace noc
{

class arbiter : public ::sc_core::sc_module
{
public:

  typedef arbiter SC_CURRENT_USER_MODULE;

  sc_core::sc_fifo_in<packet>* fifos;
  sc_core::sc_in<bool> clock;
  sc_core::sc_in<arbiter_mode> arb_type;
  sc_core::sc_out<packet> output;
  sc_core::sc_out<uint8_t> choice_out;

  arbiter(::sc_core::sc_module_name name,
          unsigned nb_fifo,
          unsigned fifo_depth = 16);

  virtual ~arbiter(void);

  inline unsigned num_of_fifos(void) const
  { return _nb_fifos; }

private:

  unsigned _nb_fifos;
  unsigned _fifo_depth;

  // Id assignement.
  std::mutex _init_id_mutex;
  unsigned _id;

  // For Rand Mode
  int _lfsr;

  // For LRU Mode
  std::vector<uint8_t> _lru_index;

  // For FIFO Mode
  std::vector<uint8_t> _pushs_in_fifos;
  unsigned int _in_fifos;

  // For Round Robin Mode
  char _last_used;

  unsigned get_id(void);

  // Rand Mode
  void init_rand(void);
  uint8_t choose_rand(void);
  void update_rand(char numFifo);

  // LRU Mode
  void init_lru(void);
  uint8_t choose_lru(void);
  void update_lru(char numFifo);

  // FIFO Mode
  void init_fifo(void);
  int choose_fifo(void);
  void update_fifo(unsigned char numFifo);
  void pushed_in_fifo(char idFifo);

  // Fixed Mode
  uint8_t choose_fixed(void);

  // Round Robin Mode
  void init_round_robin(void);
  uint8_t choose_round_robin(void);
  void update_round_robin(char numFifo);

  // Core functions
  void process(void);
  void update(char numFifo);

  //Dependencies
  void pushed_in_fifo(void);
};

} // noc

#endif // _ARBITER_ARBITER_H_
