#include <bits/stdc++.h>
#define REP(i,n) for(int i=0; i<n; i++)
#define FOR(i,n) for(int i=1; i<=n; i++)

#define PRODUCT
#include "academic_api.hpp"
#include "au2au.cpp"
#include "au2pp.cpp"
#include "pp2pp.cpp"
#include "pp2au.cpp"
typedef long long LL;
using namespace std;

vector<Path> get_ans(LL id1, LL id2){
    if(!id1 || !id2){
        vector<Path> ret;
        return ret;
    }
    bool pp1 = false, pp2 = false;
    Paper p1, p2;
    string q1 = string("OR(Composite(AA.AuId=") + to_string(id1) + string("),AND(Y>0,Id=") + to_string(id1) + string("))");
    string q2 = string("OR(Composite(AA.AuId=") + to_string(id2) + string("),AND(Y>0,Id=") + to_string(id2) + string("))");
    vector<Paper> chk = getEntities(string("OR(") + q1 + string(",") + q2 + string(")"), _ID|_F_FID|_J_JID|_C_CID|_AA_AUID|_RID |_CC | _AA_AFID);
    vector<Paper> paper_by_au;
    for(auto &p : chk){
        if(p.Id == id1){
            p1 = p;
            pp1 = true;
        }
        if(p.Id == id2){
            p2 = p;
            pp2 = true;
        }
        int flag = 0;
        for(auto &author : p.AA)
            if(author.AuId == id1 || author.AuId == id2){
                flag = 1;
            }
        if(flag)
            paper_by_au.push_back(p);
    }
    if(pp1 && pp2)
        return pp2pp(id1, id2, p1, p2);
    else if(pp1)
        return PP2AU::pp2au(id1, id2, p1, paper_by_au);
    else if(pp2)
        return AU2PP::au2pp(id1, id2, paper_by_au, p2);
    else
        return au2au(id1, id2, chk);
}

int main(){
#ifdef QWERTIER
    freopen("in","r",stdin);
#endif
    clock_t ct0, ct1; 
    struct tms tms0, tms1;
    
    ct0 = times (&tms0);
    vector<Path> pp = get_ans(2195129696ll,2230315570ll);
    //print_ans(pp);
    ct1 = times (&tms1);
    cout<<pp.size() << ' ' << (ct1 - ct0) / double(sysconf(_SC_CLK_TCK)) << endl;
    return 0;
}
