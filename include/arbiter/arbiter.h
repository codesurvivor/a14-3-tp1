#ifndef _ARBITER_ARBITER_H_
#define _ARBITER_ARBITER_H_

#include <systemc>
#include "router/packet.h"

#define NB_FIFO 4
#define FIFO_SIZE 8

enum arbiterMode {RAND, LRU, FIFO, FIXED, ROUNDROBIN};

SC_MODULE(arbiter)
{
    sc_core::sc_fifo_in<packet> f[NB_FIFO];
    sc_core::sc_in<bool> clock;
    sc_core::sc_in<int> arbType;
    sc_core::sc_out<packet> out;

    // For Rand Mode
    int lfsr;

    // For LRU Mode
    char LruIndex[4];

    // For FIFO Mode
    char pushsInFifos[NB_FIFO*FIFO_SIZE];
    unsigned int inFifos;

    // For Round Robin Mode
    char lastUsed;

    // Rand Mode
    char chooseRand();
    void updateRand(char numFifo);

    // LRU Mode
    void initLru();
    char chooseLru();
    void updateLru(char numFifo);

    // FIFO Mode
    void initFifo();
    int chooseFifo();
    void updateFifo(unsigned char numFifo);
    void pushedInFifo(char idFifo);

    // Fixed Mode
    char chooseFixed();

    // Round Robin Mode
    void initRoundRobin();
    char chooseRoundRobin();
    void updateRoundRobin(char numFifo);

    // Core functions
    void process();
    void update(char numFifo)
    {
        updateRand(numFifo);
        updateLru(numFifo);
        updateFifo(numFifo);
        updateRoundRobin(numFifo);
    }

    //Dependencies
    void pushedInFifo0()
    {
        pushedInFifo(0);
    }

    void pushedInFifo1()
    {
        pushedInFifo(1);
    }

    void pushedInFifo2()
    {
        pushedInFifo(2);
    }

    void pushedInFifo3()
    {
        pushedInFifo(3);
    }

    SC_CTOR(arbiter)
    {
        initLru();
        initFifo();
        initRoundRobin();

        SC_METHOD(process);
        sensitive << clock.pos();

        SC_METHOD(pushedInFifo0);
        sensitive << f[0].data_written_event();

        SC_METHOD(pushedInFifo1);
        sensitive << f[1].data_written_event();

        SC_METHOD(pushedInFifo2);
        sensitive << f[2].data_written_event();

        SC_METHOD(pushedInFifo3);
        sensitive << f[3].data_written_event();
    }
};

#endif // _ARBITER_ARBITER_H_
