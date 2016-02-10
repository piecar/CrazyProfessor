#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "util.h"

long fib_naive(int n)
{
    // Here we use the macro assert to check for invalid input
    // Notice the use of && followed by a string describing the error
    assert(n>0 && "N must not be 0");

    //This macro is defined in util.h
    //it mimics the arguments of printf, but it only prints when DEBUG is
    //defined. We define DEBUG in the Makefile
    log("fin of %d\n", n);
    if (n == 1 || n == 2)
        return 1;
    return fib_naive(n-1) + fib_naive(n-2);
}

int main(int argc, const char *argv[])
{
    int i;
    for (i = 1; i < 6; i++) {
        printf("fib[%i] = %ld\n",
                i, fib_naive(i));
    }
    return 0;
}
