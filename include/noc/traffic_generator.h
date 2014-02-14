#include <cstdlib>

#include <functional>

#include <systemc>

#include <noc/packet.h>


namespace noc
{

class abstract_traffic_generator
    : public ::sc_core::sc_module
{
public:

  typedef abstract_traffic_generator SC_CURRENT_USER_MODULE;
  typedef std::function<void(packet&)> creation_callback;

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

  abstract_traffic_generator(
      ::sc_core::sc_module_name name);

  void set_packet_creation_callback(
      creation_callback const& callback)
  { _callback = callback; }

protected:

  void emit_random_package(void);

  unsigned get_clock_count(void) const
  { return _clock_count; }

  void set_clock_counter(unsigned val)
  { _clock_count = val; }

  void reset_clock_count(void)
  { set_clock_counter(0); }

private:

  void clock_counter(void);

  inline uint8_t get_address(void);
  void choose_new_random_address(void);

  uint8_t _address_min, _address_max;
  unsigned _address_max_life_time;

  uint8_t _current_address;
  uint8_t _current_address_life_time;

  unsigned _clock_count;

  creation_callback _callback;
};

class stream_generator
  : public abstract_traffic_generator
{
  unsigned _period;

public:
  typedef stream_generator SC_CURRENT_USER_MODULE;

  stream_generator(::sc_core::sc_module_name name);

  // Set the emission period (in cycle).
  inline void set_period(unsigned period)
  { _period = period; }

  // Set the emission offset (in cycle).
  inline void set_offset(unsigned offset)
  { set_clock_counter(offset); }

private:

  void generate(void);
};

class burst_generator
  : public abstract_traffic_generator
{
  unsigned _current_packet_count;
  unsigned _long_period, _short_period, _burst_length;

public:
  typedef burst_generator SC_CURRENT_USER_MODULE;

  burst_generator(::sc_core::sc_module_name name);

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
  void set_offset(unsigned offset)
  { set_clock_counter(offset); }

private:

  void generate(void);
};

} // noc
