#include <iostream>
#include <chrono>
#include <bits/stdc++.h>
using namespace std;

void bin_p(int arr[1000000], int len) {
    bool flag=false;
    int l=0;
    int r=len;
    int mid;

    while ((l<=r and flag!=true)) {
    mid=(l+r)/2;
    if (arr[mid]==1) { 
        flag = true;
    }
    if (arr[mid]>1) {
        r=mid-1;
    }
    else {
        l=mid+1;
    }
    }
}

int main() {
    int a[1000000]={0};
    int i,x;
    x=999999;
    auto begin=std::chrono::steady_clock::now();
    for (i=0;i<10000;i++) {
        bin_p(a,x);
    }
    auto end=std::chrono::steady_clock::now();
    auto time_span=std::chrono::duration_cast<std::chrono::nanoseconds>((end-begin));
    cout<<(time_span.count()/10000)<<endl;
}