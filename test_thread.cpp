#include <bits/stdc++.h>
#define REP(i,n) for(int i=0; i<n; i++)
#define FOR(i,n) for(int i=1; i<=n; i++)
#include "academic_api.hpp"

typedef long long LL;
using namespace std;
#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
 
void f1(int n)
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "Thread " << n << " executing\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
 
void f2(int& n)
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "Thread 2 executing\n";
        ++n;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void f3(){
    string exp1 = string("Id=2128635872");
    string exp2 = string("Id=2100837269");
    getEntities(exp1, _ID);
    getEntities(exp2, _ID);
}
void f4(){
    cout << "#1:" << endl;
    string exp1 = string("Id=2128635872");
    string exp2 = string("Id=2100837269");
    std::thread t1(getEntities, exp1, _ID);
    std::thread t2(getEntities, exp2, _ID);
    t1.join();
    t2.join();
}
int main(){
    /*
    int n = 0;
    std::thread t1; // t1 is not a thread
    std::thread t2(f1, n + 1); // pass by value
    std::thread t3(f2, std::ref(n)); // pass by reference
    std::thread t4(std::move(t3)); // t4 is now running f2(). t3 is no longer a thread 
    t2.join();
    t4.join();
    */
    curl_global_init(CURL_GLOBAL_ALL);
    f3();
    cout << "Finished" << endl;
    cout << ti <<endl;
    ti = 0;
    f4();
    cout << "Finished" << endl;
    cout << ti <<endl;
    return 0;
}
