#include <stdio.h>

int main(int argc, char **argv)
{
    FILE *fp;
    char c;

    if (argc != 2) {
	    fprintf(stdout, "usage: %s filename\n", argv[0]);
        return 1;
    }

    fp = fopen(argv[1], "r+");
    while ((c = getc(fp)) > 0)
	putchar(c);
    fclose(fp);

    return(1);
}