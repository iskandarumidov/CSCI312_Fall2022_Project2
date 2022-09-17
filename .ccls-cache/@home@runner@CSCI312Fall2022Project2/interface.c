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
  err = pipe(toChild);
  if (err == -1) {
    printf ("Error on pipe creation: %d\n", errno);
    exit (1);
  }
  err = pipe(toParent);
  if (err == -1) {
    printf ("Error on pipe creation: %d\n", errno);
    exit (1);
  }

  childPid = fork();
  if (childPid == -1) {
    printf ("parent: fork failed, errno = %d\n", errno);
    exit (1);
  } else if ( childPid == 0 ) { // IN CHILD NOW
    if (childPid == 0) {
      close (toChild[1]);
      close (toParent[0]);
    }
    char char_fd_to_child[10];
    sprintf(char_fd_to_child, "%d", toChild[0]);
    char char_fd_to_parent[10];
    sprintf(char_fd_to_parent, "%d", toParent[1]);
    
    err = execl ("./server", "server", char_fd_to_child, char_fd_to_parent, (char *)NULL);
    if ( err == -1 ) {
      printf ("parent: execl failed, errno = %d\n", errno);
      exit (2);
    }
    exit(0);
  }
  printf ("parent: child created with pid = %d\n", childPid);
  close (toChild[0]);
  close (toParent[1]);

  err = write (toChild[1], "grgument_in_pipe", strlen("argument_in_pipe")+1);
  if (err == -1 ) {
    printf ("Error on write to pipe: %d\n", errno);
    exit (1);
  }

  int n;
  char buf[10000];
  n = read(toParent[0], &buf, 10000);
  if (n < 0) {
    printf("pipe read failed, errno = %d\n", errno);
    exit(1);
  }
  buf[n] = '\0';
  fflush(stdout);
  sleep(1); // Sometimes STDOUT gets garbled, sleep for 1 sec for it to resolve
  printf("PARENT FROM PIPE: %s\n", buf);
  printf("PARENT FROM BYTES: %d\n", n);

  

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
