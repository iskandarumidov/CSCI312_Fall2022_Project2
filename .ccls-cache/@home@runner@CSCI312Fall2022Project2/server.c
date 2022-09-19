#include "server.h"

struct record records[MAX_RECORDS];
int len = 0;

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char* save_res_to_arr(int print_until, struct record records_to_print[]){
  //char res[BUFFER_SIZE];
  char *res = malloc(BUFFER_SIZE);
  int count = 0;
  

  for (int i=0; i < print_until; i++) {
    char char_i[10];
    char char_odometer[10];
    char char_gallons[10];
    sprintf(char_i, "%d", i);
    sprintf(char_odometer, "%d", records_to_print[i].odometer);
    sprintf(char_gallons, "%f", records_to_print[i].gallons);
    //res[count] 
    //sprintf(char *restrict __s, const char *restrict __format, ...)
    //sprintf(Buffer,"Hello World");
    //sprintf(Buffer + strlen(Buffer),"Good Morning");
    //sprintf(Buffer + strlen(Buffer),"Good Afternoon");


    //     write_with_syscall(fd, chari, strlen(chari)+1);
    // write_with_syscall(fd, ": id = ", 8);
    // write_with_syscall(fd, records_to_print[i].id, strlen(records_to_print[i].id)+1);
    // write_with_syscall(fd, ", odometer = ", 14);
    // write_with_syscall(fd, charodometer, strlen(charodometer)+1);
    // write_with_syscall(fd, ", gallons = ", 13);
    // write_with_syscall(fd, chargallons, strlen(chargallons)+1);
    // write_with_syscall(fd, "\n", 1);

    

    res = concat(res, "element = ");
    res = concat(res, char_i);
    res = concat(res, ": id = ");
    res = concat(res, records_to_print[i].id);
    res = concat(res, ", odometer = ");
    res = concat(res, char_odometer);
    res = concat(res, ", gallons = ");
    res = concat(res, char_gallons);
    res = concat(res, "\n");
    

    //sprintf(charodometer, "%d", records_to_print[i].odometer);
    // res = concat(res, charodometer);

    // res = concat(res, "\n");
    /*sprintf(res, "element = ");
    sprintf(res + strlen(res), records_to_print[i].id);

    char charodometer[10];
    sprintf(charodometer, "%d", records_to_print[i].odometer);
    sprintf(res + strlen(res), charodometer);*/
    
    
    
    //sprintf(res + strlen(res), "hello");
    /*
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
    write_with_syscall(fd, "\n", 1);*/
    
    
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


/*
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
}*/

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
  //printf("ARGC: %d\n", argc);
  //printf("ARGV[0]: %s\n", argv[0]);
  read_records(DB_FILE, O_RDONLY);
  //print_records();
  print_records(STDERR, len, records);
  if(argc == 3){      // If 2 file descriptors not present, the Server was not called from Interface but rather as a standalone program. Just print gasData contents
    while(1){

    //printf("ARGV[1]: %s\n", argv[1]);
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
      //write_with_syscall(atoi(argv[2]), "IN LIST", strlen("IN LIST")+1);
      
      struct record filtered[MAX_RECORDS];
      int filtered_len= 0;
      char *res = getId(buf);
      //write_with_syscall(atoi(argv[2]), res, strlen(res)+1);
      
      for (int i=0; i < len; i++) {
        
        if (!strncmp(res, records[i].id, strlen(res)) && (strlen(res) == strlen(records[i].id))){
          strcpy(filtered[filtered_len].id, records[i].id);
          filtered[filtered_len].gallons = records[i].gallons;
          filtered[filtered_len].odometer = records[i].odometer;
          filtered_len++;
          //printf("EQ: %d", !strncmp(res, records[2].id, strlen(res)));
          
          //write_with_syscall(atoi(argv[2]), "DONE", strlen("DONE")+1);
        }
        
        
        //write_with_syscall(atoi(argv[2]), result, strlen(result)+1);
      }
      
      char *concatenated = save_res_to_arr(filtered_len, filtered);
      write_with_syscall(atoi(argv[2]), concatenated, strlen(concatenated)+1);
      //print_records(atoi(argv[2]), len, records);


      
      /*
      for (int i=0; i < filtered_len; i++) {
        //printf("RES: %s %d %f\n", filtered[i].id, filtered[i].odometer, filtered[i].gallons);
        write_with_syscall(atoi(argv[2]), filtered[i].id, strlen(filtered[i].id)+1);
        
        
      }*/
      
      
      //write_with_syscall(atoi(argv[2]), "\0", 1);
      
      
      //print_records(atoi(argv[2]), filtered_len, filtered);
      //write_with_syscall(atoi(argv[2]), filtered[0].id, strlen(filtered[0].id)+1);
      //write_with_syscall(atoi(argv[2]), filtered[1].id, strlen(filtered[1].id)+1);
      //print_records(atoi(argv[2]), len, records);
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

