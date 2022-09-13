#include<iostream>
using Rank = int;
template<typename T> struct ListNode;
template<typename T> 
using ListNodePosi = ListNode<T>*;//C++.0x 模板别名
//#define Posi(T) ListNode<T>*
template<typename T>
struct ListNode
{
//成员
    T data;//数值
    ListNodePosi<T> pred;//前驱 指向上一个node的指针
    //Posi(T) pred;
    ListNodePosi<T> succ;//后继
    //Posi(T) succ;
//构造函数
    ListNode(){}//针对header和trailer的构造
    ListNode(T e, ListNodePosi<T>p=NULL, ListNodePosi<T>s=NULL)
    :data(e),pred(p),succ(s){}//初始化列表 默认构造
//操作接口
    ListNodePosi<T> insertAsPred(T const& e);//紧靠当前节点之前插入新节点
    ListNodePosi<T> insertAsSucc(T const& e);//紧靠当前节点之后插入新节点

};

template<typename T>
ListNodePosi<T> ListNode<T>::insertAsPred(T const& e)
{
    ListNodePosi<T> x = new ListNode(e,pred,this);
    pred->succ = x;
    pred = x;
    return x;
}

template<typename T>
ListNodePosi<T> ListNode<T>::insertAsSucc(T const& e)
{
    ListNodePosi<T> x= new ListNode(e,this,succ);//创建新节点
    succ->pred = x;
    succ=x;//设置逆向链接
    return x;//返回新节点的位置
}