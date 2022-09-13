#include"Stack.hpp"
#include<iostream>
using namespace std;
//进制转换
void convert(Stack<char>& S, long long n, int base)
{
    char digit[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    while(n>0)
    {
        S.push(digit[n%base]);
        n /= base;
    }


}
//括号匹配
bool paren(const char* exp, int lo, int hi)
{
    Stack<char> s;//使用栈记录发现但尚未匹配的左括号
    for(int i =lo;i<hi;i++)//逐一检查当前字符
    {
        if('('==exp[i])
        {
            s.push('(');//遇左括号则进栈
        }
        else if(!s.empty())
        {
            s.pop();//遇右括号；若战非空，则弹出左括号
        }
        else return false;//遇右括号时栈已空，则比不匹配
    }
    return s.empty();
}
//栈混洗甄别
bool stackPermutation(Stack<int>& A, Stack<int>& B)
{//栈B是否为栈A的混洗  只是为了判断B混洗是否存在，而非有多少种混洗
    
    Stack<int> s,temp;
    while(!B.empty())
    {
        temp.push(B.top());
        B.pop();
    }
    while(!A.empty())
    {
        s.push(A.top());
        A.pop();
        if(temp.top()==s.top())
        {
            temp.pop();
            s.pop();
            while(!s.empty()&&temp.top() == s.top())
            {
                temp.pop();
                s.pop();
            
            }
        }
    }
    return syouku.empty();

}
int main()
{
    //Stack<char> s;
    /* long long num = 180;
    convert(s,num,16);
    while(!s.empty())
        printf("%c", s.pop());
    printf("\n"); */
    
    //char exp[]={'(','(',')','(',')',')'};
    /* if(paren(exp,0,6))
    {
        cout<<"匹配"<<endl;
    } */
    Stack<int> a,b;
    a.push(3);
    a.push(2);
    a.push(1);
    a.push(4);

    b.push(4);
    b.push(3);
    b.push(1);
    b.push(2);

    cout<<stackPermutation(a,b)<<endl;

    return 0;

}