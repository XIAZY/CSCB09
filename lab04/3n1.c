#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int n;

    if (argc != 2 || (n = atoi(argv[1])) <= 0) {
        fprintf(stderr, "usage: %s n\nn must be a positive integer\n", argv[0]);
        return(1);
    }
}