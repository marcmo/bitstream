#include <stdio.h>
#include "debug.h"

void showbits(int a, int width)
{
    for( int i = width - 1 ; i >= 0 ; i--)
    {
        a & (1 << i) ? printf("1") : printf("0");
    }
}
