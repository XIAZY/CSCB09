#include <stdio.h>

int main(int argc, char **argv)
{
    FILE *fp;
    char c;

    if (argc < 2)
	printf("put a file name");

    fp = fopen(argv[1], "r+");
    while ((c = getc(fp)) > 0)
	putchar(c);
    fclose(fp);

    return(1);
}