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
    string exp1 = string("RId=2128635872&count=10000");
    //string exp2 = string("Id=2100837269");
    getEntities(exp1, _ID);
    //getEntities(exp2, _ID);
}
void f4(){
    /*
    cout << "#1:" << endl;
    string exp1 = string("RId=2128635872&count=1000");
    string exp2 = string("Id=2100837269");
    std::thread t1(getEntities, exp1, _ID);
    std::thread t2(getEntities, exp1, _ID);
    std::thread t3(getEntities, exp1, _ID);
    std::thread t4(getEntities, exp1, _ID);
    std::thread t5(getEntities, exp1, _ID);
    std::thread t6(getEntities, exp1, _ID);
    std::thread t7(getEntities, exp1, _ID);
    std::thread t8(getEntities, exp1, _ID);
    std::thread t9(getEntities, exp1, _ID);
    std::thread t10(getEntities, exp1, _ID);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();
    t9.join();
    t10.join();
    */
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

    /*
    curl_global_init(CURL_GLOBAL_ALL);
    clock_t ct0, ct1; 
    struct tms tms0, tms1;
    
    ct0 = times (&tms0);
    f3();
    ct1 = times (&tms1);
    cout << "Finished" << endl;
    cout << (ct1 - ct0) / (double)sysconf (_SC_CLK_TCK) << endl;

    ct0 = times(&tms0);
    f4();
    ct1 = times (&tms1);
    cout << "Finished" << endl;
    cout << (ct1 - ct0) / (double)sysconf (_SC_CLK_TCK) << endl;
    */
    curl_global_init(CURL_GLOBAL_ALL);
    clock_t ct0, ct1; 
    struct tms tms0, tms1;
    
    ct0 = times (&tms0);
    vector<Paper> pp = getEntities(string("RId=2106787323"), _ID, true);
    ct1 = times (&tms1);
    cout<<pp.size()<<endl;
    cout<<pp.size() << ' ' << (ct1 - ct0) / double(sysconf(_SC_CLK_TCK)) << endl;
    return 0;
}
