#include <stdio.h>

struct item {
    int key;
    int data;
    struct item *next;
};

struct item *head = NULL;

int main()
{
    extern void filllist();
    extern int search(int key);  /* returns -1 if not found */
    filllist();
    printf("%d\n", search(20));
    printf("%d\n", search(38));
    printf("%d\n", search(138));
    printf("%d\n", search(8));
    printf("%d\n", search(5));
    /* expected output: 2, 3, -1, -1, 0 */
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


int search(int key) /* returns -1 if not found */
{
    struct item *node = head;
    while (node != NULL) {
        int key_found = node->key;
        if (key_found==key) {
            return node->data;
        } else if (key_found>key) {
            return -1;
        } else {
            node=node->next;
        }
    }
    return -1;
}
