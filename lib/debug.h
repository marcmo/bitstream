#include <stdio.h>

void showbits(int a, int width = 8)
{
    for( int i = width - 1 ; i >= 0 ; i--)
    {
        a & (1 << i) ? printf("1") : printf("0");
    }
}
