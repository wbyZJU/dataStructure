#include<iostream>
#include<string.h>
using namespace std;
#include"Stack.hpp"
#define N_OPTR 9
typedef enum {ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE} Operator;//运算符集合

void readNumber(char* &p, Stack<double> &stk);
char orderBetween(char op1, char op2);
Operator optr2rank(char op);
double calcu(char op, double b);
double calcu(double num1, char op, double num2);
int facl(int num);
double pow(double num1, double num2);
void append(char* &rpn, double opnd);
void append(char* &rpn, char optr);

float evaluate(char* S, char* &RPN)
{
    Stack<char> optr;//运算符栈
    Stack<double> opnd;//运算数栈
    optr.push('\0');
    while(!optr.empty())//逐个字符处理
    {
        if(isdigit(*S))//操作数
        {
            readNumber(S, opnd);//读入
            append(RPN,opnd.top());
        }
        else//运算符
        {
            switch(orderBetween(optr.top(), *S))//比较栈顶运算符和当前运算符之间优先级的高低
            {
                //分别处理
                case '<'://栈顶运算符的优先级更低
                {
                    optr.push(*S);
                    S++;
                    break;
                }
                case '='://优先级相等（当前运算符右括号或者哨兵结束符）
                {
                    optr.pop();
                    S++;
                    break;
                }
                case '>':
                {
                    char op=optr.pop();
                    append(RPN, op);
                    if('!'==op)//一元运算符
                    {
                        opnd.push(calcu(op,opnd.pop()));
                    }
                    else
                    {
                        double pOpnd2 = opnd.pop();
                        double pOpnd1 = opnd.pop();
                        opnd.push(calcu(pOpnd1, op, pOpnd2));
                    }
                    break;
                }
            }
        }
    }
    return opnd.pop();
} 


void readNumber(char* &p, Stack<double>& stk)
{
    stk.push((double)(*p-'0'));//将对应数字进栈
    while(isdigit(*(++p)))//多位整数
    {
        stk.push(stk.pop()*10+(*p-'0'));
    }
    if(*p == '.')//小数部分
    {
        double fraction=1;
        while(isdigit(*(++p)))//逐位
        {
            stk.push(stk.pop()+(*p-'0')*(fraction/=10));//加入
        }
    }
}

const char pri[N_OPTR][N_OPTR]={
/* -- + */ '>', '>', '<', '<', '<', '<', '<', '>', '>',
/* |  - */ '>', '>', '<', '<', '<', '<', '<', '>', '>',
/* 栈 * */ '>', '>', '>', '>', '<', '<', '<', '>', '>',
/* 顶 / */ '>', '>', '>', '>', '<', '<', '<', '>', '>',
/* 运 ^ */ '>', '>', '>', '>', '>', '<', '<', '>', '>',
/* 算 ! */ '>', '>', '>', '>', '>', '>', ' ', '>', '>',
/* 符 ( */ '<', '<', '<', '<', '<', '<', '<', '=', ' ',
/* |  ) */ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
/* --\0 */ '<', '<', '<', '<', '<', '<', '<', ' ', '='
//          +    -    *    /    ^    !    (    )    \0
//          |-------------- 当前运算符 --------------|
};

Operator optr2rank(char op)
{
    switch(op)
    {
        case'+':return ADD;
        case'-':return SUB;
        case'*':return MUL;
        case'/':return DIV;
        case'^':return POW;
        case'!':return FAC;
        case'(':return L_P;
        case')':return R_P;
        case'\0':return EOE;
        default:exit(-1);
    }
}
char orderBetween(char op1, char op2)
{
    return pri[optr2rank(op1)][optr2rank(op2)];
}

double calcu(char op, double b)//一元运算
{
    switch(op)
    {
        case '!':return (double)facl(int(b)); break;
        default:exit(-1);
    }
}
int facl(int num)
{
    if(num<=1) return 1;
    else
    {
        return num*facl(num-1);
    }
}
double calcu(double num1, char op, double num2)
{
    switch(op)
    {
        case '+':return num1+num2;
        case '-':return num1-num2;
        case '*':return num1*num2;
        case '/':return num1/num2;
        case '^':return pow(num1, num2);
    }
}
double pow(double num1, double num2)
{
    double res=1;
    while((int)num2--)
    {
        res = res*num1;
    }
    return res;
}

void append(char* &rpn, double opnd)
{
    char buf[64];
    if((int)opnd<opnd)
    {
        sprintf(buf,"%6.2f\0",opnd);
    }
    else{
        sprintf(buf,"%d\0",(int)opnd);
    }
    strcat(rpn,buf);
}
void append(char* &rpn, char optr)
{
    int n=strlen(rpn);
    sprintf(rpn+n,"%c\0",optr);
}
int main()
{
    char s[]={'(','0','!','+','1','1',')','*','2','^','3'};
    char ss[]={'(','0','!','+','1','1',')','*','2','^','3'};
    char* p=ss;
    //char s[]={'1','1','+','8'};
    /* Stack<double>stk;
    char* p=s;
    readNumber(p,stk);
    while(!stk.empty())
    {
        cout<<stk.pop()<<endl;
    } */
    cout<<evaluate(s,p)<<endl;
    /* for(int i=0;i<5;i++)
    {
        cout<<ss[i]<<endl;
    } */
    //printf("%f\n",evaluate(s));
    return 0;
}