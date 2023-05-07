typedef int Rank;   //秩
#define DEFAULT_CAPACITY 3  //默认的初始容量（实际应用中可设置更大）

template <typename T> class Vector  //向量模板类，一个参数类型为T的叫做Vector的类的模板
{
    
protected:

    Rank _size; int _capacity; T* _elem;   //规模、容量、数据空间
    void copyFrom(T const* A,Rank lo,Rank hi);    //复制数组区间A[lo, hi)
    void expand();      //空间不足时扩容
    void shrink();  //装填因子过小时压缩
    bool bubble(Rank lo,Rank hi);   //扫描交换
    void bubbleSort(Rank lo,Rank hi);   //冒泡排序算法
    Rank max(Rank lo,Rank hi);  //选择最大元素
    void selectionSort(Rank lo,Rank hi);    //选择排序算法
    void merge(Rank lo,Rank mi, Rank hi);    //归并算法
    void mergeSort(Rank lo,Rank hi);    //归并排序算法
    Rank partition(Rank lo, Rank hi);   // 轴点构造法
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
    Vector<T> &operator=(Vector<T> const &); // 重载赋值操作符，以便直接克隆向量
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
    int uniquify1B();//有序去重高效版
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

//重新实现和数组一样的通过下标访问元素的方式
template <typename T>
T& Vector<T>::operator[] ( Rank r ) const //重载下标操作符 const 修饰了 Vector<T>::operator[] 成员函数，表示这个函数不会修改它所属的 Vector<T> 对象的状态。这意味着这个函数只能访问向量中的元素，但不能修改它们。
    { return _elem[r]; } // assert: 0 <= r < _size


//置乱算法
template <typename T> 
void permute(Vector<T>& V){
    for (int i = V.size(); i > 0 ; i--)
    {
        swap(V[i-1],V[rand()%i])//V[i-1]与V[0,i)中某一随机元素交换
    }
}

//区间置乱
template <typename T> void Vector<T>::unsort ( Rank lo, Rank hi ) { //等概率随机置乱区间[lo, hi)
    T* V = _elem + lo; //将子向量_elem[lo, hi)视作另一向量V[0, hi - lo)；由于 _elem 是一个指向类型为 T 的元素的指针，所以 _elem + lo 的值是 _elem 加上 lo 个 sizeof(T) 字节的地址。也就是lo位置的首地址
    for ( Rank i = hi - lo; i > 0; i-- ) //自后向前
        swap ( V[i - 1], V[rand() % i] ); //将V[i - 1]与V[0, i)中某一元素随机交换
}

/*上面两个算法中细微的差别：后者是通过下标，直接访问内部数组的元素；
而前者则是借助重载的操作符“[]”，通过秩间接地访问向量的元素。
第一个函数 permute 接受一个 Vector<T> 对象作为参数，并使用重载的 [] 操作符通过秩间接访问向量的元素。
第二个函数 unsort 是一个类的成员函数，它可以直接访问向量的内部数组 _elem。它使用指针运算直接通过内部数组中的索引访问向量的元素。
*/

//判等和比较大小
//比较函数为两个重载函数，如果参数为指针，就获取指针对应数据并重新调用比较函数，将数据重新传给比较函数，此时比较函数重载为数值比较，并返回结果。
template <typename T> 
static bool lt ( T* a, T* b ) { return lt ( *a, *b ); } //less than

template <typename T> 
static bool lt ( T& a, T& b ) { return a < b; } //less than

template <typename T> 
static bool eq ( T* a, T* b ) { return eq ( *a, *b ); } //equal

template <typename T> 
static bool eq ( T& a, T& b ) { return a == b; } //equal


//查找函数
/*其中有四处细节：1.从后往前比较：由于约定了如果有多个相同元素，返回最大的那个，于是从后往前比较，发现的第一个就是最大的，可以直接返回了，减少了不必要的比较
2.查找失败的返回：选择统一返回最终的比较元素的rank，在从后往前比较时，查找失败返回-1负数既有失败的含义，便于理解，又可假想在数组开始0的位置的左侧有个哨兵，返回哨兵表示查找失败
3.使用while循环而非for循环：由于在循环控制语句中有两个判断语句，由于C/C++语言在前一判断非真后循环会立即终止，而不致因试图引用已越界的秩（-1）而出错。
4.lo<hi--先比较再立刻--，既检测了合法范围，又同时实现了右区间是开区间的效果，真正的元素比较时不会比较hi位置元素*/
template <typename T> //无序向量的顺序查找：返回最后一个元素e的位置；失败时，返回lo - 1
Rank Vector<T>::find(T const& e,Rank lo,Rank hi ) const{
    while ((lo<hi--)&&(e!=_elem[hi]))//右界大于左界并且右界位置元素和e不相同时，右界左移一位后再次进行while的两个括号内的比较
    return hi;//若hi < lo，则意味着失败；否则hi即命中元素的秩
    }

//插入操作
//需要注意腾出待插入空间得操作，必须从后往前得依次向后搬运，否则会数据覆盖导致丢失。
template <typename T>
Rank Vector<T>::insert(Rank r,T const& e){
    expand();   //先检查是否需要扩容
    for ((int i = _size; i > r ; i--)//size是元素总数，并且是最后一个元素后移一位的位置，r是需要空出来的待插入位置，由于是先搬运再前移，i=r时搬运的是r-1的位置，不需要搬运，所以在i=r+1后停止，也就是i=r时停止
         _elem[i] = _elem[i-1];//从后往前的往后搬运
         _elem[r] = e;
         _size++;
         return r;
    }
    
//区间删除操作
template <typename T> 
int Vector<T>::remove ( Rank lo, Rank hi ) { //删除区间间[lo, hi)
    if ( lo == hi ) return 0; //出于效率考虑，单独处理退化情况，比如remove(0, 0)
    while ( hi < _size ) _elem[lo++] = _elem[hi++]; //[hi, _size)顺次前移hi - lo个单元
    _size = lo; //更新规模，直接丢弃尾部[lo, _size = hi)区间
    shrink(); //若有必要，则缩容
    return hi - lo; //返回被删除元素数目
}

//单体删除操作
template <typename T> 
T Vector<T>::remove ( Rank r ) { //删除向量中秩为r的元素，0 <= r < size
    T e = _elem[r]; //备份被删除元素
    remove ( r, r + 1 ); //调用区间删除算法，等效于对区间[r, r + 1)的删除
    return e; //返回删除元素
}

//唯一化：删除相同元素（无需版）
/*从前往后依次查询前缀中是否有雷同，这样就能保证前缀的局部实现，然后依次向后查询前缀是否雷同，不雷同就继续往后查，雷同就删除当前元素，直到循环结束*/
template <typename T>
int Vector<T>::deduplicate(){
    int oldSize = _size;    //记录原始规模
    Rank i = 1; //第一个元素天然满足，因此从第二个元素开始
    while (i<_size) //i从第一个查到最后一个，也就是_size-1那个位置
    {
        (find(_elem[i],0,i))<0?i++:remove(i);   //在当前元素i的前缀，也就是[0，i)之间查询是否有和i相同的元素（为什么是半开区间，看find函数），如果没有就i++，如果有返回对应的秩，返回结果必然>0,就删除当前元素，然后继续查询i位置上的新元素。
    }
    return oldSize - _size; //返回删除元素数量
    
}


//遍历
/*为了灵活性需要将基本操作作为参数传入遍历函数，基本方法有函数指针和函数对象两种，函数对象是将一个封装了功能的对象重载括号运算符后使用方法和函数统一的一个对象，
因此他们的核心区别是，作为对象函数对象可以贯穿于原函数的整个生命周期，而函数指针只能一次一次的原子化调用*/
template <typename T> 
void Vector<T>::traverse ( void ( *visit ) ( T& ) ) //借助函数指针机制
    { 
        for ( int i = 0; i < _size; i++ ) 
        visit ( _elem[i] ); 
    } //遍历向量

template <typename T> 
template <typename VST> //元素类型、操作器
void Vector<T>::traverse ( VST& visit ) //借助函数对象机制
    { 
        for ( int i = 0; i < _size; i++ ) 
        visit ( _elem[i] ); 
    } //遍历向量


//遍历函数的实例：全部自+1操作——Increase<T>()
template <typename T> 
struct Increase //操作器：递增一个T类对象
    { virtual void operator() ( T& e ) { e++; } }; //重载括号操作符；假设T可直接递增或已重载++操作符

template <typename T> 
void increase ( Vector<T> & V ) //统一递增向量中的各元素，传入一个T引用类型的参数V
    { V.traverse ( Increase<T>() ); } //以操作器Increase<T>()为基本操作，调用向量V中的traverse方法运行遍历

    
//判断是否有序，返回向量中逆序相邻元素对的总数；
template <typename T> 
int Vector<T>::disordered() const { //被声明为const意味着函数不能修改类的成员变量。
    int n = 0; //计数器
    for ( int i = 1; i < _size; i++ ) //逐一检查“_size - 1”对相邻元素
        if ( _elem[i - 1] > _elem[i] ) n++; //逆序则计数
    return n; //n = 0表示升序有序
}

//有序向量重复元素剔除算法（低效版）
template <typename T> 
int Vector<T>::uniquify() { 
    int oldSize = _size;     int i = 1; //当前比对元素的秩，和起始于首元素
    while ( i < _size ) //从前向后，逐一比对各对相邻元素
        _elem[i - 1] == _elem[i] ? remove ( i ) : i++; //若雷同，则删除后者；否则，转至后一元素
    return oldSize - _size; //向量规模变化量，即被删除元素总数
}


//有序向量重复元素剔除算法（高效版）
 template <typename T> 
 int Vector<T>::uniquify1B() { 
    Rank i = 0, j = 0; //各对互异“相邻”元素的秩
    while ( ++j < _size ) //逐一扫描，直至末元素，j置0先j++再判断合法范围的目的不知道是什么
        if ( _elem[i] != _elem[j] ) //跳过雷同者
            _elem[++i] = _elem[j]; //发现不同元素时，向前移至i右侧的位置
            _size = ++i; //直接截除尾部多余元素
            shrink(); 
    return j-i; //向量规模变化量，即被删除元素总数
}


//查找的统一接口
/*鉴于有序查找的算法多样且各具特点，为便于测试，这里的接口不妨随机选择查找算法。
实际应用中可根据问题的特点具体确定，并做适当微调。*/
template <typename T> //在有序向量的区间[lo, hi)内，确定不大于e的最后一个节点的秩
Rank Vector<T>::search ( T const& e, Rank lo, Rank hi ) const { //assert: 0 <= lo < hi <= _size
    return ( rand() % 2 ) ? binSearch ( _elem, e, lo, hi ) : fibSearch ( _elem, e, lo, hi );//按各50%的概率随机使用二分查找或Fibonacci查找
}



//二分查找版本A
template <typename T> 
static Rank binSearchA ( T* A, T const& e, Rank lo, Ranßk hi ) {// 在有序向量的区间[lo, hi)内查找元素e，0 <= lo <= hi <= _size，都是半开区间，
while ( lo < hi ) { //每步迭代可能要做两次比较判断，有三个分支
    Rank mi = ( lo + hi ) >> 1; //以中点为轴点，半开区间中，如[0,4),中轴是2，[3,6)，位移运算向下取整。避免溢出。另外，向下取整同时左取中值又取中值+1也可以保证在左右两边搜索时不会漏掉某个元素。
        if ( e < A[mi] ) 
            hi = mi; //深入前半段[lo, mi)继续查找
        else if ( A[mi] < e ) 
            lo = mi + 1; //深入后半段(mi, hi)继续查找
        else return mi; //在mi处命中
} //成功查找可以提前终止
return -1; //在while循环中，没有出现等于的情况，并且lo不<hi了，查找失败
} //有多个命中元素时，不能保证返回秩最大者；查找失败时，简单地返回-1，而不能指示失败的位置


 #include "..\fibonacci\Fib.h" //引入Fib数列类

// Fibonacci查找算法（版本A）：在有序向量的区间[lo, hi)内查找元素e，0 <= lo <= hi <= _size
template <typename T> 
static Rank fibSearch ( T* A, T const& e, Rank lo, Rank hi ) {
    Fib fib ( hi - lo ); //用O(log_phi(n = hi - lo)）时间创建Fib数列
    while ( lo < hi ) { //每步迭代可能要做两次比较判断，有三个分支
        while ( hi - lo < fib.get() ) 
            fib.prev(); //通过向前顺序查找（分摊O(1)）——至多迭代几次？
            Rank mi = lo + fib.get() - 1; //确定形如Fib(k) - 1的轴点
            if ( e < A[mi] ) 
                hi = mi; //深入前半段[lo, mi)继续查找
            else if ( A[mi] < e ) 
                lo = mi + 1; //深入后半段(mi, hi)继续查找
    else return mi; //在mi处命中
}   //成功查找可以提前终止
return -1; //查找失败
} //有多个命中元素时，不能保证返回秩最大者；失败时，简单地返回-1，而不能指示失败的位置
//（跳过，暂时不研究了）


 // 二分查找算法（版本B）：在有序向量的区间[lo, hi)内查找元素e，0 <= lo <= hi <= _size
template <typename T> 
static Rank binSearchB ( T* A, T const& e, Rank lo, Rank hi ) {
while ( 1 < hi - lo ) { //每步迭代仅需做一次比较判断，有两个分支；成功查找不能提前终止
    Rank mi = ( lo + hi ) >> 1; //以中点为轴点
    ( e < A[mi] ) ? hi = mi : lo = mi; //经比较后确定深入[lo, mi)或[mi, hi)
    } //出口时hi = lo + 1，查找区间仅含一个元素A[lo]
return ( e == A[lo] ) ? lo : -1 ; //查找成功时返回对应的秩；否则统一返回-1
} //有多个命中元素时，不能保证返回秩最大者；查找失败时，简单地返回-1，而不能指示失败的位置


// 二分查找算法（版本C）：在有序向量的区间[lo, hi)内查找元素e，0 <= lo <= hi <= _size
template <typename T> static Rank binSearch ( T* A, T const& e, Rank lo, Rank hi ) {
while ( lo < hi ) { //每步迭代仅需做一次比较判断，有两个分支
    Rank mi = ( lo + hi ) >> 1; //以中点为轴点
    ( e < A[mi] ) ? hi = mi : lo = mi + 1; //经比较后确定深入[lo, mi)或(mi, hi)，mi都会被刨掉，
} //成功查找不能提前终止
return --lo; //循环结束时，lo为大于e的元素的最小秩，故lo - 1即不大于e的元素的最大秩
} //有多个命中元素时，总能保证返回秩最大者；查找失败时，能够返回失败的位置


template <typename T> void Vector<T>::sort ( Rank lo, Rank hi ) { //向量匙间[lo, hi)排序
switch ( rand() % 5 ) { //随机选叏排序算法。可根据具体问题癿特点灵活选叏戒扩充
    case 1: bubbleSort ( lo, hi ); break; //起泡排序
    case 2: selectionSort ( lo, hi ); break; //选择排序（习题）
    case 3: mergeSort ( lo, hi ); break; //弻幵排序
    case 4: heapSort ( lo, hi ); break; //堆排序（稍后介绍）
    default: quickSort ( lo, hi ); break; //快速排序（稍后介绍）
    }
}


template <typename T> //向量的起泡排序
void Vector<T>::bubbleSort ( Rank lo, Rank hi ) //assert: 0 <= lo < hi <= size
{ while ( !bubble ( lo, hi-- ) ); } //逐趟做扫描交换，直至全序


template <typename T> 
bool Vector<T>::bubble ( Rank lo, Rank hi ) { //一趟扫描交换
    bool sorted = true; //整体有序标志
    while ( ++lo < hi ) //自左向右，逐一检查各对相邻元素
        if ( _elem[lo - 1] > _elem[lo] ) { //若逆序，则
            sorted = false; //意味着尚未整体有序，需要通过交换使局部有序
            swap ( _elem[lo - 1], _elem[lo] ); 
    }
return sorted; //返回有序标志
}


//归并排序的框架：
/*通过递归的方式，深入最里层将向量分割成两个最小长度元素，然后从短到长依次将不同的两个向量归并再归并。
从而也实现了归并子列总是有序，毕竟只有有序的两个子列才可以进行归并*/
template <typename T> //向量归并排序
void Vector<T>::mergeSort ( Rank lo, Rank hi ) { //0 <= lo < hi <= size
    if ( hi - lo < 2 ) return; //单元素区间自然有序，否则...
    int mi = ( lo + hi ) >> 1; mergeSort ( lo, mi ); mergeSort ( mi, hi ); //以中点为界分别排序
    merge ( lo, mi, hi ); //归并：
}

template <typename T> //有序向量的归并
void Vector<T>::merge ( Rank lo, Rank mi, Rank hi ) { //各自有序的子向量[lo, mi)和[mi, hi)
    T* A = _elem + lo; //合并后的向量A[0, hi - lo) = _elem[lo, hi)，于是通过原始向量的地址，将lo位置作为向量A的首地址
    int lb = mi - lo; T* B = new T[lb]; //前子向量B[0, lb) = _elem[lo, mi)
    for ( Rank i = 0; i < lb; B[i] = A[i++] ); //复制前子向量
    int lc = hi - mi; T* C = _elem + mi; //后子向量C[0, lc) = _elem[mi, hi)
    for ( Rank i = 0, j = 0, k = 0; ( j < lb ) || ( k < lc ); ) { //B[j]和C[k]中的小者续至A末尾
        if ( ( j < lb ) && ( ! ( k < lc ) || ( B[j] <= C[k] ) ) ) A[i++] = B[j++];//当B的头部小时，B的头部进A；或者当C子列空了并且B子列没空时，B的头部继续进A
        if ( ( k < lc ) && ( ! ( j < lb ) || ( C[k] < B[j] ) ) ) A[i++] = C[k++];
 }
 delete [] B; //释放临时空间B
} //归并后得到完整的有序向量[lo, hi)
