#include<stdio.h>
//判断是否为素数
int is_prime(int n){
    if(n <= 1) return 0;
    for(int i = 2; i * i <= n; i++){
        if(n % i == 0){
            return 0; 
        }
    }
    return 1; 
}
int main(void){
    int a,b,temp,sum=0;
    scanf("%d %d",&a,&b);
    a<=b?0:(temp=a,a=b,b=temp); //确保a小于等于b
    for(int i=a;i<=b;i++){
        if(is_prime(i)){
            sum+=i;
        }
    }
    printf("%d\n", sum);
    return 0;
}