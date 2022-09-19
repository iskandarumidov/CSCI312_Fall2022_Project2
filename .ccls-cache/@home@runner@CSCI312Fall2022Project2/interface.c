#include "interface.h"
/*
char *getId(char *inp) {
  char *res = malloc(strlen(inp));
  res = strtok(inp,",");
  res = strtok (NULL, ",");
  return res;
}*/

int main(void)
{
  int toChild[2];          // Pipe to child
  int toParent[2];         // Pipe to parent
  int err;                 // Will use for error checking
  int childPid;            // Process ID of server
  int status;              // Server exit status
  err = pipe(toChild);     // Open toChild pipe file descriptor
  if (err == -1) {
    printf ("Error on pipe creation: %d\n", errno);
    exit(EXIT_FAILURE);
  }
  err = pipe(toParent);    // Open toParent pipe file descriptor
  if (err == -1) {
    printf ("Error on pipe creation: %d\n", errno);
    exit (EXIT_FAILURE);
  }

  childPid = fork();       // Fork to get new process
  if (childPid == -1) {
    printf ("parent: fork failed, errno = %d\n", errno);
    exit (1);
  } else if ( childPid == 0 ) { // In child now if PID == 0
    close (toChild[1]);
    close (toParent[0]);
    char char_fd_to_child[10];
    sprintf(char_fd_to_child, "%d", toChild[0]);
    char char_fd_to_parent[10];
    sprintf(char_fd_to_parent, "%d", toParent[1]);

    // Now run the Server.  First argument is program name, then pipe file descriptors
    err = execl ("./server", "server", char_fd_to_child, char_fd_to_parent, (char *)NULL);
    if ( err == -1 ) {
      printf ("parent: execl failed, errno = %d\n", errno);
      exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
  }
  printf ("parent: child created with pid = %d\n", childPid);
  close (toChild[0]);
  close (toParent[1]);

  /*
  err = write (toChild[1], "grgument_in_pipe", strlen("argument_in_pipe")+1);
  if (err == -1 ) {
    printf ("Error on write to pipe: %d\n", errno);
    exit (1);
  }*/

  /*
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
  printf("PARENT FROM BYTES: %d\n", n);*/

  



  
  char inp[20];    // Input
  while(1){
    //printf("Input command:\n");
    fprintf(stderr, "Input command:\n");
    scanf("%s", &inp);
    // Once Interface has input, send it over to Server
    err = write(toChild[1], inp, strlen(inp)+1);
    if (err == -1 ) {
      printf ("Error on write to pipe: %d\n", errno);
      exit (EXIT_FAILURE);
    }

    // After command was sent to Server, read response from pipe
    int n;
    char buf[10000];
    //n = read(toParent[0], &buf, 10000);
    char result[10000];
    int i = 0;
    char temp;

    
    //while (read(toParent[0], &temp, 1) > 1) {
      //if (read(toParent[0], &buf, 10000) < 1) break;
      //result[i] = buf[0];
      //strcpy(result, buf);
      //i++;
      //fprintf(stderr, "chaine : %c\n", temp);
    //}
    //fprintf(stderr, "\n");
    //fprintf(stderr, "res: %s\n", buf);
    //fprintf(stderr, "HELLO\n");
    //n = read(toParent[0], &buf, 10000);
    //fprintf(stderr, "res: %s\n", buf);

/*
    while(1){
      n = read(toParent[0], &buf, 1);
      if (n < 1){
        break;
      }
      fprintf(stderr, "res: %s\n", buf);
      fprintf(stderr, "1\n");
    }*/
    n = read(toParent[0], &buf, 10000);
    for (int i=0; i < n; i++) {
      //if(buf[i] == "\0"){
      if (buf[i] == NULL || buf[i] == '\0'){
        fprintf(stderr, "\n");
      }else{
        fprintf(stderr, "%c", buf[i]);
      }
    }
    //fprintf(stderr, "\n");
    
    if (n < 0) {
      printf("pipe read failed, errno = %d\n", errno);
      exit(EXIT_FAILURE);
    }
    buf[n] = '\0';
    fflush(stdout);
    sleep(1); // Sometimes STDOUT gets garbled, sleep for 1 sec for it to resolve
    //printf("PARENT FROM PIPE: %s\n", buf);

    if (!strncmp(inp, "exit", strlen("exit"))){
      break;
    }
  }
    if (waitpid(childPid, &status, 0) == -1) {    // Wait until child is done
      perror("waitpid failed");
      return EXIT_FAILURE;
    }

    if (WIFEXITED(status)) {      // Detect child exit status
      const int es = WEXITSTATUS(status);
      fprintf(stderr, "child exit status was %d\n", es);
    }
  fprintf(stderr, "PARENT EXITING...\n");
  return EXIT_SUCCESS;
}
