#include <stdio.h>

struct item {
    int key;
    int data;
    struct item *next;
};

struct item *head = NULL;

int main()
{
    extern void filllist(), printall();
    filllist();
    printall();
    return(0);
}


void filllist()
{
    static struct item a, b, c, d;
    head = &a;
    a.key = 5;
    a.data = 0;
    a.next = &b;
    b.key = 20;
    b.data = 2;
    b.next = &c;
    c.next = &d;
    c.key = 22;
    c.data = 6;
    d.key = 38;
    d.data = 3;
    d.next = NULL;
}


void printall()
{
    while (head!=NULL) {
        printf("%d: %d\n", head->key, head->data);
        head=head->next;
    }
    printf("[end]\n");
}
