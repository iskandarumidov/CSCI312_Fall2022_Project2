#include<stdio.h>
#include<string.h>
#include<stdlib.h>



int main(void)
{
    FILE *fptr=fopen( "clients.txt","r");
    int  i;
    char arr[200];     //THIS ARRAY WILL HOLD THE CONTENTS OF A LINE TEMPORARILY UNTIL IT IS PRINTED                       
    int temp_index=0,line_number=1;;
    memset(arr,'\0',sizeof(arr));

    while((i=getc(fptr))!=EOF)                         
    {
        if(i!='\n')                                
        {
            arr[temp_index++]=i; 
        }   
        if(i=='\n')
        {
            printf("line %d: %s\n",line_number++,arr);
            temp_index=0;
            memset(arr,'\0',sizeof(arr));
        } 
    }
    return 0;
}