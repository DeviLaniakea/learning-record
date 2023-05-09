 /*封装是面向对象编程中的一个重要概念，它指的是将数据和操作数据的方法捆绑在一起，以保护数据的完整性和安全性。
没有对ListNode对象进行封装处理，这意味着没有将数据和操作数据的方法捆绑在一起，
而是为了简洁和效率而直接访问数据。这样做可能会牺牲一些数据安全性，但可以提高代码的效率。*/
//ListNode模板类
typedef int Rank; //秩
#define ListNodePosi(T) ListNode<T>* //重命名节点类型指针，叫做ListNodePosi，在算法中用它指代目标节点当前位置

template <typename T> struct ListNode { //列表节点模板类（以双向链表形式实现）
// 成员
T data; ListNodePosi(T) pred; ListNodePosi(T) succ; //数值、前驱指针、后继指针
// 定义两种构造函数
/*默认构造函数没有参数，用于创建一个空的节点对象，并使用默认值初始化数据成员，在c++中，假如一个int类型的data会变成随机值，而指针会默认为空指针。
带参数的构造函数接受参数，并用参数的值来初始化数据成员。*/
ListNode() {} //针对header和trailer的空节点构造
ListNode ( T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL ) : data ( e ), pred ( p ), succ ( s ) {} //默认构造器
 // 插入操作接口
 ListNodePosi(T) insertAsPred ( T const& e ); //紧靠当前节点之前插入新节点
 ListNodePosi(T) insertAsSucc ( T const& e ); //紧随当前节点之后插入新节点
 };
 

 //前插操作
template <typename T> //将e紧靠当前节点之前插入于当前节点所属列表（设有哨兵头节点header）
ListNodePosi(T) ListNode<T>::insertAsPred ( T const& e ) {
    ListNodePosi(T) x = new ListNode ( e, pred, this ); //创建新节点
    pred->succ = x; pred = x; //维护两条旧链接，顺序不能颠倒
    return x; //返回新节点的位置
}


 //后插操作
template <typename T> //将e紧靠当前节点之后插入于当前节点所属列表（讴有哨兵尾节点trailer）
ListNodePosi(T) ListNode<T>::insertAsSucc ( T const& e ) {
    ListNodePosi(T) x = new ListNode ( e, this, succ ); //创建新节点
    succ->pred = x; succ = x; //设置逆向链接
    return x; //返回新节点的位置
}
