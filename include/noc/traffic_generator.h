#include <cstdlib>

#include <systemc>

#include <noc/packet.h>


namespace noc
{

class abstract_traffic_generator
    : public ::sc_core::sc_module
{
public:

  sc_core::sc_in<bool> clock;
  sc_core::sc_in<bool> acknoledge;
  sc_core::sc_out<bool> activated;
  sc_core::sc_out<packet> output;

  // Set the address range of the emmited packages.
  inline void set_address_properties(
      uint8_t min, uint8_t max, unsigned max_life_time = 20)
  {
    _address_min = min;
    _address_max = max;
    _address_max_life_time = max_life_time;
    choose_new_random_address();
  }

  abstract_traffic_generator(::sc_core::sc_module_name name);

  virtual void increase_time(void) = 0;

protected:

  void emit_random_package(void);

private:

  inline uint8_t get_address(void);
  void choose_new_random_address(void);

  uint8_t _address_min, _address_max;
  unsigned _address_max_life_time;

  uint8_t _current_address;
  uint8_t _current_address_life_time;
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
