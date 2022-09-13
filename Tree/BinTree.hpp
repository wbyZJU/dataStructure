#include"BinNode.hpp"
#include"../Stack/Stack.hpp"
#include"../Queue/Queue.hpp"
#define stature(p) ((p)?(p)->height:-1)//节点高度，空树=-1
#define HasRChild(x) ((x).rc)
#define HasLChild(x) ((x).lc)


template<typename T>
class BinTree
{
protected:
    int _size;//规模
    BinNodePosi<T> _root;//根节点
    virtual int updateHeight(BinNodePosi<T> x);//更新节点x的高度，每种树对高度的定义不一定相同，采用虚函数，便于重写
    void updateHeightAbove(BinNodePosi<T> x);//更新节点x及其祖先的高度
    template<typename VST> static void visitAlongLeftBranch(BinNodePosi<T> x, VST& visit, Stack<BinNodePosi<T>>& S);
    static void goAlongLeftBranch(BinNodePosi<T> x, Stack<BinNodePosi<T>>& S);
    static void gotoLeftmostLeaf(Stack<BinNodePosi<T>>& S);

public:
    BinTree():_size(0),_root(nullptr){}//构造函数
    ~BinTree();//析构函数
    int size() const{return _size;}//规模
    bool empty(){return !_root;}//判空
    BinNodePosi<T> root() const{return _root;}//树根

    BinNodePosi<T> insert(T const& e, BinNodePosi<T>);//插入左孩子
    BinNodePosi<T> insert(BinNodePosi<T>, T const& e);//插入右孩子 

    //先序遍历
    template<typename VST> void travPre_R(BinNodePosi<T> x, VST& visit);//二叉树先序遍历，递归版
    template<typename VST> void travPre_I1(BinNodePosi<T> x, VST& visit);//二叉树先序遍历，迭代版1
    template<typename VST> void travPre_I2(BinNodePosi<T> x, VST& visit);//二叉树先序遍历，迭代版2
    //中序遍历
    template<typename VST> void travIn_R(BinNodePosi<T> x, VST& visit);//中序遍历，递归版
    template<typename VST> void travIn_I1(BinNodePosi<T> x, VST& visit);//迭代版本
    //后序遍历
    template<typename VST> void travPost_R(BinNodePosi<T> x, VST& visit);//后序遍历，递归版
    template<typename VST> void travPost_I(BinNodePosi<T> x, VST& visit);//后序遍历，迭代
    //层次遍历
    template<typename VST> void travLevel(VST& visit);

};

template<typename T>
int BinTree<T>::updateHeight(BinNodePosi<T> x)
{
    return x->height = 1+max(stature(x->lc),stature(x->rc));
}

template<typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi<T> x)//O(n=depth(x))
{
    while(x)
    {
        updateHeight(x);
        x=x->parent;
    }
}

template<typename T>
BinNodePosi<T> BinTree<T>::insert(T const& e, BinNodePosi<T> x)
{
    _size++;
    x->insertAsLc(e);
    updateHeightAbove(x);
    return x->lc;
}

template<typename T>
BinNodePosi<T> BinTree<T>::insert(BinNodePosi<T> x, T const& e)
{
    _size++;
    x->insertAsRc(e);
    updateHeightAbove(x);
    return x->rc;
}

//template<typename T, typename VST>
template<typename T>
template<typename VST>
void BinTree<T>::travPre_R(BinNodePosi<T> x, VST& visit)
{
    if(!x) return;
    visit(x->data);
    travPre_R(x->lc,visit);
    travPre_R(x->rc,visit);
}

template<typename T>
template<typename VST>
void BinTree<T>::travPre_I1(BinNodePosi<T> x, VST& visit)
{
    Stack<BinNodePosi<T>> s;
    if(x) s.push(x);
    while(!s.empty())
    {
        x=s.pop();
        visit(x->data);
        if(HasRChild(*x)) s.push(x->rc);//右孩子先进后出
        if(HasLChild(*x)) s.push(x->lc);//左孩子后进先出

    }
}

template<typename T>
template<typename VST>
void BinTree<T>::visitAlongLeftBranch(BinNodePosi<T> x, VST& visit, Stack<BinNodePosi<T>>& S)
{
    while(x)//反复的
    {
        visit(x->data);//访问当前节点
        S.push(x->rc);//右子树入栈，将来逆序出栈
        x->lc;//沿左侧链下行
    }
   
}

template<typename T>
template<typename VST>
void BinTree<T>::travPre_I2(BinNodePosi<T> x, VST& visit)
{
    Stack<BinNodePosi<T>> S;//辅助栈
    while(true)//以右子树为单位，逐批访问节点
    {
        visitAlongLeftBranch(x,visit,S);//访问子树x的左侧链,右子树入栈缓冲
        if(S.empty()) break;//栈空退出
        x=S.pop();//弹出下一子树的根
    }
}

template<typename T>
template<typename VST>
void BinTree<T>::travIn_R(BinNodePosi<T> x, VST& visit)
{
    if(!x) return ;
    travIn_R(x->lc,visit);
    visit(x->data);
    travIn_R(x->rc,visit);
}

template<typename T>
void goAlongLeftBranch(BinNodePosi<T> x, Stack<BinNodePosi<T>>& S)
{
    while(x)
    {
        S.push(x);
        x=x->lc;
    }
}//逐层深入，左侧各点依次入栈

template<typename T>
template<typename VST> 
void BinTree<T>::travIn_I1(BinNodePosi<T> x, VST& visit)
{
    Stack<BinNodePosi<T>> S;//辅助栈
    while(true)
    {
        goAlongLeftBranch(x,S);//从当前节点出发，逐批入栈
        if(S.empty())
        {
            break;//直至所有节点处理完毕
        }
        x=S.pop();//x的左子树或者空
        visit(x->data);
        x=x->rc;//转向右子树，可能为空
    }
}

template<typename T>
template<typename VST>
void BinTree<T>::travPost_R(BinNodePosi<T> x, VST& visit)
{
    if(!x) return ;
    travPost_R(x->lc,visit);
    travPost_R(x->rc,visit);
    visit(x->data);
}


template<typename T>
void BinTree<T>::gotoLeftmostLeaf(Stack<BinNodePosi<T>>& S)
{
    while(BinNodePosi<T> x=S.top())//自顶而下反复检查栈顶元素
    {
        if(HasLChild(*x))//尽可能向左
        {
            if(HasRChild(*x)) S.push(x->rc);//若有右孩子，优先入栈
            S.push(x->lc);//转向左
        }
        else//不得已
        {
            S.push(x->rc);//转向右孩子
        }
    }
    S.pop();//返回之前，弹出栈顶空节点
}


template<typename T>
template<typename VST>
void BinTree<T>::travPost_I(BinNodePosi<T> x, VST& visit)
{
    Stack<BinNodePosi<T> > S;//辅助栈
    if(x) S.push(x);//根节点入栈
    while(!S.empty())//X始终为当前节点
    {
        if(S.top()!=x->parent)//若栈顶非X之父（而为右兄）
        {
            gotoLeftmostLeaf(S);//递归深入
        }
        x=S.pop();//弹出栈顶（前一节点后继）更新x
        visit(x->data);//并随之访问
    }
}

template<typename T>
template<typename VST>
void BinTree<T>::travLevel(VST& visit)
{
    Queue<BinNodePosi<T> >Q;//引入辅助队列
    Q.enqueue(this);//根节点入队
    while(!Q.empty())//队列变空之前，反复迭代
    {
        BinNodePosi<T> x=Q.dequeue();//取出队首元素
        visit(x->data);//访问
        if(HasLChild(*x)) Q.enqueue(x->lc);//左孩子入队
        if(HasRChild(*x)) Q.enqueue(x->rc);//右孩子入队
    }
}