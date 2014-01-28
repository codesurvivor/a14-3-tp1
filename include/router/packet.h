#ifndef _ROUTER_PACKET_H_
#define _ROUTER_PACKET_H_

#include <systemc>
#include <sstream>

/**
 * @brief Transmitted packet on the network.
 *
 * An empty packet has src_x == src_y == dest_x == dest_y == -1.
 */
struct packet
{
    uint8_t address;
    int data;

    explicit packet(uint8_t add = 0, int data = 0)
        : address(add)
        , data(data)
    {}

    bool operator==(const packet& p)
    {
        if(p.address == address && p.data == data)
        {
            return true;
        }

        return false;
    }

}; // struct packet

std::ostream& operator<< (std::ostream& stream, packet const& p);

namespace sc_core
{


inline void sc_trace(sc_trace_file* file, ::packet const& pack, std::string const& name)
{
    {
        std::ostringstream ss;
        ss << name << ".address";
        sc_trace(file, pack.address, ss.str(), 8);
    }
    {
        std::ostringstream ss;
        ss << name << ".data";
        sc_trace(file, pack.data, ss.str(), 32);
    }
}

}

#endif // _ROUTER_PACKET_H_
