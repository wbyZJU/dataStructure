using Rank = int;
class Fib//Fibonacci数列
{
private:
	Rank g, f;
public:
	Fib(Rank n)
	{
		g = 0; 
		f = 1;
		while (g < n) next();
	}
	Rank get()
	{//当前项
		return g;
	}
	Rank next()
	{//后一项
		g += f;
		f = g - f;
		return g;
	}
	Rank prev()
	{//转至前一项
		f = g - f;
		g = g - f;
		return g; 
	}
};