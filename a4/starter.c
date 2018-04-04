#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int port = 3000;
int listenfd;

#define MAXNAME 80 /* maximum permitted name size, not including \0 */

#define NPITS 6 /* number of pits on a side, not including the end pit */

struct player {
  int fd;
  int pits[NPITS + 1];
  char *name;
  // other stuff undoubtedly needed here
  struct player *next;
} *playerlist = NULL;

extern void parseargs(int argc, char **argv);
extern void makelistener();
extern int compute_average_pebbles();
extern int game_is_over(); /* boolean */
extern void broadcast(char *s);
extern void append_player(struct player *);
extern struct player *get_player(int);
extern int get_fd(struct player*, fd_set*);
extern void delete_player(int);
extern char* get_input(int);

int main(int argc, char **argv) {
  // struct player *p;
  // char msg[MAXNAME + 50];

  parseargs(argc, argv);
  makelistener();

  // while (!game_is_over()) {

  // }

  // broadcast("Game over!\r\n");
  // printf("Game over!\n");
  // for (p = playerlist; p; p = p->next) {
  // int points, i;
  // for (points = i = 0; i <= NPITS; i++)
  //     points += p->pits[i];
  // printf("%s has %d points\r\n", p->name, points);
  // snprintf(msg, sizeof msg, "%s has %d points\r\n", p->name, points);
  // broadcast(msg);

  // }
  int clientfd;
  struct sockaddr_in q;
  fd_set fds;
  int len = sizeof q;
  //   if ((listenfd = accept(listenfd, (struct sockaddr *)&q, &len)) < 0) {
  //     perror("accept");
  //     return (1);
  //   }
  char buf[1024];
  while (strcmp(buf, "STOP") != 0) {
    FD_ZERO(&fds);
    FD_SET(listenfd, &fds);
    int maxfd = listenfd;
    struct player *iter_player = playerlist;
    while (iter_player != NULL) {
      int iter_fd = iter_player->fd;
      FD_SET(iter_fd, &fds);
      if (iter_fd > maxfd) {
        maxfd = iter_fd;
      }
      iter_player = iter_player->next;
    }
    switch (select(maxfd + 1, &fds, NULL, NULL, NULL)) {
      case 0:
        printf("timeout happened\n");
        break;
      case -1:
        perror("select");
        break;
      default:
        if (FD_ISSET(listenfd, &fds)) {
          printf(
              "a new client is connecting\n"); /* so we should accept(), etc */
          if ((clientfd = accept(listenfd, (struct sockaddr *)&q, &len)) < 0) {
            perror("accept new");
            return 1;
          }
          append_player(get_player(clientfd));
          char welcome[100];
          snprintf(welcome, 100, "your fd is %d\n", clientfd);
          write(clientfd, welcome, strlen(welcome));

        } else {
        //   old connection
          clientfd=get_fd(playerlist, &fds);
          char* in = get_input(clientfd);
          write(clientfd, in, strlen(in));
        }
    }
  }
  close(listenfd);
  return (0);
}

int get_fd(struct player* start, fd_set* fds) {
    struct player* iter_player = start;
    while (iter_player != NULL) {
        if (FD_ISSET(iter_player->fd, fds)) {
            return iter_player->fd;
        } else {
            iter_player=iter_player->next;
        }
    }
}
void delete_player(int fd) {
  if (playerlist->fd == fd) {
    playerlist = playerlist->next;
  } else {
    struct player *iter_player = playerlist;
    while (iter_player->next != NULL) {
      if (iter_player->next->fd == fd) {
        iter_player->next = iter_player->next->next;
        break;
      }
    }
  }
}

void append_player(struct player *p) {
  if (playerlist == NULL) {
    playerlist = p;
  } else {
    p->next = playerlist;
    playerlist = p;
  }
}

struct player *get_player(int fd) {
  struct player *new_player = malloc(sizeof(struct player));
  new_player->fd = fd;
  new_player->next = NULL;
  return new_player;
}

// char* get_name(int fd) {
//     char welcome[20] = "Welcome to Mancala. What is your name?\n";
//     write(fd, welcome, strlen(welcome));

// }

char* get_input(int fd) {
    // 1kb buffer
    char* buff=malloc(sizeof(char)*1024);
    int len;
    if ((len=read(fd, buff, 1023)) <0) {
        perror("read");
    } else if (len==0) {
        delete_player(fd);
        close(fd);
    } else {
        buff[len]='\0';
    }
    return buff;
}
void parseargs(int argc, char **argv) {
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

void makelistener() {
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

int compute_average_pebbles() /* call this BEFORE linking the new player in to
                                 the list */
{
  struct player *p;
  int i;

  if (playerlist == NULL) return (4);

  int nplayers = 0, npebbles = 0;
  for (p = playerlist; p; p = p->next) {
    nplayers++;
    for (i = 0; i < NPITS; i++) npebbles += p->pits[i];
  }
  return ((npebbles - 1) / nplayers / NPITS + 1); /* round up */
}

int game_is_over() /* boolean */
{
  struct player *p;
  int i;
  if (!playerlist) return (0); /* we haven't even started yet! */
  for (p = playerlist; p; p = p->next) {
    int is_all_empty = 1;
    for (i = 0; i < NPITS; i++)
      if (p->pits[i]) is_all_empty = 0;
    if (is_all_empty) return (1);
  }
  return (0);
}
