
#include <random.h>

void srand(unsigned int seed)
{
    RNG_START = 1;
}

int rand()
{
    RNG_EVENT_VALRDY = 0;
    while (!RNG_EVENT_VALRDY);
    return RNG_VALUE;
}
