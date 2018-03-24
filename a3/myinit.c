#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void print_usage(char*);
char** get_args(char*);
bool is_empty(char*);
int exec(bool, char*);
bool is_run_level(char*, char);

int main(int argc, char** argv) {
  // exec(1, "1");
  int opt;
  bool is_run_level_given = false;
  char run_level;
  // get the args
  while ((opt = getopt(argc, argv, "r:")) != -1) {
    switch (opt) {
      case 'r':
        if (strlen(optarg) != 1) {
          print_usage(argv[0]);
          exit(1);
        } else {
          is_run_level_given = true;
          run_level = optarg[0];
        }
        break;
    }
  }

  if (!is_run_level_given) {
    run_level = '3';
  }

  if (optind != argc - 1) {
    print_usage(argv[0]);
    exit(1);
  }

  char* filename = argv[optind];

  FILE* fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("file open");
    exit(1);
  }

  char line[1024];

  while (fgets(line, sizeof line, fp) != NULL) {
    if (!is_empty(line)) {
      char** args = get_args(line);
      if (args == NULL) {
        fprintf(stderr, "failed to parse line %s\n", line);
      } else {
        if (is_run_level(args[0], run_level)) {
          if (strcmp(args[1], "once") == 0) {
            exec(false, args[2]);
          } else if (strcmp(args[1], "respawn")==0) {
            switch (fork()) {
              case -1:
                perror("fork");
                break;
              case 0:
                exec(true, args[2]);
            }
          } else {
            fprintf(stderr, "illegal action %s\n", args[1]);
          }
        }
      }
    }
  }

  return 0;
}

bool is_run_level(char* tab_level, char run_level) {
  if (is_empty(tab_level)) {
    return true;
  }
  for (int i = 0; i < strlen(tab_level); i++) {
    if (tab_level[i] == run_level) {
      return true;
    }
  }
  return false;
}
void print_usage(char* binary) {
  fprintf(stderr, "usage: %s [-r runlevel] file\n", binary);
}

char** get_args(char* line) {
  static char arg_one[1024];
  static char arg_two[1024];
  static char arg_three[1024];

  static char* args[3];
  args[0] = arg_one;
  args[1] = arg_two;
  args[2] = arg_three;
  int length = strlen(line);
  if (line[length - 1] == '\n') {
    line[length - 1] = '\0';
    length--;
  }
  // const char comment[2] = "#";

  // char * comment_less_line=strtok(line, comment);
  // printf("!!%s\n", comment_less_line);
  // int column_num=0;
  int arg_num = 0;
  int arg_index = 0;

  for (int index = 0; index <= strlen(line); index++) {
    if (line[index] == '#') {
      break;
    } else if (line[index] == ':' && arg_num < 2) {
      args[arg_num][arg_index] = '\0';
      arg_num++;
      arg_index = 0;
    } else {
      args[arg_num][arg_index] = line[index];
      args[arg_num][arg_index + 1] = '\0';
      arg_index++;
    }
  }

  if (arg_num != 2) {
    return NULL;
  } else {
    return args;
  }
}

bool is_empty(char* line) {
  for (int i = 0; i < strlen(line); i++) {
    if (line[i] == '#') {
      break;
    } else if (line[i] != ' ' && line[i] != '\n') {
      return false;
    }
  }
  return true;
}

int exec(bool respawn, char* command) {
  int x = fork();
  if (x == -1) {
    perror("fork");
    return (1);
  } else if (x == 0) {
    /* child */
    execl("/bin/sh", "sh", "-c", command, (char*)NULL);
    perror("/bin/sh");
    return (1);
  } else if (respawn) {
    int status;
    wait(&status);
    exec(1, command);
    return (0);
  }
  return 0;
}