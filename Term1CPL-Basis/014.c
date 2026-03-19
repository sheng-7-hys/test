#include<stdio.h>
#include<stdlib.h>
int main(){
    FILE *fp;
    char ch,filename[10];
    printf("Enter file name to open:\n");
    scanf("%s",filename);
    getchar();
    if((fp=fopen(filename,"w"))==NULL){
        printf("Cannot open file %s\n",filename);
        exit(1);
    }
    printf("Enter text to store in file %s (end with #):\n",filename);
    ch=getchar();
    while(ch!='#'){
        fputc(ch,fp);
        putchar(ch);
        ch=getchar();
    }
    printf("%p",fp);
    fclose(fp);
    putchar(10);  //换行ASCII码10
    return 0;
}