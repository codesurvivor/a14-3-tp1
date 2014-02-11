#include <noc/noc.h>

#include <noc/traffic_generator.h>


int sc_main(int argc, char **argv)
{
  sc_core::sc_clock clock;

  noc::noc noc("noc", 4, 4);
  noc.clock.bind(clock);
  for (unsigned i = 0; i < 4; ++i)
    noc.arbiter_modes[i].write((noc::arbiter_mode)(i));

  std::shared_ptr<noc::stream_generator> tg[4];

  for (unsigned i = 0; i < 4; ++i)
  {
    // Set stream_generator array.
    {
      std::stringstream ss;
      ss << "stream_generator" << i;

      tg[i] = std::make_shared<noc::stream_generator>(ss.str().c_str());
      tg[i]->set_address_properties(0, 4);
      tg[i]->set_period(3*(i+1));

      tg[i]->clock.bind(clock);
      tg[i]->activated.bind(noc.inputs_activated[i]);
      tg[i]->acknoledge.bind(noc.inputs_acknoledge[i]);
      tg[i]->output.bind(noc.inputs[i]);
    }
  }

  sc_core::vcd_trace_file* const file =
      (sc_core::vcd_trace_file*) sc_core::sc_create_vcd_trace_file("noc");

  sc_core::sc_trace(file, clock , "clock");

  for (unsigned i = 0; i < 4; ++i)
  {
    {
      std::stringstream ss;
      ss << "stream_act_" << i;
      sc_core::sc_trace(file, noc.inputs_activated[i], ss.str().c_str());
    }
    {
      std::stringstream ss;
      ss << "stream_ack_" << i;
      sc_core::sc_trace(file, noc.inputs_acknoledge[i], ss.str().c_str());
    }
    {
      std::stringstream ss;
      ss << "input_" << i;
      sc_core::sc_trace(file, noc.inputs[i], ss.str().c_str());
    }
    {
      std::stringstream ss;
      ss << "output_" << i;
      sc_core::sc_trace(file, noc.outputs[i], ss.str().c_str());
    }
  }

  sc_core::sc_start(1000, sc_core::SC_NS);

  sc_core::sc_close_vcd_trace_file(file);

  return 0;
}
