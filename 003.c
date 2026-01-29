#include<stdio.h>
int main(){
    char *str="Hello, World!";
    char *p=str;
    printf("%p\n",p);
    printf("%p\n",str);
    printf("%s\n",p);
    return 0;
}