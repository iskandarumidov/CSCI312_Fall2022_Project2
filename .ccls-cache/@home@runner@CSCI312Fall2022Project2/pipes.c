int main(void){
  int fd[2];
  if(pipe(fd) == -1){
    printf("ERR!\n");
    return 1;
  }

  int id = fork();
  if (id == 0){
    int x;
    printf("INP NUM:\n");
    scanf("%d", &x);
    write(fd)

  }
}