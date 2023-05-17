#include "Stack.h"

//十进制到r进制转换：循环取余法
void convert ( Stack<char>& S, __int64 n, int base ) { //十进制数n到base进制的转换（递归版）
static char digit[] //0 < n, 1 < base <= 16，新进制下的数位符号，可视base取值范围适当扩充
= { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
if ( 0 < n ) { //在尚有余数之前，不断通过递归得到所有更高位
convert ( S, n / base, base ); 
S.push ( digit[n % base] ); //输出低位
}
} //新进制下由高到低的各数位，自顶而下保存于栈S中
