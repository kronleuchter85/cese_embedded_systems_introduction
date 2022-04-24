#include <stdlib.h>
#include <time.h> 

//======================== Repository ================================

static float available_tariffs[10];

void repository_initialize()
{
    srand(time(NULL));

    int i;
    for (i = 0; i < 10; i++)
    {
        available_tariffs[i] = 1 + rand();
    }
}

float *repository_available_tariffs_get()
{
    return available_tariffs;
}