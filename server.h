#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define STDOUT 1
#define STDERR 2
#define DB_FILE "./gasData"
#define MAX_RECORDS 1000
#define BUFFER_SIZE 40000


typedef struct record {
  char id[9];
  int odometer;
  float gallons;
} record;