#include <iostream>
#include <chrono>
#include <bits/stdc++.h>
using namespace std;

void sum_lin(int arr[1000000], int len) {
    int l,r;
    l=0;
    r=len;
    while (r>l and arr[l]+arr[r]!=1) {
        if (arr[l]+arr[r]>0) {
            r-=1;
        }
        else {
            l+=1;
        }
    }
}

int main() {
    int a[1000000]={0};
    int i,x;
    x=999999;
    auto begin=std::chrono::steady_clock::now();
    for (i=0;i<100;i++) {
        sum_lin(a,x);
    }
    auto end=std::chrono::steady_clock::now();
    auto time_span=std::chrono::duration_cast<std::chrono::nanoseconds>((end-begin));
    cout<<(time_span.count()/100)<<endl;
}