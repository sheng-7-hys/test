
//头文件包含
#include<stdlib.h>
#include<stdio.h>
#include<malloc.h>

//函数状态码定义
#define TRUE       1
#define FALSE      0
#define OK         1
#define ERROR      0
#define OVERFLOW   -1
#define INFEASIBLE -2
#define NULL  0
typedef int Status;

//二叉链表存储结构定义
typedef int TElemType;
typedef struct BiTNode{
    TElemType data;
    struct BiTNode  *lchild, *rchild; 
} BiTNode, *BiTree;

//创建二叉树各结点，输入零代表创建空树
//采用递归的思想创建
//递归边界：空树如何创建呢：直接输入0；
//递归关系：非空树的创建问题，可以归结为先创建根节点，输入其数据域值；再创建左子树；最后创建右子树。左右子树递归即可完成创建！

Status CreateBiTree(BiTree &T){
   TElemType e;
   scanf("%d",&e);
   if(e==0)T=NULL;
   else{
     T=(BiTree)malloc(sizeof(BiTNode));
     if(!T)exit(OVERFLOW);
     T->data=e;
     CreateBiTree(T->lchild);
     CreateBiTree(T->rchild);
   }
   return OK;  
}

//下面是需要实现的函数的声明
int GetDepthOfBiTree ( BiTree T){
    if(T==NULL)return 0;
    else{
      int leftDepth=GetDepthOfBiTree(T->lchild);
      int rightDepth=GetDepthOfBiTree(T->rchild);
      return (leftDepth>rightDepth?leftDepth:rightDepth)+1;
    }
}
int LeafCount(BiTree T){
    if(T==NULL)return 0;
    else if(T->lchild==NULL && T->rchild==NULL)return 1;
    else return LeafCount(T->lchild)+LeafCount(T->rchild);
}
//下面是主函数
int main()
{
   BiTree T;
   int depth, numberOfLeaves;
   CreateBiTree(T);
   depth= GetDepthOfBiTree(T);
     numberOfLeaves=LeafCount(T);
   printf("%d %d\n",depth,numberOfLeaves);
}

/* 请在这里填写答案 */