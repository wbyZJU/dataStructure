#include "listnode.hpp"
template<typename T>
class List//列表模板类
{
private:
    int _size;//规模
    ListNodePosi<T> header;//头哨兵
    ListNodePosi<T> trailer;//尾哨兵


protected:
    void init();//列表创建时的初始化
    int clear();//清除所有节点
    void copyNodes(ListNodePosi<T> p, int n);//复制列表中自位置p起的n项
    void mergeSort(ListNodePosi<T> &p, int n);//从列表位置p开始对连续的n个节点归并排序
    void selectionSort(ListNodePosi<T> p, int n);//从列表位置p开始对连续的n个节点选择排序
    void insertionSort(ListNodePosi<T> p, int n);//从列表位置p开始对连续的n个节点插入排序

public:
//构造函数
    List(){init();}//默认构造
    List(List<T>const& L){copyNodes(L.first(),L._size);}//整体复制列表L
    List(List<T>const& L, Rank r, int n){copyNodes(L[r],n);}//复制列表L从r起的n项
    List(ListNodePosi<T>p,int n){copyNodes(p, n);}//复制列表自位置p起的n项
    
//析构函数
    ~List(){clear(); delete header; delete trailer;}//释放所有节点

//只读访问接口

    T& operator[] (Rank r)const; //重载[],支持循秩访问
    ListNodePosi<T> first() const{return header->succ;}//返回首节点
    ListNodePosi<T> last() const{return trailer->pred;}//返回末节点位置
    ListNodePosi<T> find(T const& e) const{return find(e, _size, trailer);}//无序列表查找
    ListNodePosi<T> find(T const& e, int n, ListNodePosi<T> p) const;//无序区间查找
    ListNodePosi<T> search(T const& e) const{return find(e, _size, trailer);}//有序列表查找
    ListNodePosi<T> search(T const& e, int n, ListNodePosi<T> p) const;//有序区间查找
    ListNodePosi<T> selectMax(ListNodePosi<T> p,int n);//在p及其n-1个后继中选出最大者
    ListNodePosi<T> selectMax(){return selectMax(header->succ, _size);}//整体最大者
//可写访问接口
    ListNodePosi<T> insertAsFirst(T const& e)//将e当做首节点插入
    {_size++; return header->insertAsSucc(e);}
    ListNodePosi<T> insertAsLast(T const& e)//将e当做尾节点
    {_size++; return trailer->insertAsPred(e);}
    ListNodePosi<T> insert(T const& e, ListNodePosi<T> p)//将e作为前驱插入
    {_size++; return p->insertAsPred(e);}
    ListNodePosi<T> insert(ListNodePosi<T> p, T const& e)//将e作为后继插入
    {_size++; return p->insertAsSucc(e);}
    T remove(ListNodePosi<T> p);//删除p处的节点，返回节点值
    ListNodePosi<T> merge(ListNodePosi<T> p, int n, List<T> &L, ListNodePosi<T> q, int m);
    void sort(ListNodePosi<T> p, int n);//列表区间排序
    void sort(){sort(header->succ,_size);}//列表整体排序

    int deduplicate();//无序去重
    int uniquify();//有序去重
};

template<typename T>
void List<T>::init()
{
    header = new ListNode<T>;//创建头哨兵结点
    trailer = new ListNode<T>;//创建尾哨兵结点
    header->succ=trailer;
    header->pred=NULL;
    trailer->pred=header;
    trailer->succ=NULL;
    _size=0;
} 

template<typename T>
T& List<T>::operator[](Rank r) const
{
    ListNodePosi<T> p = first();//从首节点出发
    while(r--) p=p->succ;
    return p->data;//目标节点
}

template<typename T>
void List<T>::copyNodes(ListNodePosi<T>p,int n)
{
    init();//创建头尾节点并做初始化
    while(n--)//将自p起的n项依次作为末节点插入
    {
        insertAsLast(p->data);
        p->succ;
    }
}

template<typename T>
T List<T>::remove(ListNodePosi<T> p)
{
    T temp = p->data;//备份待删除节点的数值
    p->pred->succ=p->succ;//后继
    p->succ->pred=p->pred;//前驱 跳过p点
    delete p;//释放节点
    _size--;//更新规模
    return temp;
}

template<typename T>
int List<T>::clear()//清除列表有效节点，并返回列表初始长度
{
    int oldSize = _size;
    while(_size>0){
        remove(header->succ);//反复删除首节点，直到列表变空
    }
    return oldSize;
}

template<typename T>
ListNodePosi<T> List<T>::find(T const& e, int n, ListNodePosi<T> p) const
{
    //在节点p（不包含p）的n个真前驱中。找到等于e的最后者
    while(0 < n--)//从右至左
    {
        if(e==(p=p->pred)->data) return p;//逐一对比，直至命中
    }
    return NULL;//查找失败
}


template<typename T>
int List<T>::deduplicate()
{
    int oldSize = _size;
    if(_size<2) return 0;
    ListNodePosi<T> p = first();//从首节点开始
    for(Rank r = 0; p!=trailer; p=p->succ)
    {
        if(ListNodePosi<T> q = find(p->data, r, p))
            remove(q);
        else
            r++;//无重前缀的长度
    }
    return oldSize - _size;//删除元素的数量
}

template<typename T>
int List<T>::uniquify()//剔除重复元素
{
    if(_size<2) return 0;
    int oldSize = _size;//记录原规模
    ListNodePosi<T> p =first();//各区段起点
    ListNodePosi<T> q;//直接后继
    while((q = p->succ)!=trailer)//反复考察紧邻的节点pq
    {
        if(p->data != q->data)//若互异，转向下一对
            p = q;
        else
            remove(q);
    }
    return oldSize - _size;
}

template<typename T>
ListNodePosi<T> List<T>::search(T const& e, int n, ListNodePosi<T> p) const//有序区间查找
{
    //在有序列表节点p的n各前驱中，找到不大于e的最后者
    while(0 < n--)
    {
        if((p=p->pred)->data <= e)
            break;
    }
    return p;
}

template<typename T>//从起始位置p的n个元素中选出最大者
ListNodePosi<T> List<T>::selectMax(ListNodePosi<T> p, int n)
{
    ListNodePosi<T> max = p;//选取最大值的初始值p
    for(ListNodePosi<T> cur = p; n>1;n--)//后续节点逐一与p比较
    {
        if((cur=cur->succ)->data >= max->data)
        {
            max=cur;//更新最大值
        }
    }
    return max;
}

template<typename T>
void List<T>::selectionSort(ListNodePosi<T> p, int n)
{
    ListNodePosi<T> head = p->pred, tail=p;
    for(int i=0;i<n;i++) tail=tail->succ;//将排序区间整理为（head,tail）
    while(n>1)
    {//反复从待排序区间找出最大者，并移动到有序区间前端
        insert(remove(selectMax(head->succ,n)), tail);
        tail=tail->pred;//有序区间更新
        n--;//待排序区间更新
    }
}

template<typename T>
void List<T>::insertionSort(ListNodePosi<T> p,int n)
{
    for(int r=0;r<n;r++ )
    {
        insert(search(p->data,r,p),p->data);
        p->succ;
        remove(p->pred);
    }
}


template<typename T>
ListNodePosi<T> List<T>::merge(ListNodePosi<T> p, int n, List<T> &L, ListNodePosi<T> q, int m)
{
    ListNodePosi<T> pp = p->pred;
    while((0 < m)&&(q!=p))
    {
        if((0<n)&&(p->data<=q->data))
        {
            p=p->succ;
            n--;
        }
        else{
            insert(L.remove((q=q->succ)->pred,p));
            m--;
        }
    }
    return pp->succ;

}

template<typename T>
void List<T>::mergeSort(ListNodePosi<T> &p, int n)
{
    if(n<2) return;
    ListNodePosi<T> q = p;
    int m=n>>1;
    for(int i=0;i<m;i++)
    {
        q=q->succ;
    }
    mergeSort(p,m);
    mergeSort(q,n-m);
    p=merge(p,m,*this,q,n-m);
}