#include "server.h"

struct record records[MAX_RECORDS];
int len = 0;

void write_with_syscall(int fd, char *str, int len){
  int err = write(fd, str, len);
  if (err < 0) {
    printf("write failed, errno = %d\n", errno);
    exit(2);
  }
}

void read_records(char *str, int flags){
  int n, fd;
  char buf[BUFFER_SIZE];
  
  fd = open(str, flags);
  if (fd < 0) {
    printf("open failed, errno = %d\n", errno);
    exit(1);
  }
  
  n = read(fd, buf, BUFFER_SIZE);
  if (n < 0) {
    printf("read failed, errno = %d\n", errno);
    exit(1);
  }
  buf[n] = '\0';

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

  close(fd);
}

void print_records(){
  for (int i=0; i < len;  i++) {
    write_with_syscall(STDOUT, "element = ", 11);
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
}

char *getId(char *inp) {
  char *res = malloc(strlen(inp));
  res = strtok(inp,",");
  res = strtok (NULL, ",");
  return res;
}

int main(int argc, char *argv[]) {
  //printf("ARGC: %d\n", argc);
  //printf("ARGV[0]: %s\n", argv[0]);
  read_records(DB_FILE, O_RDONLY);
  print_records();
  if(argc == 3){
    while(1){

    //printf("ARGV[1]: %s\n", argv[1]);
    int n;
    char buf[BUFFER_SIZE];
    n = read(atoi(argv[1]), &buf, BUFFER_SIZE);
    if (n < 0) {
      printf("pipe open failed, errno = %d\n", errno);
      exit(1);
    }

    buf[n] = '\0';
    //printf("CHILD FROM PIPE: %s\n", buf);
    if(!strncmp(buf, "mpg", strlen("mpg"))){
      //printf("CHILD: got MPG\n");
      char *res = getId(buf);
      int gal = 0.0;
      float miles = 0.0;
      for (int i=0; i < len; i++) {
        if (!strncmp(res, records[i].id, strlen(res))){
          gal = gal + records[i].gallons;
          miles = miles + (float) records[i].odometer;
        }
      }
      char result[100];
      sprintf(result, "%f", miles / gal);
      
      if (gal == 0.0 | miles == 0){
        write_with_syscall(atoi(argv[2]), "0", strlen("0")+1);
      }else{
        write_with_syscall(atoi(argv[2]), result, strlen(result)+1);  
      }
      
      
      //write_with_syscall(atoi(argv[2]), "GOT MPG", strlen("GOT MPG")+1);
      
      
      /*int err = write(atoi(argv[2]), "hello from child", strlen("hello from child")+1);
      if (err == -1 ) {
        printf ("Error on write to pipe: %d\n", errno);
        exit (1);
      }*/
      
    } else if (!strncmp(buf, "list", strlen("list"))){
      struct record filtered[MAX_RECORDS];
      int filtered_len= 0;

      for (int i=0; i < len; i++) {
        if (!strncmp(res, records[i].id, strlen(res))){
            gal = gal + records[i].gallons;
            miles = miles + (float) records[i].odometer;
        }
      }
      
    } else if (!strncmp(buf, "exit", strlen("exit"))){
      exit(0);
    } else{
      printf("UNKNOWN\n");
    }
    
    
  
    
  }
  }
}

