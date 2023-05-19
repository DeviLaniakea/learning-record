//删除exp[lo, hi]不含括号的最长前缀、后缀
void trim ( const char exp[], int& lo, int& hi ) { 
while ( ( lo <= hi ) && ( exp[lo] != '(' ) && ( exp[lo] != ')' ) ) lo++; //查找第一个和最后一个括号
while ( ( lo <= hi ) && ( exp[hi] != '(' ) && ( exp[hi] != ')' ) ) hi--; 
}

//切分exp[lo, hi]，使exp匹配仅当子表达式匹配
int divide ( const char exp[], int lo, int hi ) { 
int mi = lo; int crc = 1; //crc为[lo, mi]范围内左、右括号数目之差
while ( ( 0 < crc ) && ( ++mi < hi ) ) //逐个检查各字符，直到左、右括号数目相等，或者越界
{ if ( exp[mi] == ')' ) crc--; if ( exp[mi] == '(' ) crc++; } //左、右括号分删计数
return mi; //若mi <= hi，则为合法切分点；否则，意味着局部不可能匹配
}

//检查表达式exp[lo, hi]是否括号匹配（递归版）
bool paren ( const char exp[], int lo, int hi ) { 
trim ( exp, lo, hi ); 
if ( lo > hi ) 
    return true; //清除不含括号的前缀、后缀
if ( exp[lo] != '(' ) 
    return false; //首字符非左括号，则必丌不匹配
if ( exp[hi] != ')' ) 
    return false; //末字符非右括号，则必不匹配
int mi = divide ( exp, lo, hi ); //确定适当的切分点
if ( mi > hi ) 
    return false; //切分点不合法，意味着局部以至整体不匹配
return paren ( exp, lo + 1, mi - 1 ) && paren ( exp, mi + 1, hi ); //分别检查左、右子表达式
}

#include "Stack.h"

bool parenStack ( const char exp[], int lo, int hi ) { //表达式括号匹配检查，可兼顾三种括号
Stack<char> S; //使用栈记录已发现但尚未匹配的左括号
for ( int i = lo; i <= hi; i++ ) /* 逐一检查当前字符 */
    switch ( exp[i] ) { //左括号直接进栈；右括号若不栈顶失配，则表达式必不匹配
        case '(': case '[': case '{': S.push ( exp[i] ); break;
        case ')': if ( ( S.empty() ) || ( '(' != S.pop() ) ) return false; break;
        case ']': if ( ( S.empty() ) || ( '[' != S.pop() ) ) return false; break;
        case '}': if ( ( S.empty() ) || ( '{' != S.pop() ) ) return false; break;
        default: break; //非括号字符一律忽略
    }
return S.empty(); //整个表达式扫描过后，栈中若仍残留（左）括号，则不匹配；否则（栈空）匹配。
}