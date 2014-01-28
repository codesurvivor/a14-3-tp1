#include <cstdlib>

#include <systemc>


namespace noc
{

struct abstract_traffic_generator
    : public ::sc_core::sc_module
{
  sc_core::sc_in<bool> clock;
  sc_core::sc_out<bool> activated;
  sc_core::sc_out<uint8_t> address;
  sc_core::sc_out<int> data;

  // Set the address range of the emmited packages.
  inline void set_address_range(uint8_t min, uint8_t max)
  { _address_min = min; _address_max = max; }

  abstract_traffic_generator(::sc_core::sc_module_name name);

  virtual void increase_time(void) = 0;

protected:

  void emit_random_package(void);
  uint8_t _address_min, _address_max;
};

class stream_generator
  : public abstract_traffic_generator
{
  unsigned _current_cycle_mod;
  unsigned _period;

public:
  typedef stream_generator SC_CURRENT_USER_MODULE;

  stream_generator(::sc_core::sc_module_name name);

  void generate(void);

  virtual void increase_time(void);

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
  typedef burst_generator SC_CURRENT_USER_MODULE;

  burst_generator(::sc_core::sc_module_name name);

  void generate(void);

  virtual void increase_time(void);

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

} // noc
