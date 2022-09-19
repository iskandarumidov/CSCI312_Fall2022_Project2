#include "interface.h"

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
  close (toChild[0]);
  close (toParent[1]);

  
  char inp[20];    // Input
  while(1){
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
    char buf[BUFFER_SIZE];
    int i = 0;
    char temp;

    n = read(toParent[0], &buf, BUFFER_SIZE);
    for (int i=0; i < n; i++) {
      if (buf[i] == NULL || buf[i] == '\0'){
        fprintf(stderr, "\n");
      }else{
        fprintf(stderr, "%c", buf[i]);
      }
    }
    
    if (n < 0) {
      printf("pipe read failed, errno = %d\n", errno);
      exit(EXIT_FAILURE);
    }
    buf[n] = '\0';
    fflush(stderr);
    sleep(1); // Sometimes STDERR gets garbled, sleep for 1 sec for it to resolve

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
