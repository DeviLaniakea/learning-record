#include <iostream>
using namespace std;

void bubbleSort(int A[],int n){//引入待排序列，和问题规模
        int sorted = false;
        while (!sorted)
        {
            sorted = true;

            for (int i = 1; i < n; i++)//
            {
                if (A[i-1]<A[i])//让i的前一个元素和递增的i位置元素依次比较
                {
                   // swap(A[i-1],A[i]);
                    sorted = false;
                } 
            }
            n--;
        }
    }

    void swap(int &a,int &b){
        int tmp = a;
        a = b;
        b = tmp;
    }



int main(void){

    cout<<"Reincarnation"<<endl;
    int A[5]={3,7,5,2,6};
    for (int i = 0; i <5; i++)
    {
        cout<<A[i]<<endl;
    }
    bubbleSort(A,5);
        for (int i = 0; i <5; i++)
    {
        cout<<A[i];
    }
    
cin.get();
return 0;
}