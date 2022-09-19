#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define STDOUT 1
#define STDERR 2
#define DB_FILE "./gasData"
#define MAX_RECORDS 1000
#define BUFFER_SIZE 40000

#define MPG "mpg,"
#define LIST "list,"
#define EXIT "exit"


typedef struct record {
  char id[9];
  int odometer;
  float gallons;
} record;

char* concat(const char *s1, const char *s2);
char* save_res_to_arr(int print_until, struct record records_to_print[]);
void write_with_syscall(int fd, char *str, int len);
void read_records(char *str, int flags);
char *getId(char *inp);
int compare (const void *a, const void *b);
