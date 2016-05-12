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
  vector<Paper> pp = getEntities(string("RId=2100837269"), _ID|_AA_AUID, true);
  FILE *fpp = fopen("id.txt", "w"), *fau = fopen("auid.txt","w");
  for(auto &p : pp){
      fprintf(fpp, "%lld\n", p.Id);
      for(auto &a : p.AA)
          fprintf(fau, "%lld\n", a.AuId);
  }
  return 0;
}
