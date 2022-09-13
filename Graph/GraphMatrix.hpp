#include"../Vector/Vector.hpp"
#include"Graph.hpp"
template<typename Tv>
struct Vertex
{
    Tv data;//数据
    int inDegree, outDegree;//出入度数
    VStatus status;//状态
    int dTime, fTime;//时间标签
    Rank parent;//在遍历树中的父节点
    int priority;//优先级数
    Vertex(Tv const& d):data(d),inDegree(0),outDegree(0),status(UNDISCOVERED),
                        dTime(-1),fTime(-1),parent(-1),priority(INT8_MAX){}//构造新顶点
};


template<typename Te>
struct Edge
{
    Te data;//数据
    int weight;//权重
    EType type;//类型
    Edge(Te const& d, int w):data(d),weight(w),type(UNDETERMINED){}
};


template<typename Tv, typename Te>
class GraphMatrix:public Graph<Tv, Te>//基于向量，以邻接矩阵实现的图
{
private:
    Vector<Vertex<Tv> > V;//顶点集（向量）
    Vector<Vector<Edge<Te>*>> E;//邻接矩阵

public:
    GraphMatrix(){this->n=this->e=0;}//构造
    ~GraphMatrix()//析构
    {
        for(Rank j=0;j<this->n;j++)
        {
            for(Rank k=0;k<this->n;k++)
            {
                delete E[j][k];//逐条清除
            }
        }
    }
//顶点操作
    //顶点的基本操作：查询第i个顶点
    virtual Tv& vertex(Rank i){return V[i].data;}
    virtual int inDegree(Rank i){return V[i].inDegree;}//入度
    virtual int outDegree(Rank i){return V[i].outDegree;}//出度
    virtual VStatus& satus(Rank i){return V[i].VStatus;}//状态
    virtual int& dTime(Rank i){return V[i].dTime;}//时间戳
    virtual int& fTime(Rank i){return V[i].fTime;}
    virtual Rank& parent(Rank i){return V[i].parent;}//父节点
    virtual int& priority(Rank i){return V[i].priority;}//优先级
    virtual Rank nextNbr(Rank i,Rank j){while((j>-1)&&exists(i,j--)) return j;}//逆向线性试探
    virtual Rank firstNbr(Rank i){return nextNbr(i,this->n);}//假想有哨兵存在
    //顶点的动态操作
    virtual Rank insert(Tv const& vertex){//插入顶点，返回编号
        for(int i=0;i<this->n;i++){E[i].insert(nullptr);}//邻接矩阵每行新增一个元素（各顶点预留一条潜在的关联边）
        this->n++;//更新顶点数目
        E.insert(Vector<Edge<Te>*>(this->n,this->n,nullptr));//创建新顶点对应的边向量
        return V.insert(Vertex<Tv>(vertex));//顶点向量增加一个顶点
    }
    virtual Tv remove(Rank i){

    }

    
//边操作
    //基本操作
    virtual EType& type(Rank i, Rank j){return E[i][j].EType;}//边类型
    virtual Te& edge(Rank i, Rank j){return E[i][j].data;};//边数据
    virtual int& weight(Rank i,Rank j){return E[i][j].weight;}//权重
    virtual bool exists(Rank i, Rank j){return 0<=i&&i<this->n&&0<=j&&j<this->n&&E[i][i]!=nullptr;}
    //动态操作
    virtual void insert(Te const& edge,int w,Rank i,Rank j)
    {
        if(exists(i,j)) return;//确保该边不存在
        E[i][j]=new Edge<Te>(edge,w);//创建新边
        this->e++;//更新边数
        V[i].outDegree++;//更新尾出度
        V[j].inDegree++;//更新头 入度
    }
    virtual void remove(Rank i,Rank j){
        Te eBak=edge(i,j);//备份删除边信息
        delete E[i][j];//删除边
        E[i][j]=nullptr;//置空
        this->e--;//更新边数
        V[i].outDegree--;
        V[j].inDegree--;
        return eBak;//返回删除的边的信息
    }
};