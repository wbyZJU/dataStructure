using Rank = int; //秩
#define DEFAULT_CAPACITY 3 //默认的初始容量
#include "fib.h"
#include <iostream>
template <typename T>
class Vector //向量模板类
{
protected:
	Rank _size; //规模
	int _capacity; //容量
	T* _elem; //数据区
	void copyFrom(T const* A, Rank lo, Rank hi); //复制数组区间A[lo, hi)
	void expand();//向量容量不够时扩容
	void shrink();//缩容
	int disordered() const;
	bool bubble(Rank lo, Rank hi);
	void bubbleSort(Rank lo, Rank hi);
	//ToDo堆排序

	void merge(Rank lo, Rank mi, Rank hi);//归并
	void mergeSort(Rank lo, Rank hi);//归并排序

public:
//构造函数
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) // 容量为c，规模为s，所有元素初始化为0
	{
		_elem = new T[_capacity = c];
		for (_size = 0; _size < s; _size++)
		{
			_elem[_size] = v;
		}
		//for(_size = 0; _size < s; _elem[_size++] = v);
	}
	Vector(T const* A, Rank n) //数组整体复制
	{
		copyFrom(A, 0, n);
	}
	Vector(T const* A, Rank lo, Rank hi) //区间
	{
		copyFrom(A, lo, hi);
	}
	Vector(Vector<T> const& v) //向量整体复制
	{
		copyFrom(v._elem, 0, v._size);
	}
	Vector(Vector<T> const& v, Rank lo, Rank hi) //区间
	{
		copyFrom(v._elem, lo, hi);
	}
//析构函数
	~Vector()
	{
		delete[] _elem; //释放内部空间
	}
//只读访问接口
	bool empty() const{return !_size;}
	Rank size() const{return _size;}
	Rank find(T const& e, Rank lo, Rank hi) const;//无序向量区间查找
	Rank find(T const& e)const { return find(e, 0, _size); }//无需向量整体查找
	//Rank search(T const& e, Rank lo, Rank hi) const { return (rand() % 2) ? binSearch() : fibSearch();}//有序向量区间查找，二分查找与fib查找各50%概率。
//可写访问接口
	T& operator[](Rank r);//重载下标操作符，可类似于数组引用各元素
	T remove(Rank r);//删除秩为r的元素
	int remove(Rank lo, Rank hi);//删除秩在区间[lo,hi)之内的元素
	Rank insert(Rank r, T const& e);//秩为r处插入元素
	Rank insert(T const& e){return insert(_size,e);}//末尾插入元素
	//ToDo
	void sort(Rank lo, Rank hi);
	int deduplicate();//无序去重
	int uniquify();//有序去重

//遍历
	void traverse(void(*visit)(T&));//遍历（使用函数指针，只读或局部性修改）
	template<typename VST>void traverse(VST&);//遍历（使用函数对象，可全局修改）
};



template <typename T> 
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi)
{
	_elem = new T[_capacity = 2 * (hi - lo)]; //分配空间
	for (_size = 0; lo < hi; lo++, _size++)
	{
		_elem[_size] = A[lo]; //A[lo,hi)区间元素逐一复制到_elem[0,hi-lo)
	}
}//const修饰，保证A中元素不会被篡改

template <typename T> 
void Vector<T>::expand()
{
	if (_size < _capacity) return;//判断是否空间不足,未满员不必扩容
	if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;//不低于最小容量
	//_capacity = max(_capacity, DEFAULT_CAPACITY);
	T* oldElem = _elem;
	_elem = new T[_capacity *= 2];//容量加倍
	for (int i = 0; i < _size; i++)
	{
		_elem[i] = oldElem[i];//复制原向量内容
	}
	delete oldElem;//释放原空间
}

template<typename T>
void Vector<T>::shrink()//缩容
{
	if (_capacity < DEFAULT_CAPACITY << 1)return;
	if (_size << 2 > _capacity)return;//25%为界
	T *oldElem = _elem;
	_elem = new T[_capacity >>= 1];//位右移赋值，容量减半
	for (int i = 0; i < _size; i++) _elem[i] = oldElem[i];//复制原向量内容
	delete[]oldElem;//释放原空间
}

template <typename T> 
T& Vector<T>::operator[](Rank r)
{
	return _elem[r];//0<=r<=_size
}

template <typename T> 
Rank Vector<T>::insert(Rank r, T const& e)
{
	expand();//如有必要，扩容
	for (int i = _size; i > r; i--)
	{
		_elem[i] = _elem[i - 1];//自后向前，元素向后移动
	}
	_elem[r] = e;
	_size++;
	return r;
}

template <typename T> 
int Vector<T>::remove(Rank lo, Rank hi)//删除秩在区间[lo,hi)之内的元素
{
	if (lo == hi) return 0;//单独考虑退化情况，加快效率
	while (hi < _size)
	{
		_elem[lo++] = _elem[hi++];//从前向后左移元素
	}
	_size = lo;//更新向量的规模
	shrink();//非必要操作
	return hi - lo;//返回被删除元素的个数
}

template<typename T> 
T Vector<T>::remove(Rank r)//删除秩为r的元素，返回被删除元素
{
	T e = _elem[r];//备份被删除的元素
	remove(r, r + 1);//调用区间删除函数
	return e;//返回被删除的元素
}

template<typename T> 
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const//无序向量区间查找
{//返回最后一个元素e的位置，查找失败返回lo-1 [lo, hi)
	while ((lo < hi--) && (e != _elem[hi]));//从后向前，逆序查找
	return hi;//若hi<lo,则失败，否则hi为命中元素的秩
}

template<typename T> 
int Vector<T>::deduplicate()//无序去重
{//返回被删除元素的总数，故需要记录初始向量的规模
	int oldSize = _size;//记录初始向量的矩阵
	Rank i = 1;//从_elem[1]开始
	while (i < _size)//自前向后开始考察，前缀中有雷同（假设最多只有一个雷同的）的删除，若无继续向后考察
	{
		find(_elem[i], 0, i) < 0 ? i++ : remove(_elem[i]);
	}
	return oldSize - _size;
}

template<typename T>
void Vector<T>::traverse(void(*visit)(T&))//遍历（使用函数指针，只读或局部性修改）
{
	for (int i = 0; i < _size; i++)
	{
		visit(_elem[i]);
	}
}
template<typename T>template<typename VST>
void Vector<T>::traverse(VST&visit)//遍历（使用函数对象，可全局修改），通用性更强
{
	for (int i = 0; i < _size; i++)
	{
		visit(_elem[i]);
	}
}

/*函数对象练习*/
//template<typename T>
//class Increase
//{
//	void operator()(T&e)
//	{
//		e++;
//	}
//
//};
//
//template<typename T>
//void Increase(Vector<T>& v)
//{
//	v.traverse(Increase<T>());
//}
template<typename T>
int Vector<T>::disordered() const//返回逆序对的数目
{
	int n = 0;//计数器
	for (int i = 1; i < _size; i++)
	{
		if (_elem[i - 1] > _elem[i]) n++;//比较相邻元素
	}
	return n;//向量有序有且仅当n=0
}

//template<typename T>
//int Vector<T>::uniquify()//有序去重-低效版
//{
//	int oldSize = _size;//记录原有向量的规模
//	int i = 0;
//	while (i < _size - 1)
//	{
//		_elem[i] == _elem[i + 1] ? remove(i + 1) : i++;//若相同，则删除后面一个元素，否则转到后一元素。
//	}
//	return oldSize - _size;//返回删除元素的个数，
//}
template<typename T>
int Vector<T>::uniquify()//高效版
{
	/*int i = 0;
	int j = 0;*/
	Rank i = 0;//各对互异“相邻”元素的秩
	Rank j = 0;
	while (++j < _size)//逐一扫描
	{
		if (_elem[i] != _elem[j])
			_elem[++i] = _elem[j];//发现不同元素，前移至紧邻于前者右侧
	}
	_size = ++i;
	shrink();//直接截除尾部元素
	return j - i;//返回向量的规模变化
}

//template<typename T>
//Rank Vector<T>::binSearch(T const& e, Rank lo, Rank hi)
//{
//	Rank mi = (lo + hi) >> 1;//以中点为轴点
//	while (lo < hi)
//	{
//		if (e < _elem[mi])
//		{
//			hi = mi;//深入前半段继续查找，[lo,mi)
//		}
//		else if (_elem[mi]<e)
//		{
//			lo = mi + 1;//深入后半段继续查找，(mi,hi)
//		}
//		else
//			return mi;//查找成功提前返回
//	}
//	return -1;//查找失败
//}


//版本A
template<typename T>
static Rank binSearch_A(T *A, T const& e, Rank lo, Rank hi)
{
	
	while (lo < hi)
	{
		Rank mi = (lo + hi) >> 1;//以中点为轴点
		if (e < A[mi])
		{
			hi = mi;//深入前半段继续查找，[lo,mi)
		}
		else if (A[mi] < e)
		{
			lo = mi + 1;//深入后半段继续查找，(mi,hi)
		}
		else
			return mi;//查找成功提前返回
	}
	return -1;//查找失败
}//不符合语义约定，返回秩最大者

template<typename T>
static Rank fibSearch(T *A, T const& e, Rank lo, Rank hi)
{
	Fib fib(hi - lo);
	while (lo < hi)
	{
		while (hi - lo < fib.get()) fib.prev();
		Rank mi = lo + fib.get() - 1;
		if (e < A[mi])
		{
			hi = mi;
		}
		else if (A[mi] < e)
		{
			lo = mi + 1;
		}
		else
			return mi;
	}
	return -1;
}

//二分查找，版本B
template<typename T>
static Rank binSearch_B(T *A, const T& e, Rank lo, Rank hi)
{
	while (1 < hi - lo)//每次迭代做一次判断，有两个分支；成功查找不能提前返回
	{
		Rank mi = (lo + hi) / 2;//中点为轴点
		e < A[mi] ? hi = mi : lo = mi;//经比较后确定深入[lo,mi)还是[mi,hi)
	}//出口时hi=lo+1，查找区间仅含一个元素A[lo]
	return (e == A[lo])?lo:(-1);//查找成功返回对应的秩，否则返回-1
}//多个元素命中时，不能保证返回秩最大者，查找失败时，返回-1，而不能返回失败的位置

//二分查找， 版本C
template<typename T>
static Rank binSearch(T *A, const Rank& e, Rank lo, Rank hi)
{
	while (lo < hi)//每次迭代仅作一次比较判断，两分支
	{
		Rank mi = (lo + hi) / 2;//以中点为轴点
		e < A[mi] ? hi = mi : lo = mi + 1;
	}//成功查找不能提前终止
	return --lo;//循环结束时，lo为大于e的元素的秩最小者，故lo-1为不大于e元素的秩最大者
}//多个元素命中时，总能保证返回秩最大者，查找失败时，返回失败的位置。

template<typename T>
bool Vector<T>::bubble(Rank lo, Rank hi)
{
	bool isSort = true;//用来判断是否已经整体有序，便于提前中止排序
	while (++lo < hi)//从左至右依次检测
	{
		if (_elem[lo - 1] > _elem[lo])//存在逆序则交换
		{
			swap(_elem[lo - 1], _elem[lo]);
			isSort = false;//无序
		}
	}
	return isSort;//返回是否整体有序
}

template<typename T>//向量的起泡排序
void Vector<T>::bubbleSort(Rank lo, Rank hi)
{
	while (!bubble(lo, hi--));
}

//改进版冒泡排序
/********************************************
template<typename T>
Rank Vector<T>::bubble(Rank lo, Rank hi)
{
	Rank last = lo;
	while (++lo < hi)
	{
		if (_elem[lo - 1] > _elem[lo])
		{
			last = lo;
			swap(_elem[lo - 1], _elem[lo]);
		}
	}
	return last;
}

template<typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi)
{
	while (lo < (hi = bubble(lo, hi)));
}
*********************************************/

template <typename T>//递归
void Vector<T>::mergeSort(Rank lo, Rank hi)
{
	if (hi - lo < 2)
		return;

	Rank mi = (lo + hi) / 2;
	mergeSort(lo, mi);
	mergeSort(mi, hi);
	merge(lo, mi, hi);
}

template <typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi)
{
	T* A = _elem + lo;
	int lb = mi - lo;
	T* B = new T[lb];//临时存放A的前半部分
	for (int i = 0; i < lb; i++) B[i] = A[i];//数据挪到B
	int lc = hi - mi;
	T* C = _elem + mi;
	for (int i = 0, j = 0, k = 0; j < lb || k < lc;)
	{
		if (j < lb && (k >= lc || B[j] <= C[k])) A[i++] = B[j++];//短路求值 C[k]越界时将B中剩余元素放到A
		if (k < lc && (j >= lb || C[k] < B[j])) A[i++] = C[k++];//B[j]越界时将C中剩余元素放到A
	}
	delete[] B;//释放临时空间B
}

//merge分开版本
/*********************************************************
template<typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi)
{
	//构造一个和A一样大的数组来存放结果
	Rank *temp = new Rank[hi - lo];
	int i = lo, j = mi + 1;
	int lb = mi;
	int lc = hi;
	int k = 0;
	while (i <= lb && j < lc)
	{
		if (_elem[i] <= _elem[j])
		{
			temp[k++] = _elem[i++];
		}
		else
			temp[k++] = _elem[j++];
	}
	while (i <= lb)
	{//比较完毕，第一组数有剩余，填进temp
		temp[k++] = _elem[i++];
	}
	while (j < lc)
	{
		temp[k++] = _elem[j++];
	}
	for (i = 0; i < k; i++) {// 将排好序的数填回到array数组的对应位置
		_elem[lo + i] = temp[i];
	}
}
*********************************************************/
