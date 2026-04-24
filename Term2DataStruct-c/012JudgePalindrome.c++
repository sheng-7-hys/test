#include <stdio.h>
#include <stdlib.h>
typedef char DataType;
typedef struct SNode
{
    DataType data;      
    struct SNode *next;
}SNode,*LinkStack;            
int InitLinkStack( LinkStack *top )
{
    *top = (LinkStack)malloc( sizeof(SNode) );
    if( *top == NULL ) 
    { 
        printf("初始化链栈出错");
        return 0;    
    }
    (*top)->next = NULL;
    return  1;
}
int LinkStackEmpty( LinkStack top )
{
    if( top->next == NULL ) 
        return 1;
    else
        return 0;
}
int Push(LinkStack top, DataType e)
{
    SNode *p;
    p = (SNode*)malloc(sizeof(SNode));
    if (!p)
    {
        printf("入栈操作出错!\n");
        return 0;
    }
    p->data = e;
    p->next = top->next;
    top->next = p;
    return 1;
}
int Pop(LinkStack top, DataType *e)
{
    SNode *p;
    if (!top->next)
    {
        printf("栈已空，无法完成出栈操作!\n");
        return 0;
    }
    p = top->next;
    top->next = p->next;
    *e = p->data;
    free(p);
    return 1;
}
int Destroy(LinkStack top)
{
    SNode *p;
    while (top)
    {
        p = top;
        top = top->next;
        free(p);
    }
    return 1;
}
typedef struct QNode
{
    DataType data;            
    struct QNode *next;        
}LQNode,*PQNode ;
typedef struct 
{ 
    PQNode front, rear;        
}LinkQueue;
int InitLinkQueue(LinkQueue* Q)
{
    Q->front = Q->rear = (PQNode)malloc(sizeof(LQNode));
    if (!Q->front)
    {
        printf("初始化队列失败！\n");
        return 0;
    }
    Q->front->next = NULL;
    return 1;
}
int LinkQueueEmpty(LinkQueue Q)
{
    if (Q.front == Q.rear) return 1;
    else return 0;
}
int EnLinkQueue(LinkQueue* Q, DataType e)
{
    PQNode p;
    p = (PQNode)malloc(sizeof(LQNode));
    if (!p)
    {
        printf("内存分配失败，不能完成入队操作！\n");
        return 0;
    }
    p->data = e;
    p->next = NULL;//初始化入队结点
    Q->rear->next = p;
    Q->rear = p;
    return 1;
}
int DeLinkQueue(LinkQueue* Q, DataType* e)
{
    PQNode p;
    if (Q->front == Q->rear)
    {
        printf("队列已空，不能完成出队操作！\n");
        return 0;
    }
    p = Q->front->next;
    *e = p->data;
    Q->front->next = p->next;
    free(p);
    if (Q->rear == p) //若删除的队列是最后一个结点，则移动队尾指针
    {
        Q->rear = Q->front;
    }
    return 1;
}
int DestroyLinkQueue(LinkQueue* Q)
{
    while (Q->front)
    {
        Q->rear = Q->front->next;
        free(Q->front);
        Q->front = Q->rear;

    }
    return 1;
}
/* 本题要求函数 */
int JudgePalindrome(LinkStack st,LinkQueue qu)
{
    char s_char, q_char;
    // 【关键修正】拷贝栈指针，避免修改原栈（解决类型错误）
    LinkStack temp_st = st;
    // 循环弹出栈顶、出队队头元素，逐一比对
    while(temp_st != NULL && qu.front != qu.rear)
    {
        // 出栈获取栈顶元素（传拷贝后的栈指针地址）
        Pop(temp_st, &s_char);
        // 出队获取队头元素
        DeLinkQueue(&qu, &q_char);
        // 元素不相等，直接返回-1（不是回文）
        if(s_char != q_char)
        {
            return -1;
        }
    }
    // 所有元素比对完成且全部相等，返回2（是回文）
    return 2;
}
int main()
{
    LinkStack s;
    LinkQueue q;
    char ch;
    int rst;
    InitLinkStack(&s);
    InitLinkQueue(&q);
    while((ch=getchar())!='\n')
    {
        Push(s,ch);
        EnLinkQueue(&q,ch);
    }
    rst = JudgePalindrome(s,q);
    if ( rst == -1 )
    {
        printf("不是回文字符串\n");
    } 
    else if ( rst == 2 )
    {
        printf("是回文字符串\n");
    }
    Destroy(s);
    DestroyLinkQueue(&q);
    return 0;
}

/* 请在这里填写答案 */