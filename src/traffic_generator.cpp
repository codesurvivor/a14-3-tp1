#include <noc/traffic_generator.h>


namespace noc
{


abstract_traffic_generator::abstract_traffic_generator(
    sc_core::sc_module_name name)
  : ::sc_core::sc_module(name)
  , _address_min(0)
  , _address_max(1) {}


void abstract_traffic_generator::emit_random_package(void)
{
  uint8_t add =
      (rand() % (_address_max - _address_min)) + _address_min;
  int dat = rand();

  increase_time();
  address.write(add);
  data.write(dat);
  wait(clock.negedge_event());
  activated.write(true);
  wait(clock.posedge_event());
  //increase_time();
  wait(clock.negedge_event());
  activated.write(false);
}


stream_generator::stream_generator(sc_core::sc_module_name name)
  : abstract_traffic_generator(name)
  , _current_cycle_mod(0)
  , _period(1)
{
  SC_THREAD(generate);
  dont_initialize();

  sensitive << clock.pos();
}


void stream_generator::generate()
{
  while(true)
  {
    wait();

    if (_current_cycle_mod == 0)
      emit_random_package();

    increase_time();
  }
}


void stream_generator::increase_time(void)
{
  ++_current_cycle_mod;
  _current_cycle_mod %= _period;
}


burst_generator::burst_generator(sc_core::sc_module_name name)
  : abstract_traffic_generator(name)
  , _current_long_cycle_mod(0)
  , _long_period(1)
  , _current_short_cycle_mod(0)
  , _short_period(1)
  , _burst_length(1)
{
  SC_THREAD(generate);
  dont_initialize();

  sensitive << clock.pos();
}


void burst_generator::generate(void)
{
  while(true)
  {
    wait();

    if (_current_long_cycle_mod < _burst_length &&
        _current_short_cycle_mod == 0)
      emit_random_package();

    increase_time();
  }
}


void burst_generator::increase_time(void)
{
  ++_current_long_cycle_mod;
  _current_long_cycle_mod %= _long_period;

  ++_current_short_cycle_mod;
  _current_short_cycle_mod %= _short_period;
}


} // noc
