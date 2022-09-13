#include"List.hpp"

int main()
{
    List<int> l;
   for(int i=0;i<10;i++)
   {
        l.insertAsLast(i);
   }
    std::cout<<l.selectMax()->data<<std::endl;
    return 0;
}