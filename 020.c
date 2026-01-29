#include<stdio.h>
int main() {
    char str[21];int count=0;
    while(fgets(str, sizeof(str), stdin)!=NULL){ 
        count++;
    }
    printf("%d\n", count);  

    return 0;
}