#include <cstdlib>

#include <systemc>


struct abstract_traffic_generator
    : public ::sc_core::sc_module
{
  sc_core::sc_in<bool> clock;
  sc_core::sc_out<bool> activated;
  sc_core::sc_out<uint8_t> address;
  sc_core::sc_out<int> data;

  // Set the address range of the emmited packages.
  inline void set_address_range(uint8_t min, uint8_t max)
  {
    _address_min = min;
    _address_max = max;
  }

  abstract_traffic_generator(void)
    : _address_min(0)
    , _address_max(1)
  {}

protected:

  void emit_random_package(void)
  {
    uint8_t add = (rand() % (_address_max - _address_min)) + _address_min;
    int     dat = rand();

    address.write(add);
    data.write(dat);
    activated.write(true);
    _need_down_activation = true;
  }

  void check_activated_state(void)
  {
    if (_need_down_activation)
    {
      activated.write(false);
      _need_down_activation = false;
    }
  }

  uint8_t _address_min, _address_max;

private:

  bool _need_down_activation;
};

class stream_generator
  : public abstract_traffic_generator
{
  unsigned _current_cycle_mod;
  unsigned _period;

public:

  SC_CTOR(stream_generator)
    : _current_cycle_mod(0)
    , _period(1)
  {
    SC_METHOD(generate);
    sensitive << clock.pos();
  }

  void generate(void)
  {
    if (_current_cycle_mod == 0)
    {
      emit_random_package();
    }
    else
    {
      check_activated_state();
    }

    ++_current_cycle_mod;
    _current_cycle_mod %= _period;
  }

  // Set the emission period (in cycle).
  inline void set_period(unsigned period)
  { _period = period; }

  // Set the emission offset (in cycle).
  inline void set_offset(unsigned offset)
  { _current_cycle_mod = offset; }
};

class burst_generator
  : public abstract_traffic_generator
{
  unsigned _current_long_cycle_mod, _current_short_cycle_mod;
  unsigned _long_period, _short_period, _burst_length;

public:

  SC_CTOR(burst_generator)
    : _current_long_cycle_mod(0)
    , _long_period(1)
    , _current_short_cycle_mod(0)
    , _short_period(1)
    , _burst_length(1)
  {
    SC_METHOD(generate);
    sensitive << clock.pos();
  }

  void generate(void)
  {
    if (_current_long_cycle_mod < _burst_length)
    {
      if (_current_short_cycle_mod == 0)
      {
        emit_random_package();
      }
      else
      {
        check_activated_state();
      }
    }
    else
    {
      check_activated_state();
    }

    ++_current_long_cycle_mod;
    _current_long_cycle_mod %= _long_period;

    ++_current_short_cycle_mod;
    _current_short_cycle_mod %= _short_period;
  }

  // Set the emission period (in cycle).
  inline void set_burst(
      unsigned long_period,
      unsigned short_period,
      unsigned burst_length)
  {
    _long_period  = long_period;
    _short_period = short_period;
    _burst_length = burst_length;
  }

  // Set the emission offset (in cycle).
  inline void set_offset(unsigned offset)
  {
    _current_long_cycle_mod = offset;
    _current_short_cycle_mod = offset;
  }
};
