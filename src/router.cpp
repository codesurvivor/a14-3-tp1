#include "router.h"
#define Router_input_ports 1
#define Router_output_ports 4

void router::main()
{
        
        assert((x_ != -1) && (y_ != -1)); // to identify PE

        for (int iport = 0; iport < Router_input_ports ; ++iport)
                read_packet(iport);
}

void router::set_xy(int x, int y)
{
        assert((x_ == -1) && (y_ == -1)); // set once only
        assert((x != -1) && (y != -1)); // must use a legal location

        x_ = x;
        y_ = y;
}

void router::read_packet(int iport)
{
        assert(iport < Router_input_ports);
	packet p.activated = activated[iport].read();
        packet p.data = data[iport].read();
	 packet p.Addr = address[iport].read();

        if ((p.Addr == -1)||(p.activated =! 1 ))
                return; // empty packet

        route_packet_xy(p);
  
}


void router::route_packet_xy(packet p)
{
        if ((p.Addr == -1) )
        {
                printf("router (%d,%d): drop packet with invalid destination"
                        " (%d,%d)->(%d,%d)\n",
                        p.Addr);
                return;
        }

        if (p.Addr == 1) 
        {
                out_addr_queue_[1].push_back(p.Addr);
		out_data_queue_[1].push_back(p.data);
        }
        else if (p.Addr == 2) /
        {
                out_addr_queue_[2].push_back(p.Addr);
		out_data_queue_[2].push_back(p.data);
        }
	else if (p.Addr == 3) /
        {
                out_addr_queue_[3].push_back(p.Addr);
		out_data_queue_[3].push_back(p.data);
        }
        else 
        {
                out_addr_queue_[Router_output_ports].push_back(p.Addr);
		out_data_queue_[Router_output_ports].push_back(p.data);
        }
}


void router::getQueueSize(int * size)
{
                for(int i = 0;i<Router_output_ports;i++)
        {
                size[i] = (int)out_data_queue_[i].size();
        }
}

