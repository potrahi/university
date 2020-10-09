#include <stdio.h>

int main(int arg, char **args)
{
    int value = 0;
    while (scanf("%d", &value) == 1)
    {
        printf("%d ", value);
        int result = 0;
        int elone_mask = 0b11111111;
        int shift = 0;
        int bytes[4] = {};
        int i = 0;
        for(i; i < 4; i++)
        {
            bytes[i] = (value >> shift) & elone_mask;
            shift += 8;
        }
        int j = 0;
        for(j; j < i; j++)
        {
            shift -= 8; 
            result |= bytes[j] << shift;
        }
        printf("converts to %d\n", result);
    }
    return 0;
}
