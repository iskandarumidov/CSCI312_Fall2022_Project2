#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


#define MSG "message to be written to file.\n"

typedef struct Record {
  char id[9];
  int odometer;
  float gallons;
} Record;

int main(void) {
  int n, fd, err;
  char buf[50];
  //fd = open ("./myFile", O_CREAT + O_RDWR, 0755);
  fd = open("./gasData", O_RDONLY, 0755);
  
  if (fd < 0) {
    printf("open failed, errno = %d\n", errno);
    exit(1);
  }


  /* Write information to the file. */
  //err = write (fd, MSG, strlen(MSG)+1);
  n = read(fd, buf, 50);
  printf("read bytes: %d\n", n);
  buf[n] = '\0';
  //write(1, buf, 50);
  //write(1, buf, 1);
  //int i;
  //printf("%s", buf);
  struct Record records[1000];
  int len = 0;
  //Record arrStruct;
  //arrStruct.gallons = 20.0;
  //arrStruct.odometer = 76;
  //strcpy (arrStruct.id, "ISK\0");
  //arrStruct.id 
  //printf("arrStruct: gall %f\n", arrStruct.gallons);
  //printf("arrStruct: odo %d\n", arrStruct.odometer);
  //printf("arrStruct: id %s\n", arrStruct.id);

  
  Record testRec;
  testRec.gallons = 10.0;
  testRec.odometer = 83;
  char name[9] = "Joe\0";
  //strcpy (testRec.id, name);
  //testRec.id = name;
  
  //printf("rec %f", testRec.gallons);
  //printf("rec %s", testRec.id);
  char* token = strtok(buf, " \n");
  while (token != NULL) {
    strcpy (records[len].id, token);
    
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
    
    write(1, chari, strlen(chari)+1);
    write(1, ": id = ", 8);
    write(1, records[i].id, strlen(records[i].id)+1);
    write(1, ", odometer = ", 14);
    write(1, charodometer, strlen(charodometer)+1);
    write(1, ", gallons = ", 13);
    write(1, chargallons, strlen(chargallons)+1);
    write(1, "\n", 1);
  }
  
  if (err < 0) {
    printf("write failed, errno = %d\n", errno);
    exit(2);
  }
  //printf ("%d characters written to file\n", err);
  close (fd);
  return 0;
}

//TODO - err check like file not there, etc - err, bytes written
// negative, positive, etc
//TODO - makefile
// TODO - proper gasData file with edge cases