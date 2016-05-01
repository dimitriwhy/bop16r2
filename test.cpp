#include <bits/stdc++.h>
#define REP(i,n) for(int i=0; i<n; i++)
#define FOR(i,n) for(int i=1; i<=n; i++)

#include "academic_api.hpp"
typedef long long LL;
using namespace std;

int main(){
#ifdef QWERTIER
    freopen("in","r",stdin);
#endif
    char *hStr = new char[100000000],*bStr = new char[100000000];
    LL t1 = clock();
    vector<Paper> entities = getEntities(string("RId=2128635872"), _RID) ;
    for(auto x : entities){
        //printf("%lld\n", x.Id);
        for(auto y : x.AA){
            printf("%lld %lld\n", y.AuId, y.AfId);
        }
    }
    LL t2 = clock();
    printf("%f\n", ti);
    printf("%f\n",double(t2-t1)/CLOCKS_PER_SEC);
    return 0;
}
