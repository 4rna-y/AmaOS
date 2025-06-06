#include "Math.h"

float floor_f(float x)
{
    int i = (int)x;
    if (x < 0.0f && x != (float)i) return (float)(i - 1);
    return (float)i;
}

