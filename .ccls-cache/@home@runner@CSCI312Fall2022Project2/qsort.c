#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


typedef struct {
    int price;
    int id;
} order;
order list[6];
int i = 0;

int compare (const void * a, const void * b){
  order *orderA = (order *)a;
  order *orderB = (order *)b;
  return ( orderA->price - orderB->price );
}

int main ()
{
    srand (time(NULL));

    printf("Before sorting\n");
    for(i=0; i<6; i++){ 
        list[i].price = rand()%10;
        list[i].id = i; 
        printf ("Order id = %d Price = %d \n",list[i].id, list[i].price);           
    }
    printf("AFTER sorting\n");
    int n;
    qsort (list, 6, sizeof(order), compare);
    for (n=0; n<6; n++)
         printf ("Order id = %d Price = %d \n",list[n].id, list[n].price);          
    return 0;
}