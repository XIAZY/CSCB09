#include <stdio.h>
#include <stdlib.h>

struct item {
    int key;
    int data;
    struct item *next;
};

struct item *head = NULL;

int main()
{
    extern void insert(int key, int data), delete(int key), printall();
    extern int search(int key);

    insert(38, 3);
    insert(20, 2);
    insert(5, 0);
    insert(22, 6);
    insert(46, 18);

    printf("With all five items:\n");
    printall();

/*

    printf("After delete(22):\n");
    delete(22);
    printall();
    printf("After delete(5):\n");
    delete(5);
    printall();
    printf("delete(30) shouldn't do anything:\n");
    delete(30);
    printall();

*/

    return(0);
}


void insert(int key, int data)
{
    struct item *new_node = malloc(sizeof(struct item));
    if (new_node == NULL) {
        perror("malloc");
        exit(1);
    }

    new_node->key=key;
    new_node->data=data;
    new_node->next=NULL;

    if (head == NULL) {
        head=new_node;
        return;
    }

    struct item *node = head;

    if (node->key>key) {
        new_node->next=node;
        head=new_node;
        return;
    }

    while (node->next != NULL) {
        struct item *next=node->next;
        if (next->key > key) {
            node->next = new_node;
            new_node->next=next;
            return;
        } else {
            node=next;
        }
    }
    node->next=new_node;
    return;
}


void delete(int key)
{
}


int search(int key) /* returns -1 if not found */
{
    struct item *p;

    for (p = head; p && p->key < key; p = p->next)
        ;
    if (p && p->key == key)
        return(p->data);
    else
        return(-1);
}


void printall()
{
    struct item *p;
    for (p = head; p; p = p->next)
        printf("%d: %d\n", p->key, p->data);
    printf("[end]\n");
}
