// Program to create a simple calculator
#include <stdio.h>

int main() {
  while(1){
  char operation;
  double n1, n2;
  printf("Enter an operator (+, -, *, /):\n");
  scanf("%c %lf %lf", &operation, &n1, &n2);
  //printf("Enter two operands:\n");
  //scanf("%lf %lf",&n1, &n2);
  
    switch(operation){
      case '+':
        printf("%.1lf + %.1lf = %.1lf\n",n1, n2, n1+n2);
        break;
      case '-':
        printf("%.1lf - %.1lf = %.1lf\n",n1, n2, n1-n2);
        break;
      case '*':
        printf("%.1lf * %.1lf = %.1lf\n",n1, n2, n1*n2);
        break;
      case '/':
        printf("%.1lf / %.1lf = %.1lf\n",n1, n2, n1/n2);
        break;
      // operator doesn't match any case constant +, -, *, /
      default:
        printf("Error! operator is not correct\n");
    }
    //operation = ' ';
    //printf("Enter an operator (+, -, *, /):\n");
    //scanf("%c", &operation);
    //printf("Enter two operands:\n");
    //scanf("%lf %lf",&n1, &n2);
  }
  return 0;
}