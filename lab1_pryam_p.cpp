#include <iostream>
#include <chrono>
#include <bits/stdc++.h>
using namespace std;

void pr_p(int arr[1000000], int len) {
    int i;
    for (i=0;i<=len;i++) {
        if (arr[i]==1) {
            cout<<" ";
        }
    }
}

int main() {
    int a[1000000]={0};
    int i,x;
    x=999999;
    auto begin=std::chrono::steady_clock::now();
    for (i=0;i<10000;i++) {
        pr_p(a,x);
    }
    auto end=std::chrono::steady_clock::now();
    auto time_span=std::chrono::duration_cast<std::chrono::nanoseconds>((end-begin));
    cout<<(time_span.count()/10000)<<endl;
}
