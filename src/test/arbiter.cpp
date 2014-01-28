#include <systemc>
#include "arbiter/arbiter.h"
#include "traffic/traffic_generator.h"
#include "router/packet.h"

SC_MODULE(packet_wrapper)
{
    public:
    sc_core::sc_in<bool> clock;
    sc_core::sc_in<uint8_t> addr;
    sc_core::sc_in<int> data;
    sc_core::sc_fifo_out<packet> packs;

    void process()
    {
        packs.write(packet(addr.read(), data.read()));
    }

    SC_CTOR(packet_wrapper)
    {
        SC_METHOD(process);
        dont_initialize();
        sensitive << clock.pos();
    }
};

int sc_main(int argc, char *argv[])
{
    sc_core::vcd_trace_file* const file =
            (sc_core::vcd_trace_file*) sc_core::sc_create_vcd_trace_file("output");

    sc_core::sc_clock clock;
    sc_core::sc_signal<int> arbType;
    sc_core::sc_signal<packet> out;
    sc_core::sc_fifo<packet> fifos[4];

    sc_core::sc_signal<bool> stream_activated[4];
    sc_core::sc_signal<uint8_t> stream_addr[4];
    sc_core::sc_signal<int> stream_data[4];

    arbiter* ab = new arbiter("arbiter");
    ab->clock(clock);
    ab->arbType(arbType);
    ab->out(out);

    arbType.write(arbiterMode::FIXED);

    stream_generator* tg[4];
    packet_wrapper* pw[4];

    for (int i=0; i < 4; ++i)
    {
        tg[i] = new stream_generator("stream_generator");
        tg[i]->set_address_range(10, 100);
        tg[i]->set_period(10);

        tg[i]->activated(stream_activated[i]);
        tg[i]->address(stream_addr[i]);
        tg[i]->data(stream_data[i]);

        pw[i] = new packet_wrapper("packet_wrapper");
        pw[i]->clock(clock);
        pw[i]->addr(stream_addr[i]);
        pw[i]->data(stream_data[i]);
        pw[i]->packs(fifos[i]);

        ab->f[i](fifos[i]);
    }

    sc_core::sc_trace(file, ab->out, "arbiter_out" );

    sc_core::sc_start(1000, sc_core::SC_NS);

    sc_core::sc_close_vcd_trace_file(file);

    return 0;
}
