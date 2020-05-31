
#include <Fibonacci/Fib.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if (2 < argc)
    {
        fprintf(stderr, "Usage: %s <rank>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);

    Fib f(0);
    for (int i = 0; i < n; ++i)
    {
        f.next();
        printf("fib(%02d) = %d\n", f.getItem(), f.get());
    }

    return 0;
}