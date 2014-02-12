#include <noc/noc.h>
#include <cstdlib>


namespace noc
{


noc::noc(sc_core::sc_module_name name, unsigned nb_input,
         unsigned nb_output,
         unsigned fifo_depth)
  : inputs(nb_input)
  , inputs_activated(nb_input)
  , inputs_acknoledge(nb_input)
  , outputs(nb_output)
  , arbiter_modes(nb_output)
  , fifo_choosen_index(nb_output)
  , _routers(nullptr)
  , _arbiters(nullptr)
  , _nb_input(nb_input)
  , _nb_output(nb_output)
  , _fifo_depth(fifo_depth)
{
  // Build routers.
  {
    _routers = reinterpret_cast<router*>(
          std::malloc(sizeof(router)*_nb_input));

    for (unsigned i = 0; i < _nb_input; ++i)
    {
      std::stringstream ss;
      ss << "router" << i;
      new(_routers + i) router(ss.str().c_str(), _nb_output);
    }
  }

  // Build arbiters.
  {
    _arbiters = reinterpret_cast<arbiter*>(
          std::malloc(sizeof(arbiter)*_nb_output));

    for (unsigned i = 0; i < _nb_output; ++i)
    {
      std::stringstream ss;
      ss << "arbiter" << i;
      new(_arbiters + i) arbiter(ss.str().c_str(), _nb_output, _fifo_depth);
    }
  }

  // Build fifos.
  {
    _fifos = reinterpret_cast<fifo*>(
          std::malloc(sizeof(fifo) * _nb_input * _nb_output));

    for (unsigned i = 0, j = 0, k = 0, end = _nb_input * _nb_output;
         i < end;
         ++i, ++j)
    {
      std::stringstream ss;
      ss << "fifo_" << j << '_' << k;
      new(_fifos + i) fifo(ss.str().c_str(), _fifo_depth);

      if (j = _nb_input) { j = 0; ++k; }
    }
  }

  // Connect everyone.
  {
    // Connect inputs, acknoledge and clock to routers.
    {
      for (unsigned i = 0; i < _nb_input; ++i)
      {
        _routers[i].input.bind(inputs[i]);
        _routers[i].activated_in.bind(inputs_activated[i]);
        _routers[i].clock.bind(clock);
        _routers[i].acknoledge.bind(inputs_acknoledge[i]);
      }
    }

    // Connect fifos to arbiters and routers.
    {
      for (unsigned i = 0; i < _nb_input; ++i)
        for (unsigned j = 0; j < _nb_output; ++j)
        {
          _routers[i].fifos[j].bind(get_fifo(i,j));
          _arbiters[j].fifos[i].bind(get_fifo(i,j));
        }
    }

    // Connect arbiters to outputs, modes, choosen fifo and clock.
    {
      for (unsigned j = 0; j < _nb_output; ++j)
      {
        _arbiters[j].output.bind(outputs[j]);
        _arbiters[j].arb_type.bind(arbiter_modes[j]);
        _arbiters[j].clock.bind(clock);
        _arbiters[j].choice_out.bind(fifo_choosen_index[j]);
      }
    }
  }
}


noc::~noc(void)
{
  // Free routers.
  for (unsigned i = 0; i < _nb_input; ++i)
    _routers[i].~router();
  std::free(_routers);

  // Free arbiters.
  for (unsigned i = 0; i < _nb_output; ++i)
    _arbiters[i].~arbiter();
  std::free(_arbiters);

  // Free fifos.
  for (unsigned i = 0, end = _nb_input * _nb_output; i < end; ++i)
    _fifos[i].~fifo();
  std::free(_arbiters);
}


} // noc
