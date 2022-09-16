#include "main.h"

void write_with_syscall(int fd, char *str, int len){
  int err = write(fd, str, len);
  if (err < 0) {
    printf("write failed, errno = %d\n", errno);
    exit(2);
  }
}

int main(void) {
  int n, fd;
  char buf[50];
  fd = open("./gasData", O_RDONLY);
  
  if (fd < 0) {
    printf("open failed, errno = %d\n", errno);
    exit(1);
  }

  n = read(fd, buf, 50);
  buf[n] = '\0';
  
  struct record records[1000];
  int len = 0;
  
  char* token = strtok(buf, " \n");
  while (token != NULL) {
    strcpy(records[len].id, token);
    
    token = strtok(NULL, " \n");
    records[len].odometer = atoi(token);
  
    token = strtok(NULL, " \n");
    records[len].gallons = atof(token);

    token = strtok(NULL, " \n");
    len++;
  }

  for (int i=0; i < len;  i++) {
    write(1, "element = ", 11);
    char chari[10];
    char charodometer[10];
    char chargallons[10];
    
    sprintf(chari, "%d", i);
    sprintf(charodometer, "%d", records[i].odometer);
    sprintf(chargallons, "%f", records[i].gallons);
    
    write_with_syscall(STDOUT, chari, strlen(chari)+1);
    write_with_syscall(STDOUT, ": id = ", 8);
    write_with_syscall(STDOUT, records[i].id, strlen(records[i].id)+1);
    write_with_syscall(STDOUT, ", odometer = ", 14);
    write_with_syscall(STDOUT, charodometer, strlen(charodometer)+1);
    write_with_syscall(STDOUT, ", gallons = ", 13);
    write_with_syscall(STDOUT, chargallons, strlen(chargallons)+1);
    write_with_syscall(STDOUT, "\n", 1);
  }
  
  close (fd);
  return 0;
}

// TODO - makefile
// TODO - proper gasData file with edge cases
// TODO - headers