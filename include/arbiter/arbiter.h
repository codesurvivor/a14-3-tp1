#ifndef _ARBITER_ARBITER_H_
#define _ARBITER_ARBITER_H_

#include <systemc>
#define NB_FIFO 4
#define FIFO_SIZE 8

struct Packet
{
    uint8_t address;
    int data;
};

enum arbiterMode {RAND, LRU, FIFO, FIXED, ROUNDROBIN};

SC_MODULE(out4)
{
    sc_fifo<Packet> f[NB_FIFO];
    sc_in<bool> clk;
    sc_in<int> arbType;
    sc_out<Packet> out[NB_FIFO];

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
    char chooseRand()
    {
        return lfsr >> 30;
    }

    void updateRand(char numFifo)
    {
        int retro = ((lfsr>>1) + (lfsr>>6) + (lfsr>>16))%2;
        lfsr = (lfsr << 1) + retro;
    }

    // LRU Mode

    void initLru()
    {
        for(int i = 0; i < NB_FIFO; ++i)
        {
            LruIndex[i] = i;
        }
    }

    char chooseLru()
    {
        for(int i = 0; i < NB_FIFO; i++)
        {
            if(f[LruIndex[i]].num_available() > 0 )
            {
                return LruIndex[i];
            }
        }

        return -1;
    }

    void updateLru(char numFifo)
    {
        bool found = false;

        for(int i = 0; i < NB_FIFO-1; i++)
        {
            if (found)
            {
                LruIndex[i-1] = LruIndex[i];
            }
            else if (LruIndex[i] == numFifo)
            {
                found = true;
            }
        }

        LruIndex[NB_FIFO-1] = numFifo;
    }

    // FIFO Mode

    void initFifo()
    {
        inFifos = 0;
    }

    int chooseFifo()
    {
        if (inFifos == 0)
        {
            return -1;
        }

        return pushsInFifos[0];
    }

    void updateFifo(unsigned char numFifo)
    {
        bool found = false;

        for (unsigned int i = 0; i < inFifos; ++i)
        {
            if (found)
            {
                pushsInFifos[i-1] = pushsInFifos[i];
            }
            else if (pushsInFifos[i] == numFifo)
            {
                found = true;
            }
        }

        --inFifos;
    }

    void pushedInFifo(char idFifo)
    {
        pushsInFifos[inFifos] = idFifo;
        ++inFifos;
    }

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

    // Fixed Mode

    char chooseFixed()
    {
        for(int i = 0; i < 4; i++)
        {
            if(f[i].num_available() > 0 )
            {
                return i;
            }
        }

        return -1;
    }

    // Round Robin Mode

    void initRoundRobin()
    {
        lastUsed = NB_FIFO - 1;
    }

    char chooseRoundRobin()
    {
        for(int i = 0; i < 4; i++)
        {
            if(f[(lastUsed + 1 + i)%4].num_available() > 0 )
            {
                return (lastUsed + 1 + i)%4;
            }
        }

        return -1;
    }

    void updateRoundRobin(char numFifo)
    {
        lastUsed = numFifo;
    }


    // Core functions

    void process()
    {
        const int type = arbType.read();
        char choice = -1;

        switch(type)
        {
        case RAND :
            choice = chooseRand();
            break;
        case LRU :
            choice = chooseLru();
            break;
        case FIFO :
            choice = chooseFifo();
            break;
        case FIXED :
            choice = chooseFixed();
            break;
        case ROUNDROBIN :
            choice = chooseRoundRobin();
            break;
        default:
            break;
        }

        if (choice == -1)
        {
            return;
        }

        Packet packet = f[choice].read();
        out[packet.address].write(packet);

        update(choice);
    }

    void update(char numFifo)
    {
        updateRand(numFifo);
        updateLru(numFifo);
        updateFifo(numFifo);
        updateRoundRobin(numFifo);
    }

    SC_CTOR(out4)
    {
        initLru();
        initFifo();
        initRoundRobin();

        SC_METHOD(process);
        sensitive << clk.pos();

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
