#include "arbiter/arbiter.h"

// Rand Mode
char arbiter::chooseRand()
{
    return (lfsr >> 15)%2 + 2 * ((lfsr >> 23)%2);
}

void arbiter::updateRand(char numFifo)
{
    int retro = ((lfsr>>1) + (lfsr>>6) + (lfsr>>16))%2;
    lfsr = (lfsr << 1) + retro;
}

// LRU Mode

void arbiter::initLru()
{
    for(int i = 0; i < NB_FIFO; ++i)
    {
        LruIndex[i] = i;
    }
}

char arbiter::chooseLru()
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

void arbiter::updateLru(char numFifo)
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

void arbiter::initFifo()
{
    inFifos = 0;
}

int arbiter::chooseFifo()
{
    if (inFifos == 0)
    {
        return -1;
    }

    return pushsInFifos[0];
}

void arbiter::updateFifo(unsigned char numFifo)
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

void arbiter::pushedInFifo(char idFifo)
{
    pushsInFifos[inFifos] = idFifo;
    ++inFifos;
}

// Fixed Mode

char arbiter::chooseFixed()
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

void arbiter::initRoundRobin()
{
    lastUsed = NB_FIFO - 1;
}

char arbiter::chooseRoundRobin()
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

void arbiter::updateRoundRobin(char numFifo)
{
    lastUsed = numFifo;
}


// Core functions

void arbiter::process()
{
    while (true)
    {
        wait();

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

        choice_out.write((int)choice);

        if (choice == -1)
        {
            continue;
        }

        packet pack= f[choice].read();
        out.write(pack);

        update(choice);
    }
}
