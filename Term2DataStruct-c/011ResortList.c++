#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct node{
    char address[10];
    int data;
    char next[10];
}node;
node **sort(node *list,int n,char next[]){
    node **list1=(node**)malloc(sizeof(node*)*n);
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(strcmp(list[j].address,next)==0){
                list1[i]=&list[j];
                strcpy(next,list[j].next);
                break;
            }
        }
    }
    return list1;
}
/*node **resort(node **list1,int n){
    node **list2=(node**)malloc(sizeof(node*)*n);
    for(int i=0;i<n/2;i++){
        list2[2*i]=list1[n-1-i];
        list2[2*i+1]=list1[i];
    }
    return list2;
}
*/
node **resort(node **list1,int n){
    node **list2=(node**)malloc(sizeof(node*)*n);
    int idx = 0;
    int left = 0, right = n-1;
    while(idx < n){
        list2[idx++] = list1[right--];
        if(idx >= n) break;
        list2[idx++] = list1[left++];
    }
    return list2;
}
int main(){
    char next[10];
    int n;
    scanf("%s %d",next,&n);
    node *list=(node*)malloc(sizeof(node)*n);
    for(int i=0;i<n;i++){
        scanf("%s %d %s",list[i].address,&list[i].data,list[i].next);
    }
    node **list1=sort(list,n,next);
    node **list2=resort(list1,n);
    for(int i=0;i<n;i++){
       printf("%.5s %d ",list2[i]->address,list2[i]->data);
if(i == n-1)
    printf("-1\n");
else
    printf("%.5s\n",list2[i+1]->address);
    }
    return 0;

}