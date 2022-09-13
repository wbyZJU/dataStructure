using VStatus = enum{UNDISCOVERED,DISCOVERED,VISITED};//顶点状态
using EType = enum{UNDETERMINED,TREE,CROSS,FORWARD,BACKWARD};//边在遍历树中的类型
template <typename Tv, typename Te>
class Graph
{
private:
    void reset(){

    }
    void BFS(Rank, int&);//（连通域0）广度优先算法
public:
//顶点
    int n;//顶点总数
    virtual Rank insert(Tv const&)=0;//插入顶点，返回编号
    virtual Tv remove(Rank)=0;//删除顶点及其关联边，并返回该顶点信息
    virtual Tv& vertex(Rank)=0;//顶点的数据
    virtual int inDegree(Rank)=0;//顶点的入度
    virtual int outDegree(Rank)=0;//顶点的出度
    virtual Rank firstNbr(Rank)=0;//顶点的首个邻接顶点
    virtual Rank nextNbr(Rank, Rank)=0;//顶点（相对当前邻居的）下一邻居
    virtual VStatus& status(Rank)=0;//顶点的状态
    virtual int& dTime(Rank)=0;
    virtual int& fTime(Rank)=0;
    virtual Rank& parent(Rank)=0;//顶点在遍历树中的父亲
    virtual int& priority(Rank)=0;//顶点在遍历树中的优先级数
//边
    int e;//边数
    virtual bool exists(Rank, Rank)=0;//边uv是否存在
    virtual void insert(Te const&,int,Rank,Rank)=0;//在指定顶点之间插入指定权重的边
    virtual Te remove(Rank,Rank)=0;//删除一对顶点之间的边，返回该边信息
    virtual EType& type(Rank, Rank)=0;//边类型
    virtual Te& edge(Rank, Rank)=0;//边数据
    virtual int& weight(Rank,Rank)=0;//边权重

};
