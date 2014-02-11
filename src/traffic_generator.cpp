#include <noc/traffic_generator.h>


namespace noc
{


abstract_traffic_generator::abstract_traffic_generator(
    sc_core::sc_module_name name)
  : ::sc_core::sc_module(name)
  , clock("clock")
  , acknoledge("acknoledge")
  , activated("activated")
  , output("output")
  , _address_min(0)
  , _address_max(1) {}


void abstract_traffic_generator::emit_random_package(void)
{
  packet p;
  p.address = get_address();
  p.data = rand();

  /* ^  _____
   * |_|     |___continue
   * ---------------------> traffic_generator.activated
   * ^      ___
   * |_____|   |_
   * ---------------------> traffic_generator.acknoledge
   */

  increase_time();
  output.write(p);
  wait(clock.negedge_event());
  activated.write(true);
  wait(acknoledge.posedge_event());
  activated.write(false);
  wait(acknoledge.negedge_event());
}


uint8_t abstract_traffic_generator::get_address(void)
{
  --_current_address_life_time;
  uint8_t res = _current_address;
  if (!_current_address_life_time) choose_new_random_address();
  return res;
}


void abstract_traffic_generator::choose_new_random_address(void)
{
  _current_address =
      (rand() % (_address_max - _address_min)) + _address_min;
  _current_address_life_time = (rand() % _address_max_life_time);
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


void stream_generator::generate(void)
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
