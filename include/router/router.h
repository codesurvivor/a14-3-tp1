#ifndef ROUTER_H
#define ROUTER_H

#define Router_input_ports 1
#define Router_output_ports 4

#include <systemc.h>
#include <list>

// an empty packet has src_x == src_y == dest_x == dest_y == -1

struct packet
{
    int Addr;
    int data;
    int activated;

    packet(int Ad = -1, int data = -1)
        : Addr(Ad),data(data) , activated(activated)
    {
    }

}; // struct packet


SC_MODULE(router)
{
    sc_in<bool> clock; // port for the global clock
    sc_in<bool> activated[Router_input_ports ] ;
    sc_in<uint8_t> address[Router_input_ports ] ;
    sc_in<int> data[Router_input_ports ] ;
    // sc_in<packet> port_in[1]; // input ports
    // sc_out<packet> port_out[4]; // output ports
    //sc_out<uint8_t> address[Router_input_ports ] ;
    //sc_out<int> data[Router_input_ports ] ;

    int fifo_out_depth;

    sc_fifo<uint8_t > 		*address[Router_input_ports ];
    sc_fifo<int > 		*data[Router_input_ports ];

    void main(); // specify the functionality of router per clock cycle

    //  SC_CTOR(router)
    router(sc_module_name name, int fifo_out_depth_,sc_trace_file *F):sc_module(name)
      : x_(-1), y_(-1)
    {
        fifo_out_depth	=	fifo_out_depth_;
        fifo_in		=	new sc_fifo<int > ("fifo_in",fifo_in_depth_);

        fifo_out_depth	=	fifo_out_depth_;
        fifo_out	=	new sc_fifo<int > ("fifo_out",fifo_out_depth_);

        SC_METHOD(main);
        sensitive << clock.pos();
    }

    // use this function to set the coordinates of the router
    void set_xy(int x, int y);


    void getQueueSize(int * size);


    protected:
    //std::list<int> out_data_queue_[Router_output_ports]; // data output queues
    //std::list<int> out_addr_queue_[Router_output_ports]; // addr output queues

    int x_, y_; // location of the router

    void read_packet(int iport); // read a packet from the link
    void route_packet_xy(packet p); // route the packet to the output queue
}; // router

#endif // ROUTER_H
