#include<stdio.h>
#include<stdlib.h>
#include <string.h>
float man_normal[][2]={};
typedef struct Patient{
    char sex[10];
    double data1;
    double data2;
    double data3;
    double data4;
    double data5;
    struct Patient* next;
}Patient;
Patient* create_node(char sex[10],double data1,double data2,double data3,double data4,double data5){
    Patient* p=(Patient*)malloc(sizeof(Patient));
    if(!p){
        fprintf(stderr,"内存分配失败\n");
        exit(EXIT_FAILURE);
    }
    strcpy(p->sex, sex);
    p->data1=data1;
    p->data2=data2; 
    p->data3=data3;
    p->data4=data4;
    p->data5=data5;
    p->next=NULL;
    return p;
}
int main(){
    int n;
    Patient* head=NULL;
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        char sex[10];
        double data1,data2,data3,data4,data5;
        scanf("%s %lf %lf %lf %lf %lf", sex, &data1, &data2, &data3, &data4, &data5);
        Patient* new_node = create_node(sex, data1, data2, data3, data4, data5);
        if(head==NULL){
            head=new_node;
        } else {
            new_node->next = head;
            head = new_node;
        }//头插法
    }
    Patient* cur=head;
    for(int i=0;i<n;i++){
        int count=0;



    }
}