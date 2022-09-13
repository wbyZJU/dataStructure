#include <iostream>

void swap(int* &p1,int* &p2){

    int *temp=p1;
    p1=p2;
    p2=temp;
    printf("交换中：a=%d,b=%d \n",*p1,*p2);
    printf("交换中(地址)：p1=%d \n",p1);
    printf("交换中(地址)：p2=%d \n",p2);
}

int main(){

    int a=1,b=3;
    int *p1=&a,*p2=&b;

    // 交换前
    printf("交换前：a=%d,b=%d \n",*p1,*p2);
    printf("交换前(地址)：p1=%d \n",p1);
    printf("交换前(地址)：p2=%d \n",p2);
    // 交换中
    swap(p1,p2);
//swap(&a,&b);
    // 交换后
    printf("交换后：a=%d,b=%d \n",*p1,*p2);
    printf("交换后(地址)：p1=%d \n",p1);
    printf("交换后(地址)：p2=%d \n",p2);
    return 0;

}
