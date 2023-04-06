typedef int Rank;   //秩
#define DEFAULT_CAPACITY 3  //默认的初始容量（实际应用中可设置更大）

template <typename T> class Vector  //向量模板类，一个参数类型为T的叫做Vector的类的模板
{
    
protected:

    Rank _size; int _capacity; T* _elem;   //规模、容量、数据空间
    void copyFrom(T const* A,Rank lo,Rank hi);    //复制数组区间A[lo, hi)
    void expand();      //空间不足时扩容
    void shrink();  //装填因子过小时压缩
    void bubble(Rank lo,Rank hi);   //扫描交换
    void bubbleSort(Rank lo,Rank hi);   //冒泡排序算法
    Rank max(Rank lo,Rank hi);  //选择最大元素
    void selectionSort(Rank lo,Rank hi);    //选择排序算法
    void merge(Rank lo,Rank hi);    //归并算法
    void mergeSort(Rank lo,Rank hi);    //归并排序算法
    Rank partition(Rank lo,Rank hi);    //轴点构造法
    void quickSort(Rank lo,Rank hi);    //快速排序算法
    void heapSort(Rank lo,Rank hi);     //堆排序

private:
    /* data */
public:
    //构造函数
    /*默认的构造方法是，
    首先根据创建者指定的初始容量，向系统申请空间，以创建内部私有数组_elem[];
    若容量未明确指定，则使用默认值DEFAULT_CAPACITY。
    接下来，鉴于初生的向量尚不包含任何元素，故将指示规模的变量_size初始化为0。*/
    Vector(int c = DEFAULT_CAPACITY,int s = 0,T v = 0)  //容量为c，规模为s，所有元素初始为v
    {
        _elem = new T[_capacity = c];
        for (_size = 0; _size < s; _elem[_size++] = v)   //s<=c        
    };

    Vector ( T const* A, Rank n ) { copyFrom ( A, 0, n ); } //数组整体复制
    Vector ( T const* A, Rank lo, Rank hi ) { copyFrom ( A, lo, hi ); } //区间
    Vector ( Vector<T> const& V ) { copyFrom ( V._elem, 0, V._size ); } //向量整体复制
    Vector ( Vector<T> const& V, Rank lo, Rank hi ) { copyFrom ( V._elem, lo, hi ); } //区间

    //析构函数
    /*只需释放用于存放元素的内部
    数组_elem[]，将其占用的空间交还操作系统。_capacity和_size之类的内部变量无需做任何
    处理，它们将作为向量对象自身的一部分被系统回收，此后既无需也无法被引用。*/
    ~Vector(){delete[] _elem;}

    // 叧读讵问接口
    Rank size() const { return _size; } //规模
    bool empty() const { return !_size; } //判空
    int disordered() const; //判断向量是否已排序
    Rank find ( T const& e ) const { return find ( e, 0, _size ); } //无序向量整体查找
    Rank find ( T const& e, Rank lo, Rank hi ) const; //无序向量区间查找
    Rank search ( T const& e ) const //有序向量整体查找
    { return ( 0 >= _size ) ? -1 : search ( e, 0, _size ); }
    Rank search ( T const& e, Rank lo, Rank hi ) const; //有序向量区间查找
    // 可写访问接口
    T& operator[] ( Rank r ) const; //重载下标操作符，可以类似于数组形式引用各元素
    Vector<T> & operator= ( Vector<T> const& ); //重载赋值操作符，以便直接克隆向量
    T remove ( Rank r ); //删除秩为r的元素
    int remove ( Rank lo, Rank hi ); //删除秩在区间[lo, hi)之内的元素
    Rank insert ( Rank r, T const& e ); //插入元素
    Rank insert ( T const& e ) { return insert ( _size, e ); } //默认作为末元素插入
    void sort ( Rank lo, Rank hi ); //对[lo, hi)排序
    void sort() { sort ( 0, _size ); } //整体排序
    void unsort ( Rank lo, Rank hi ); //对[lo, hi)置乱
    void unsort() { unsort ( 0, _size ); } //整体置乱
    int deduplicate(); //无序去重
    int uniquify(); //有序去重
    // 遍历
    void traverse ( void (* ) ( T& ) ); //遍历（使用函数指针，只读或局部性修改）
    template <typename VST> void traverse ( VST& ); //遍历（使用函数对象，可全局性修改）

    
};

//基于复制的构造方法
template <typename T> //元素类型
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) { //以数组区间A[lo, hi)为蓝本复制向量
    _elem = new T[_capacity = 2 * ( hi - lo ) ]; _size = 0; //分配原数组2倍空间，规模清零 
    while ( lo < hi ) //A[lo, hi)内元素逐一
        _elem[_size++] = A[lo++]; //复制至_elem[0, hi - lo)
}

//重载向量赋值操作符

/*需强调的是，由于向量内部含有动态分配的空间，默认的运算符"="不足以支持向量之间的直接赋值。
例如，6.3节将以二维向量形式实现图邻接表，其主向量中的每一元素本身都是一维向量，
故通过默认赋值运算符，并不能复制向量内部的数据区。为适应此类赋值操作的需求，重载向量的赋值运算符。*/

/*为什么要重载赋值运算符：对于简单的类，默认的赋值运算符一般就够用了，我们也没有必要再显式地重载它。
但是当类持有其它资源时，例如动态分配的内存、打开的文件、指向其他数据的指针、网络连接等，
默认的赋值运算符就不能处理了，我们必须显式地重载它，这样才能将原有对象的所有数据都赋值给新对象。*/

/*运算符重载函数本质就是将一个运算符与一个类内函数做映射，运算符不再执行他原始的程序，而执行你重载那个函数*/
template <typename T> 
Vector<T>& Vector<T>::operator= ( Vector<T> const& V ) { //重载，不过是一个函数名比较特殊的一个函数，函数名规则是：operator接运算符
    if ( _elem ) 
        delete [] _elem; //释放原有内容
    copyFrom ( V._elem, 0, V.size() ); //整体复制
    return *this; //返回当前前对象的引用，以便链式赋值
}

//空间不足时扩容，应当在执行所有添加操作之前执行
template <typename T> 
void Vector<T>::expand(){
    if ( _size < _capacity ) return; //尚未满员时，无需扩容
    if ( _capacity < DEFAULT_CAPACITY ) 
        _capacity = DEFAULT_CAPACITY; //不低于默认最小容量
    T* oldElem = _elem; 
    _elem = new T[_capacity <<= 1]; //容量加倍
    for ( int i = 0; i < _size; i++ )
        _elem[i] = oldElem[i]; //复制原向量内容（T为基本类型，或已重载赋值操作符'='）
    delete [] oldElem; //释放原空间
}