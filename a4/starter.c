#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <stdbool.h>
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
extern void append_player_to_list(struct player *);
extern struct player *get_player(int, char *);
extern int get_fd(struct player *, fd_set *);
extern void delete_player(int);
extern char *get_input(int, int);
extern char *get_name(int);
extern void add_player(int);
extern void write_output(int, char *);
extern bool is_name_duplicate(char *);
extern char *player_to_string(struct player *);
extern char *game_stat();

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
  bool is_end = false;
  while (!is_end) {
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
          printf("a new client is connecting\n");
          /* so we should accept(), etc */
          if ((clientfd = accept(listenfd, (struct sockaddr *)&q, &len)) < 0) {
            perror("accept new");
            return 1;
          }
          add_player(clientfd);

        } else {
          //   old connection
          clientfd = get_fd(playerlist, &fds);
          char *in = get_input(clientfd, 1024);
          if (in != NULL) {
            write_output(clientfd, in);
          }
        }
    }
  }
  close(listenfd);
  return (0);
}

void broadcast(char *msg) {
  struct player *iter_player = playerlist;
  while (iter_player != NULL) {
    write_output(iter_player->fd, msg);
    iter_player = iter_player->next;
  }
}
int get_fd(struct player *start, fd_set *fds) {
  struct player *iter_player = start;
  while (iter_player != NULL) {
    if (FD_ISSET(iter_player->fd, fds)) {
      return iter_player->fd;
    } else {
      iter_player = iter_player->next;
    }
  }
}
char *get_input_without_newline(char *in) {
  if (in != NULL) {
    char *buffer = malloc((sizeof(char)) * 1024);
    if (buffer != NULL) {
      strcpy(buffer, in);
      char *lineend;
      if ((lineend = strchr(buffer, '\n'))) {
        *lineend = '\0';
      }
      if ((lineend = strchr(buffer, '\r'))) {
        *lineend = '\0';
      }
      return buffer;
    } else {
      perror("malloc");
      exit(1);
    }
  } else {
    return NULL;
  }
}

void write_output(int fd, char *msg) {
  if (write(fd, msg, strlen(msg)) < 0) {
    perror("write");
  }
}
void delete_player(int fd) {
  if (playerlist != NULL) {
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
}

void append_player_to_list(struct player *p) {
  if (playerlist == NULL) {
    playerlist = p;
  } else {
    p->next = playerlist;
    playerlist = p;
  }
}

struct player *get_player(int fd, char *name) {
  struct player *new_player = malloc(sizeof(struct player));
  if (new_player != NULL) {
    new_player->fd = fd;
    new_player->next = NULL;
    new_player->name = name;
    int pebbles = compute_average_pebbles();
    int pits[NPITS + 1] = {pebbles,pebbles,pebbles,pebbles,pebbles,pebbles};
    memcpy(new_player->pits, pits, sizeof(pits));
    return new_player;
  } else {
    perror("malloc");
    exit(1);
  }
}

void add_player(int fd) {
  char *name = get_name(fd);
  if (name != NULL) {
    append_player_to_list(get_player(fd, name));
    char msg[100];
    snprintf(msg, sizeof msg, "%s has joined the game\r\n", name);
    broadcast(msg);
    write_output(fd, game_stat());
  }
}

char *game_stat() {
  char *stat = malloc(sizeof(char) * 1024);

  if (stat != NULL) {
    stat[0] = '\0';
    struct player *iter_player = playerlist;
    while (iter_player != NULL) {
      strcat(stat, player_to_string(iter_player));
      strcat(stat, "\n");
      iter_player = iter_player->next;
    }
  } else {
    perror("malloc");
    exit(1);
  }
  return stat;
}

bool is_name_duplicate(char *name) {
  struct player *iter_player = playerlist;
  while (iter_player != NULL) {
    if (strcmp(name, iter_player->name) == 0) {
      return true;
    } else {
      iter_player = iter_player->next;
    }
  }
  return false;
}

char *get_name(int fd) {
  char welcome[20] = "Welcome to Mancala. ";
  char again[20] = "What is your name?\r\n";
  char taken_msg[100] =
      "Sorry, someone else already has that name.  Please choose another.\r\n";
  write_output(fd, welcome);
  bool is_set = false;
  char *name;
  while (!is_set) {
    write_output(fd, again);
    name = get_input_without_newline(get_input(fd, MAXNAME));
    if (name != NULL) {
      if (strlen(name) == 0) {
        is_set = false;
      } else if (is_name_duplicate(name)) {
        write_output(fd, taken_msg);
        is_set = false;
      } else {
        is_set = true;
      }
    } else {
      return NULL;
      break;
    }
  }
  return name;
}

char *player_to_string(struct player *p) {
  char *result = malloc(sizeof(char) * 1024);
  char buffer[1024];
  int *pits = p->pits;
  snprintf(buffer, sizeof(buffer),
           "%s: [0]%d [1]%d [2]%d [3]%d [4]%d [5]%d [end pit]%d", p->name,
           pits[0], pits[1], pits[2], pits[3], pits[4], pits[5], pits[6]);
  strcpy(result, buffer);
  return result;
}

char *get_input(int fd, int buffersize) {
  // 1kb buffer
  char *buff = malloc(sizeof(char) * buffersize);
  if (buff != NULL) {
    int len;
    if ((len = read(fd, buff, buffersize - 2)) < 0) {
      perror("read");
    } else if (len == 0) {
      close(fd);
      delete_player(fd);
      buff = NULL;
    } else {
      buff[len] = '\0';
    }
    return buff;
  } else {
    perror("malloc");
    exit(1);
  }
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
