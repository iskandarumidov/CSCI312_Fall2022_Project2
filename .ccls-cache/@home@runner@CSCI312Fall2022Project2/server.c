#include "server.h"

struct record records[MAX_RECORDS];
int len = 0;

char* concat(const char *s1, const char *s2){
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char* save_res_to_arr(int print_until, struct record records_to_print[]){
  char *res = malloc(BUFFER_SIZE);
  int count = 0;
  
  for (int i=0; i < print_until; i++) {
    char char_i[10];
    char char_odometer[10];
    char char_gallons[10];
    sprintf(char_i, "%d", i);
    sprintf(char_odometer, "%d", records_to_print[i].odometer);
    sprintf(char_gallons, "%f", records_to_print[i].gallons);

    res = concat(res, "element = ");
    res = concat(res, char_i);
    res = concat(res, ": id = ");
    res = concat(res, records_to_print[i].id);
    res = concat(res, ", odometer = ");
    res = concat(res, char_odometer);
    res = concat(res, ", gallons = ");
    res = concat(res, char_gallons);
    res = concat(res, "\n");
    
  }
  return res;
}

void write_with_syscall(int fd, char *str, int len){
  int err = write(fd, str, len);
  if (err < 0) {
    printf("write failed, errno = %d\n", errno);
    exit(EXIT_FAILURE);
  }
}

void read_records(char *str, int flags){
  int n, fd;
  char buf[BUFFER_SIZE];
  
  fd = open(str, flags);
  if (fd < 0) {
    printf("open failed, errno = %d\n", errno);
    exit(EXIT_FAILURE);
  }
  
  n = read(fd, buf, BUFFER_SIZE);
  if (n < 0) {
    printf("read failed, errno = %d\n", errno);
    exit(EXIT_FAILURE);
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

void print_records(int fd, int print_until, struct record records_to_print[]){
  for (int i=0; i < print_until; i++) {
    write_with_syscall(fd, "element = ", 11);
    char chari[10];
    char charodometer[10];
    char chargallons[10];
    
    sprintf(chari, "%d", i);
    sprintf(charodometer, "%d", records_to_print[i].odometer);
    sprintf(chargallons, "%f", records_to_print[i].gallons);
    
    write_with_syscall(fd, chari, strlen(chari)+1);
    write_with_syscall(fd, ": id = ", 8);
    write_with_syscall(fd, records_to_print[i].id, strlen(records_to_print[i].id)+1);
    write_with_syscall(fd, ", odometer = ", 14);
    write_with_syscall(fd, charodometer, strlen(charodometer)+1);
    write_with_syscall(fd, ", gallons = ", 13);
    write_with_syscall(fd, chargallons, strlen(chargallons)+1);
    write_with_syscall(fd, "\n", 1);
    
    
  }
  sleep(1);
}




char *getId(char *inp) {
  char *res = malloc(strlen(inp));
  res = strtok(inp,",");
  res = strtok (NULL, ",");
  return res;
}

int main(int argc, char *argv[]) {
  read_records(DB_FILE, O_RDONLY);
  print_records(STDERR, len, records);
  if(argc == 3){      // If 2 file descriptors not present, the Server was not called from Interface but rather as a standalone program. Just print gasData contents
    while(1){
    int n;
    char buf[BUFFER_SIZE];
    n = read(atoi(argv[1]), &buf, BUFFER_SIZE);
    if (n < 0) {
      printf("pipe open failed, errno = %d\n", errno);
      exit(EXIT_FAILURE);
    }

    buf[n] = '\0';
    if(!strncmp(buf, MPG, strlen(MPG))){
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
      
      if (gal == 0.0 | miles == 0){    // If 0 in numerator or denominator, just return 0
        write_with_syscall(atoi(argv[2]), "0", strlen("0")+1);
      }else{                           // Otherwise, return actual result
        write_with_syscall(atoi(argv[2]), result, strlen(result)+1);  
      }
      
    } else if (!strncmp(buf, LIST, strlen(LIST))){
      struct record filtered[MAX_RECORDS];
      int filtered_len= 0;
      char *res = getId(buf);
      
      for (int i=0; i < len; i++) {
        
        if (!strncmp(res, records[i].id, strlen(res)) && (strlen(res) == strlen(records[i].id))){
          strcpy(filtered[filtered_len].id, records[i].id);
          filtered[filtered_len].gallons = records[i].gallons;
          filtered[filtered_len].odometer = records[i].odometer;
          filtered_len++;
        }
      }
      
      char *concatenated = save_res_to_arr(filtered_len, filtered);
      write_with_syscall(atoi(argv[2]), concatenated, strlen(concatenated)+1);
      
      sleep(1);
    } else if (!strncmp(buf, EXIT, strlen(EXIT))){
      write_with_syscall(atoi(argv[2]), "CHILD EXITING...", strlen("CHILD EXITING...")+1);
      exit(EXIT_SUCCESS);
    } else{
      write_with_syscall(atoi(argv[2]), "UNKNOWN", strlen("UNKNOWN")+1);
    }
    
    
  
    
  }
  }
}

