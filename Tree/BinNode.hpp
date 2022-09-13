typedef enum{RB_RED, RB_BLACK} RBColor;

template<typename T> struct BinNode;
template<typename T> using BinNodePosi = BinNode<T>*;//节点位置

template<typename T>
struct BinNode
{
//成员
    T data;
    BinNodePosi<T> parent,lc,rc;//父节点以及左右孩子
    int height;
    int npl;//Null path length(左式堆，也可直接用height替代)
    RBColor color;//颜色（红黑树）
//构造函数
    BinNode():parent(nullptr),lc(nullptr),rc(nullptr),height(0),npl(0),color(RB_RED){}
    BinNode(T e,BinNodePosi<T> parent=nullptr,BinNodePosi<T> lc=nullptr,BinNodePosi<T> rc=nullptr,
            int h=0, int l=0,RBColor c=RB_RED):data(e),parent(parent),lc(lc),rc(rc),
                                               height(h),npl(l),color(c){}
//操作接口
    int size();//统计当前节点后代总数，亦即以其为根的子树的规模
    BinNodePosi<T> insertAsLc(T const& );
    BinNodePosi<T> insertAsRc(T const& );
};

//实现
template<typename T>
BinNodePosi<T> BinNode<T>::insertAsLc(T const& e)
{
    return lc = new BinNode(e,this);
}

template<typename T>
BinNodePosi<T> BinNode<T>::insertAsRc(T const& e)
{
    return rc = new BinNode(e,this);
}

template<typename T>
int BinNode<T>::size()
{
    int s=1;
    if(lc) s += lc->size();
    if(rc) s += rc->size();

    return s; 
}

