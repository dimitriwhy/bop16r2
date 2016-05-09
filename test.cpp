#include <bits/stdc++.h>
#define REP(i,n) for(int i=0; i<n; i++)
#define FOR(i,n) for(int i=1; i<=n; i++)

#define PRODUCT
#include "academic_api.hpp"
#include "au2au.cpp"
#include "au2pp.cpp"
#include "idwithid.cpp"
typedef long long LL;
using namespace std;

int main(){
#ifdef QWERTIER
    freopen("in","r",stdin);
#endif
    clock_t ct0, ct1; 
    struct tms tms0, tms1;
    
    ct0 = times (&tms0);
    vector<Path> pp = Id2Id(2126125555ll,2060367530ll);
    //print_ans(pp);
    ct1 = times (&tms1);
    cout<<pp.size() << ' ' << (ct1 - ct0) / double(sysconf(_SC_CLK_TCK)) << endl;
    cout << ti << endl;
    return 0;
}
