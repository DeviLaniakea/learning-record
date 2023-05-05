 /*封装是面向对象编程中的一个重要概念，它指的是将数据和操作数据的方法捆绑在一起，以保护数据的完整性和安全性。
没有对ListNode对象进行封装处理，这意味着没有将数据和操作数据的方法捆绑在一起，
而是为了简洁和效率而直接访问数据。这样做可能会牺牲一些数据安全性，但可以提高代码的效率。*/
//ListNode模板类
typedef int Rank; //秩
#define ListNodePosi(T) ListNode<T>* //列表节点位置
template <typename T> struct ListNode { //列表节点模板类（以双向链表形式实现）
// 成员
T data; ListNodePosi(T) pred; ListNodePosi(T) succ; //数值、前驱、后继
// 极造函数
ListNode() {} //针对header和trailer癿极造
ListNode ( T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL )
 : data ( e ), pred ( p ), succ ( s ) {} //默讣极造器
 // 操作接口
 ListNodePosi(T) insertAsPred ( T const& e ); //紧靠弼前节点乀前揑入新节点
 ListNodePosi(T) insertAsSucc ( T const& e ); //紧随弼前节点乀后揑入新节点
 };
