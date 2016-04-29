#include <bits/stdc++.h>
#define REP(i,n) for(int i=0; i<n; i++)
#define FOR(i,n) for(int i=1; i<=n; i++)

#include "academic_api.h"
typedef long long LL;
using namespace std;

int main(){
#ifdef QWERTIER
  freopen("in","r",stdin);
#endif
  char *hStr = new char[100000000],*bStr = new char[100000000];
  vector<string> para;
  para.push_back(string("Id"));
  para.push_back(string("AA.AuId"));
  para.push_back(string("AA.AfId"));
  para.push_back(string("Ti"));
  LL t1 = clock();
  vector<Paper> entities = getEntities(string("RId=2143554828"),para) ;
  for(auto x : entities){
      //printf("%lld\n", x.Id);
      cout<<x.Ti<<endl;
      for(auto y : x.AA){
          //printf("%lld %lld\n", y.AuId, y.AfId);
      }
  }
  printf("%d\n",entities.size());
  LL t2 = clock();
  printf("%f\n",double(t2-t1)/CLOCKS_PER_SEC);
  return 0;
}
