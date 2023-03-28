#include <iostream>
using namespace std;


//升序冒泡
void bubbleSort1A(int A[],int n){//引入待排序列，和问题规模
        int sorted = false;
        while (!sorted)
        {
            sorted = true;//假设已有序
            for (int i = 1; i < n; i++)//一趟
            {

                if (A[i-1]>A[i])//让i的前一个元素和递增i对应的位置元素依次比较，如果后面的小，那么互换
                {
                    swap(A[i-1],A[i]);
                    sorted = false;//出现局部逆序，假设毅然还未整体有序
                } 
            }
            n--;//，抛掉末尾已经就位的元素，问题规模-1
        }
    }

    void swap(int &a,int &b){
        int tmp = a;
        a = b;
        b = tmp;
    }



int main(void){

    cout<<"Reincarnation"<<endl;
    int A[5];

    for (int i = 0; i <5; i++)  //循环输入数组
    {
        cin>>A[i];
    }
    for (int i = 0; i <5; i++)  //循环输出数组
    {
        cout<<A[i]<<",";
    }
    cout<<endl;
    bubbleSort1A(A,5);
    for (int i = 0; i <5; i++)  //循环输出排序结果
    {
        cout<<A[i]<<",";
    }
    
cin.get();
return 0;
}