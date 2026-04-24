#include <stdio.h>
#include <malloc.h>
typedef int Status;
typedef char SElemType;
#define stack_INIT_SIZE     100
#define stackINCREMENT  10
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
 typedef struct {
       SElemType  *base;   //栈底指针
       SElemType  *top;     //栈顶指针
       int stacksize;       //栈空间
}Sqstack;

Status iniStack(Sqstack &S) //初始化栈
{
 //构造一个空栈S
    S.base=(SElemType*)malloc(stack_INIT_SIZE * sizeof(SElemType));
    if(!S.base)return(ERROR);
    S.top=S.base;
    S.stacksize=stack_INIT_SIZE;
    return OK;
  }//InitStack

/* 你的代码将被嵌入到这里 */
在这里描述函数接口。例如：
Status push(Sqstack &S,SElemType x) //x入栈S
{
}
Status pop(Sqstack &S,SElemType &e)//从S栈出栈1次元素放入e
{
}
Status Compare(char s[]) //s为表达式
{
   Sqstack S;
   SElemType e;
   Status flag=TRUE;
   int i=0;
   iniStack(S);
   while(s[i]!='#' && flag==TRUE )
   {
       switch(s[i])
       {
         case '(':
         case '[':
         case '{':push(S,s[i]);break;
         case ')': if(pop(S,e)==ERROR || e!='(')//如果是(
                     flag=FALSE;break;
         case ']': if(pop(S,e)==ERROR || e!='[')//如果是[
                     flag=FALSE;break;
         case '}': if(pop(S,e)==ERROR || e!='{')//如果是{
                     flag=FALSE;break;
       }
       i++;
   }
   if(flag==TRUE && s[i]=='#' && S.top==S.base)
     return TRUE;
   else
     return FALSE;
}

int main()
{
    char s[100];
    Status st;
  scanf("%s",s);
   st=Compare(s);
    if(st==TRUE)
        printf("检查结果：括号匹配！");
    else
        printf("检查结果：括号不匹配！");
    return 0;
}
