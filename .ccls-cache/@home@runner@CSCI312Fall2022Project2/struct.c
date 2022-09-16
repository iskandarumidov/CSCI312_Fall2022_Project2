#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

struct rec{
    int account;
    char name[100];
    double balance;
};

int main()
{
    struct rec rec1;

    FILE *fptr;
  
    //fptr = fopen("clients.txt", "r");
    char buf[50];
    int fd = open("./gasData", O_RDONLY, 0755);


    lseek(fd,0,SEEK_SET);
    int n = read(fd,&buf,sizeof(struct rec));
    //n = read(fd, buf, 10);
    write(1, buf, 10);
    fclose(fptr);
  /*
    if (fptr == NULL)
        printf("File could not be opened, exiting program.\n");
    else
    {
        printf("%-10s%-13s%s\n", "Account", "Name", "Balance");
        while (!feof(fptr))
        {
            fscanf(fptr, "%d%s%lf", &rec1.account, rec1.name, &rec1.balance);
            //fread(&rec1, sizeof(rec1),1, fptr);
            printf("%8d %10s %10f\n", rec1.account, rec1.name, rec1.balance);
            printf("i = %d, f = %f, str = %s\n", i, f, str);
        }
        fclose(fptr);
    }*/
    return 0;
}