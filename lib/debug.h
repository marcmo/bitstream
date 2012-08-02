#include <stdio.h>

void showbits(int a, int width = 8)
{
    printf("0b");
    for( int i = width - 1 ; i >= 0 ; i--)
    {
        int mask = 1 << i;
        int k = a & mask;
        if( k == 0)
            printf("0");
        else
            printf("1");
    }
}
