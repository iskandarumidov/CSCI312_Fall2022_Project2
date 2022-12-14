#include "server.h"

struct record records[MAX_RECORDS];
int len = 0;

// Helper function to concatenate two strings
char* concat(const char *s1, const char *s2){
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

// Saves output that's sent over pipe to an array
char* save_res_to_arr(int print_until, struct record records_to_print[]){
  char *res = malloc(BUFFER_SIZE);      // Allocate space for resulting long array
  int count = 0;
  
  for (int i=0; i < print_until; i++) {
    char char_i[10];                    // Need to convert ints and floats to chars
    char char_odometer[10];
    char char_gallons[10];
    sprintf(char_i, "%d", i);
    sprintf(char_odometer, "%d", records_to_print[i].odometer);
    sprintf(char_gallons, "%f", records_to_print[i].gallons);

    // Use concat helper to generate string to be sent over pipe
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

// Write to a file descriptor using write() syscall
void write_with_syscall(int fd, char *str, int len){
  int err = write(fd, str, len);
  if (err < 0) {
    printf("write failed, errno = %d\n", errno);
    exit(EXIT_FAILURE);
  }
}

// Read records from a file
void read_records(char *str, int flags){
  int n, fd;
  char buf[BUFFER_SIZE];

  // Get file descriptor by opening file
  fd = open(str, flags);
  if (fd < 0) {
    printf("open failed, errno = %d\n", errno);
    exit(EXIT_FAILURE);
  }

  // Read file from that file descriptor
  n = read(fd, buf, BUFFER_SIZE);
  if (n < 0) {
    printf("read failed, errno = %d\n", errno);
    exit(EXIT_FAILURE);
  }
  // Null terminator
  buf[n] = '\0';

  // Use whitespace or \n as tokens to split up input
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

// Extracts record ID from commands like mpg,123 or list,123
char *getId(char *inp) {
  char *res = malloc(strlen(inp));
  res = strtok(inp,",");
  res = strtok (NULL, ",");
  return res;
}

// Compares two record structs
int compare (const void *a, const void *b){
  record *recordA = (record *)a;
  record *recordB = (record *)b;
  return (recordA->odometer - recordB->odometer);
}

int main(int argc, char *argv[]) {
  read_records(DB_FILE, O_RDONLY);
  char *initialRead = save_res_to_arr(len, records);
  write_with_syscall(STDERR, initialRead, strlen(initialRead)+1);

  // If 2 file descriptors not present, Server was not called from Interface
  // Just gasData content will be printed
  if(argc == 3){      
    while(1){      // Start listening for commands from Interface
      int n;
      char buf[BUFFER_SIZE];
      n = read(atoi(argv[1]), &buf, BUFFER_SIZE);
      if (n < 0) {
        printf("pipe open failed, errno = %d\n", errno);
        exit(EXIT_FAILURE);
      }

      buf[n] = '\0';
      // Determine type of command
      if(!strncmp(buf, MPG, strlen(MPG))){
        char *res = getId(buf);
        // int gal = 0.0;
        // float miles = 0.0;
        float min_odometer = INT_MAX;
        float max_odometer = INT_MIN;
        float sum_gallons = 0.0;

        for (int i=0; i < len; i++) {      // Calculage MPG
          if (!strncmp(res, records[i].id, strlen(res)) && (strlen(res) == strlen(records[i].id))){
            if ((float) records[i].odometer < min_odometer){
              min_odometer = (float) records[i].odometer;
            }
            if ((float) records[i].odometer > max_odometer){
              max_odometer = (float) records[i].odometer;
            }
            sum_gallons = sum_gallons + records[i].gallons;
          }
        }
        char result[100];    // Save float result to be returned
        sprintf(result, "%f", (max_odometer - min_odometer) / sum_gallons);
        
        if (sum_gallons == 0.0 | min_odometer == INT_MAX | max_odometer == INT_MIN){    // If 0 in numerator or denominator, just return 0
          write_with_syscall(atoi(argv[2]), "0", strlen("0")+1);
        }else{                           // Otherwise, return actual result
          write_with_syscall(atoi(argv[2]), result, strlen(result)+1);  
        }
        
      } else if (!strncmp(buf, LIST, strlen(LIST))){  // If command is LIST
        struct record filtered[MAX_RECORDS];          // Only matching IDs go here
        int filtered_len= 0;
        char *res = getId(buf);
        
        for (int i=0; i < len; i++) {
          if (!strncmp(res, records[i].id, strlen(res)) && (strlen(res) == strlen(records[i].id))){  // If ID exactly the same
            strcpy(filtered[filtered_len].id, records[i].id);
            filtered[filtered_len].gallons = records[i].gallons;
            filtered[filtered_len].odometer = records[i].odometer;
            filtered_len++;
          }
        }
        // qsort sorts filtered records using compare() function
        qsort(filtered, filtered_len, sizeof(record), compare);
        
        // First get result in res array, then write to parent
        char *concatenated = save_res_to_arr(filtered_len, filtered);
        write_with_syscall(atoi(argv[2]), concatenated, strlen(concatenated)+1);
        
        sleep(1);  // Need to wait to clear up the STDERR stream
      } else if (!strncmp(buf, EXIT, strlen(EXIT))){
        write_with_syscall(atoi(argv[2]), "CHILD EXITING...", strlen("CHILD EXITING...")+1);
        exit(EXIT_SUCCESS);
      } else{
        write_with_syscall(atoi(argv[2]), "UNKNOWN", strlen("UNKNOWN")+1);
      }
    }
  }
}
