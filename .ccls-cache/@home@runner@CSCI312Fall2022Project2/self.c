#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main (int argc, char *argv[], char *envp[]){
  int err;
  int i;
  char param[10];


  if (argc > 1) {
    for (i = 0; i < argc; i++)
      printf ("argument %d: %s\n", i,argv[i]);
    exit (0);
  } else {
    sprintf (param, "%d", 12345);
    err = execl ("./self", "self", param,(char *)NULL);
  exit (1);
}