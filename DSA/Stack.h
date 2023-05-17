#include "Vector.h" //以向量为基类，派生出栈模板类

template <typename T> 
class Stack: public Vector<T> { //将向量的首/末端作为栈底/顶

public: //size()、empty()以及其它开放接口，均可直接沿用
//入栈：等效于将新元素作为向量的末元素插入
void push ( T const& e ) { 
    insert ( size(), e ); 
    } 
//出栈：等效于删除向量的末元素
T pop() { 
    return remove ( size() - 1 ); 
    } 
//取顶：直接返回向量的末元素
T& top() { 
    return ( *this ) [size() - 1]; 
    } 
};
