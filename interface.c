#include "interface.h"

char *getId(char *inp) {
  char *res = malloc(strlen(inp));
  res = strtok(inp,",");
  res = strtok (NULL, ",");
  return res;
}

int main(void)
{
  int toChild[2];
  int toParent[2];
  int err;
  int childPid;
  int status;
  //err = pipe(toChild);
  //if ( err == -1) {
  //  printf ("Error on pipe creation: %d\n", errno);
  //  exit (1);
  //}

  childPid = fork();
  if (childPid == -1) {
    printf ("parent: fork failed, errno = %d\n", errno);
    exit (1);
  } else if ( childPid == 0 ) { // IN CHILD NOW
    err = execl ("./server", "server", (char *)NULL);
    if ( err == -1 ) {
      printf ("parent: execl failed, errno = %d\n", errno);
      exit (2);
    }
    exit(0);
  }
  printf ("parent: child created with pid = %d\n", childPid);

  if ( waitpid(childPid, &status, 0) == -1 ) {
    perror("waitpid failed");
    return EXIT_FAILURE;
  }

  if ( WIFEXITED(status) ) {
    const int es = WEXITSTATUS(status);
    printf("child exit status was %d\n", es);
  }

  printf("PARENT EXITS\n");

  /*
  if (childPid == 0) {
    close (toChild[1]);

  }*/

  /*
  char inp[20];
  while(1){
    printf("Input command:\n");
    scanf("%s", &inp);
    if(!strncmp(inp, MPG, strlen(MPG))){
      printf("MPG SELECTED\n");
      char *res = getId(inp);
      if(res){
        printf("IN MAIN: %s", res);
        
      }else{
        printf("NEED TO ENTER ID FOR MPG!\n");
      }
      

    } else if (!strncmp(inp, LIST, strlen(LIST))){
      printf("LIST SELECTED\n");
      char *res = getId(inp);
      if(res){
        printf("IN MAIN: %s", res);
      }else{
        printf("NEED TO ENTER ID FOR LIST!\n");
      }
    } else if (!strncmp(inp, EXIT, strlen(EXIT))){
      printf("EXIT SELECTED\n");
    }else{
      printf("UNKNOWN\n");
    }
  }
  
  */

  return 0;
}
