#include "interface.h"

char *getId(char *inp) {
  char *res = malloc(strlen(inp));
  res = strtok(inp,",");
  res = strtok (NULL, ",");
  return res;
}

int main(void)
{
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
  
  

  return 0;
}
