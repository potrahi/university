#include <stdio.h>

#define ARR_SIZE 1000000

static unsigned int result[ARR_SIZE];

void read(int numbers)
{
    int n = 0;
    for (n; n < numbers; n++)
    {
        int a = 0, b = 0, c = 0;
        scanf("%d %d", &a, &b);
        c = a + b;
        result[n] = c;
    }
}

void calc(int numbers)
{
    for(--numbers;numbers != 0; numbers--)
    {
        if(result[numbers] >= 10)
        {
            result[numbers-1] += result[numbers]/10;
            result[numbers] %= 10;
        }
    }
}

void print(int numbers)
{
    int n = 0;
    for(n;n < numbers;n++)
        printf("%d", result[n]);
    printf("\n");
}

int main()
{
    int b = 0, blocks = 0;
    scanf("%d", &blocks);
    for (b; b < blocks; b++)
    {
        int numbers = 0;
        scanf("%d", &numbers);
        read(numbers);
        calc(numbers);
        print(numbers);
        if(b < blocks - 1) printf("\n");
    }
    return 0;
}
