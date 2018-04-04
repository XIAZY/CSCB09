#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int port = 3000;
int listenfd;

#define MAXNAME 80  /* maximum permitted name size, not including \0 */

#define NPITS 6  /* number of pits on a side, not including the end pit */

struct player {
    int fd;
    int pits[NPITS+1];
    other stuff undoubtedly needed here
    struct player *next;
} *playerlist = NULL;

extern void parseargs(int argc, char **argv);
extern void makelistener();
extern int compute_average_pebbles();
extern int game_is_over();  /* boolean */
extern void broadcast(char *s);


int main(int argc, char **argv)
{
    struct player *p;
    char msg[MAXNAME + 50];

    parseargs(argc, argv);
    makelistener();

    while (!game_is_over()) {

    }

    broadcast("Game over!\r\n");
    printf("Game over!\n");
    for (p = playerlist; p; p = p->next) {
	int points, i;
	for (points = i = 0; i <= NPITS; i++)
	    points += p->pits[i];
	printf("%s has %d points\r\n", p->name, points);
	snprintf(msg, sizeof msg, "%s has %d points\r\n", p->name, points);
	broadcast(msg);
    }

    return(0);
}


void parseargs(int argc, char **argv)
{
    int c, status = 0;
    while ((c = getopt(argc, argv, "p:")) != EOF) {
	switch (c) {
	case 'p':
	    port = atoi(optarg);
	    break;
	default:
	    status++;
	}
    }
    if (status || optind != argc) {
	fprintf(stderr, "usage: %s [-p port]\n", argv[0]);
	exit(1);
    }
}


void makelistener()
{
    struct sockaddr_in r;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	perror("socket");
	exit(1);
    }

    memset(&r, '\0', sizeof r);
    r.sin_family = AF_INET;
    r.sin_addr.s_addr = INADDR_ANY;
    r.sin_port = htons(port);
    if (bind(listenfd, (struct sockaddr *)&r, sizeof r)) {
	perror("bind");
	exit(1);
    };

    if (listen(listenfd, 5)) {
	perror("listen");
	exit(1);
    }
}




int compute_average_pebbles()  /* call this BEFORE linking the new player in to the list */
{
    struct player *p;
    int i;

    if (playerlist == NULL)
	return(4);

    int nplayers = 0, npebbles = 0;
    for (p = playerlist; p; p = p->next) {
	nplayers++;
	for (i = 0; i < NPITS; i++)
	    npebbles += p->pits[i];
    }
    return((npebbles - 1) / nplayers / NPITS + 1);  /* round up */
}


int game_is_over() /* boolean */
{
    struct player *p;
    int i;
    if (!playerlist)
	return(0);  /* we haven't even started yet! */
    for (p = playerlist; p; p = p->next) {
	int is_all_empty = 1;
	for (i = 0; i < NPITS; i++)
	    if (p->pits[i])
		is_all_empty = 0;
	if (is_all_empty)
	    return(1);
    }
    return(0);
}
