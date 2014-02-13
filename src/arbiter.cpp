#include <noc/arbiter.h>

#include <limits>


namespace noc
{

// Rand Mode

void arbiter::init_rand(void)
{ _lfsr = 1; }


uint8_t arbiter::choose_rand(void)
{
  // We can compute a random number in one clock step using pipelining
  // for the different operations.

  uint8_t randomNumber = 0;

  for (int i = 0; i < 8; ++i)
  {
    int const id = i*4;
    uint8_t bit = (_lfsr >> id) % 2;
    randomNumber += bit << i;
  }

  int availableFifos = 0;
  for (int i = 0; i < _nb_fifos; i++)
    if (fifos[i].num_available() > 0 )
      ++availableFifos;

  if (availableFifos == 0)
    return std::numeric_limits<uint8_t>::max();

  const int chosen = randomNumber % availableFifos;
  int counter = 0;

  for (int i = 0; i < _nb_fifos; i++)
  {
    if (fifos[i].num_available() > 0)
    {
      if (counter++ == chosen)
        return i;
    }
  }

  // Unreachable
  assert(false);
}


void arbiter::update_rand(char numFifo)
{
  int retro = ((_lfsr>>1) + (_lfsr>>6) + (_lfsr>>16))%2;
  _lfsr = (_lfsr << 1) + retro;
}


// LRU Mode

void arbiter::init_lru(void)
{ for(int i = 0; i < _nb_fifos; ++i) _lru_index[i] = i;}


uint8_t arbiter::choose_lru(void)
{
  // We can compute it in parallel with only one clock step.

  for (int i = 0; i < _nb_fifos; i++)
    if (fifos[_lru_index[i]].num_available() > 0 )
      return _lru_index[i];

  return std::numeric_limits<uint8_t>::max();
}


void arbiter::update_lru(char numFifo)
{
  // We can compute it in parallel with only one clock step.

  bool found = false;

  for(int i = 0; i < _nb_fifos; i++)
  {
    /**/ if (found)
      _lru_index[i-1] = _lru_index[i];
    else if (_lru_index[i] == numFifo)
      found = true;
  }

  _lru_index[_nb_fifos-1] = numFifo;
}


// FIFO Mode

void arbiter::init_fifo(void)
{ _in_fifos = 0; }


int arbiter::choose_fifo(void)
{
  if (_in_fifos == 0) return -1;
  return _pushs_in_fifos[0];
}


void arbiter::update_fifo(unsigned char numFifo)
{
  bool found = false;

  for (unsigned int i = 0; i < _in_fifos; ++i)
  {
    /**/ if (found)
      _pushs_in_fifos[i-1] = _pushs_in_fifos[i];
    else if (_pushs_in_fifos[i] == numFifo)
      found = true;
  }

  --_in_fifos;
}


void arbiter::pushed_in_fifo(char idFifo)
{
  std::lock_guard<std::mutex> lock(_init_id_mutex);
  _pushs_in_fifos.at(_in_fifos) = idFifo;
  ++_in_fifos;
  assert(_in_fifos < _pushs_in_fifos.size());
}


// Fixed Mode

uint8_t arbiter::choose_fixed(void)
{
  // This can be done by adding a "callback" when a fifo is pushed,
  // and another when a fifo is read.
  // Callbacks, which update at each push, the index of the first
  // non empty fifo.

  for (int i = 0; i < _nb_fifos; i++)
    if (fifos[i].num_available() > 0 )
      return i;

  return std::numeric_limits<uint8_t>::max();
}


// Round Robin Mode

void arbiter::init_round_robin(void)
{ _last_used = _nb_fifos - 1; }


uint8_t arbiter::choose_round_robin(void)
{
  // Same remark than fixed mode.

  for (int i = 0; i < _nb_fifos; i++)
    if (fifos[(_last_used + 1 + i) % _nb_fifos].num_available() > 0)
      return (_last_used + 1 + i) % _nb_fifos;

  return std::numeric_limits<uint8_t>::max();
}


void arbiter::update_round_robin(char numFifo)
{ _last_used = numFifo; }


// Core functions

void arbiter::process(void)
{
  //std::cout << "Entering in process thread." << std::endl;
  while (true)
  {
    wait(clock.posedge_event());

    uint8_t choice = std::numeric_limits<uint8_t>::max();
    const int type = arb_type.read();
    switch(type)
    {
      case RAND :
        choice = choose_rand();
        break;
      case LRU :
        choice = choose_lru();
        break;
      case FIFO :
        choice = choose_fifo();
        break;
      case FIXED :
        choice = choose_fixed();
        break;
      case ROUNDROBIN :
        choice = choose_round_robin();
        break;
      default:
        break;
    }

    choice_out.write(choice);

    if (choice == std::numeric_limits<uint8_t>::max())
      continue;

    //std::cout << "Arbitering type: " << type << std::endl;
    //std::cout << "|\n--> Current choice: " << unsigned(choice) << "\n\n";

    // Choice processing time.
    wait(clock.negedge_event());
    wait(clock.posedge_event());

    packet pack= fifos[choice].read();
    output.write(pack);

    update(choice);
  }
}


void arbiter::update(char numFifo)
{
  update_rand(numFifo);
  update_lru(numFifo);
  update_fifo(numFifo);
  update_round_robin(numFifo);
}


void arbiter::pushed_in_fifo(void)
{
  unsigned id = get_id();
  //std::cout << "New pushedInFifo thread with id: " << id << std::endl;

  while(true)
  {
    wait(fifos[id].data_written_event());
    //std::cout << "fifo[" << id << "] has been written." << std::endl;
    pushed_in_fifo(id);
  }
}


arbiter::arbiter(
    sc_core::sc_module_name name,
    unsigned nb_fifo,
    unsigned fifo_depth)
  : ::sc_core::sc_module(name)
  , fifos(nullptr)
  , clock("clock")
  , arb_type("arbitration_type")
  , output("output")
  , choice_out("fifo_choosen_index")
  , _nb_fifos(nb_fifo)
  , _pushs_in_fifos(2*_nb_fifos*_fifo_depth, 0)
  , _lru_index(_nb_fifos, 0)
  , _fifo_depth(fifo_depth)
  , _id(0)
{
  choice_out.initialize(std::numeric_limits<uint8_t>::max());
  output.initialize(packet());

  init_rand();
  init_lru();
  init_fifo();
  init_round_robin();

  fifos = new ::sc_core::sc_fifo_in<packet>[_nb_fifos];

  SC_THREAD(process);
  sensitive << clock.pos();

  // Declare the NB_FIFO threads: 1 by FIFO.
  for (unsigned i = 0; i < _nb_fifos; ++i)
  {
    std::stringstream ss;
    ss << "pushed_in_fifo" << i;
    ::sc_core::sc_process_handle handle =
        sc_core::sc_get_curr_simcontext()->create_thread_process(
          ss.str().c_str(), false,
          SC_MAKE_FUNC_PTR(SC_CURRENT_USER_MODULE,
                           pushed_in_fifo), this, 0 );
    sensitive << handle;
    sensitive_pos << handle;
    sensitive_neg << handle;
    sensitive << clock.pos();
  }
}


arbiter::~arbiter(void)
{ delete[] fifos; }


unsigned arbiter::get_id(void)
{
  std::lock_guard<std::mutex> lock(_init_id_mutex);
  return _id++;
}


} // noc
