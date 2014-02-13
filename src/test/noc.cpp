#include <noc/noc.h>

#include <cstdio>
#include <functional>

#include <noc/traffic_generator.h>

#include "packet_tracer.h"


void display_arbitration(void)
{
  std::cerr
      << "  Arbitration map:\n"
         "    0 -> RAND\n"
         "    1 -> LRU\n"
         "    2 -> FIFO\n"
         "    3 -> FIXED\n"
         "    4 -> ROUNDROBIN\n";
}


void display_usage(std::string const& prog_name)
{
  std::cerr
      << "usage: "
      << prog_name
      << " <nb_input> <nb_output> <arbiter_mode> <execution_time_ns>\n\n";

  display_arbitration();
}


int sc_main(int argc, char **argv)
{
  if (argc < 5)
  {
    display_usage(argv[0]);

    return 1;
  }

  unsigned
      nb_input = std::atoi(argv[1]),
      nb_output = std::atoi(argv[2]),
      arbiter_mode = std::atoi(argv[3]),
      execution_time_ns = std::atoi(argv[4]);

  if (arbiter_mode >= noc::arbiter_mode::ARBITER_MODE_LAST)
  {
    std::cerr
        << "Arbitration mode have to be less than "
        << unsigned(noc::arbiter_mode::ARBITER_MODE_LAST)
        << std::endl << std::endl;

    display_arbitration();

    return 2;
  }

  // The general clock.
  sc_core::sc_clock clock(
        "clock",
        sc_core::sc_time(CLOCK_PERIOD_NS, sc_core::SC_NS));

  // The noc module.
  noc::noc noc("noc", nb_input, nb_output);
  {
    noc.clock.bind(clock);
    for (unsigned i = 0; i < nb_output; ++i)
      noc.arbiter_modes[i].write((noc::arbiter_mode)(arbiter_mode));
  }

  // The packet tracer (get statistics on packets).
  test::packet_tracer tracer("packet_tracer", nb_output);
  {
    tracer.clock.bind(clock);
    for (unsigned i = 0; i < nb_output; ++i)
      tracer.inputs[i].bind(noc.outputs[i]);

    noc::abstract_traffic_generator::creation_callback callback =
        std::bind(&test::packet_tracer::prepare_packet_insertion_in_fifo,
                  &tracer,
                  std::placeholders::_1);

    noc.set_marker_callback(callback);
  }

  // Set stream_generator array.
  std::shared_ptr<noc::stream_generator> tg[nb_input];
  {
    noc::abstract_traffic_generator::creation_callback callback =
        std::bind(&test::packet_tracer::prepare_packet,
                  &tracer,
                  std::placeholders::_1);

    for (unsigned i = 0; i < nb_input; ++i)
    {
      std::stringstream ss;
      ss << "stream_generator" << i;

      tg[i] = std::make_shared<noc::stream_generator>(ss.str().c_str());
      tg[i]->set_address_properties(0, nb_output);
      tg[i]->set_period(3*(i+1));

      tg[i]->clock.bind(clock);
      tg[i]->activated.bind(noc.inputs_activated[i]);
      tg[i]->acknoledge.bind(noc.inputs_acknoledge[i]);
      tg[i]->output.bind(noc.inputs[i]);

      tg[i]->set_packet_creation_callback(callback);
    }
  }

  // Tracing.
  sc_core::vcd_trace_file* const file =
      reinterpret_cast<sc_core::vcd_trace_file*>(
        sc_core::sc_create_vcd_trace_file("noc"));
  {
    sc_core::sc_trace(file, clock , "clock");

    for (unsigned i = 0; i < nb_input; ++i)
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
    }

    for (unsigned i = 0; i < nb_output; ++i)
    {
      std::stringstream ss;
      ss << "output_" << i;
      sc_core::sc_trace(file, noc.outputs[i], ss.str().c_str());
    }
  }

  sc_core::sc_start(double(execution_time_ns), sc_core::SC_NS);

  sc_core::sc_close_vcd_trace_file(file);

  return 0;
}
