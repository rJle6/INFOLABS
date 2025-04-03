#include <iostream>
#include <chrono>
#include <bits/stdc++.h>
using namespace std;

void sum_pr(int arr[100000], int len) {
    int i,j;
    for (i=0;i<=len;i++) {
        for (j=i+1;j<=len;j++) {
            if (arr[i]+arr[j]==1) {
                cout<<" ";
            }
        }
    }
}

int main() {
    int a[100000]={0};
    int i,x;
    x=99999;
    auto begin=std::chrono::steady_clock::now();
    for (i=0;i<1;i++) {
        sum_pr(a,x);
    }
    auto end=std::chrono::steady_clock::now();
    auto time_span=std::chrono::duration_cast<std::chrono::microseconds>((end-begin));
    cout<<(time_span.count())<<endl;
}