#include <stdio.h>

int main()
{
    unsigned int multiplier = 1; // the higher the number the less colors are displayed

    for(int r = 0; r <= 255; r+=multiplier)
    {
        for(int g = 0; g <= 255; g+=multiplier)
        {
            for(int b = 0; b <= 255; b+=multiplier)
            {
                printf("\e[38;2;%d;%d;%dm Hello, World!", r, g, b);
            }
        }
        
    }
    
    return 0;
}
