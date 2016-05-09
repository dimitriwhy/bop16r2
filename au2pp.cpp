#include <bits/stdc++.h>
#define REP(i,n) for(int i=0; i<n; i++)
#define FOR(i,n) for(int i=1; i<=n; i++)

#include "academic_api.hpp"

const bool cmp_by_Id(const Paper& i, const Paper& j){
    return i.Id < j.Id;
}

Path get_path(int cnt, ...){
    va_list args;
    Path ret;
    va_start(args, cnt);
    while(cnt--){
        LL tmp;
        ret.push_back(tmp=va_arg(args, LL));
    }
    va_end(args);
    return ret;
}
typedef vector<LL> Path;
namespace AU2PP{


/*------------------------Author To Paper--------------------------------*/



    void au2pp_1hop(LL au_id, LL id, vector<Path> &ret, Paper &paper){
        for(auto &x : paper.AA){
            if(x.AuId == au_id){
                ret.push_back(get_path(2, au_id, id));
                break;
            }
        }
    }

    void au2pp_2hop(LL au_id, LL id, vector<Path> &ret, vector<Paper> &paper_by_au, vector<Paper> &paper_ref_ed){

        sort(paper_by_au.begin(), paper_by_au.end(), cmp_by_Id);
        sort(paper_ref_ed.begin(), paper_ref_ed.end(), cmp_by_Id);

        int p = 0, n = paper_by_au.size(), m = paper_ref_ed.size();
        for(int i = 0; i < n; i++){
            while(p < m && paper_ref_ed[p].Id < paper_by_au[i].Id)
                p++;
            if(p >= m)
                break;
            if(paper_by_au[i].Id == paper_ref_ed[p].Id){
                ret.push_back(get_path(3, au_id, paper_ref_ed[p].Id, id));
            }
        }
    }

    void au2pp_3hop(LL au_id, LL id, vector<Path> &ret, vector<Paper> &paper_by_au, Paper &paper, vector<Paper> &paper_ref_ed, vector<Paper> &paper_au_af){
        sort(paper_by_au.begin(), paper_by_au.end(), cmp_by_Id);

        int n = paper_by_au.size();
        unordered_set<LL> au_af_au_id; // Afs in each path(Au, Af, Au, Id) must be distinct
        unordered_map<LL,vector<LL> > au_id_id_id;
        for(int i = 0; i < n; i++){
        
            //Au -> Id -> F -> Id
            for(auto x : paper_by_au[i].F){
                for(auto y : paper.F){
                    if(x.FId == y.FId){
                        ret.push_back(get_path(4, au_id, paper_by_au[i].Id, x.FId, id));
                    }
                }
            }
        
            //Au -> Id -> C -> Id
            if(paper_by_au[i].C == paper.C){
                ret.push_back(get_path(4, au_id, paper_by_au[i].Id, paper.C.CId, id));
            }
            //Au -> Id -> J -> Id
            if(paper_by_au[i].J == paper.J){
                ret.push_back(get_path(4, au_id, paper_by_au[i].Id, paper.J.JId, id));
            }

            //Au -> Id -> Au -> Id
            for(auto x : paper_by_au[i].AA){
                for(auto y : paper.AA){
                    if(x.AuId == y.AuId){
                        ret.push_back(get_path(4, au_id, paper_by_au[i].Id, x.AuId, id));
                    }
                }
            }

            //Au -> Id -> Id -> Id
            for(auto &y : paper_by_au[i].RId){
                au_id_id_id[y].push_back(paper_by_au[i].Id);
            }
    
        }

        //Au -> Id -> Id -> Id
        for(auto &p : paper_ref_ed){
            if(au_id_id_id.count(p.Id)){
                for(auto &x : au_id_id_id[p.Id])
                    ret.push_back(get_path(4, au_id, x, p.Id, id));
            }
        }        

        //Au -> Af -> Au -> Id
        unordered_set<LL> au;
        for(auto &author : paper.AA)
            au.insert(author.AuId);
        unordered_set<LL> af;
        set<pair<long long, long long> > af_au;
        for(int i = 0; i < paper_by_au.size(); ++i){
            for(auto &author : paper_by_au[i].AA){
                if(author.AuId == au_id && author.AfId != -1)
                    af.insert(author.AfId);
            }
        }
        for(int i = 0; i < paper_au_af.size(); i++){
            for(auto &author : paper_au_af[i].AA)
                if(au.count(author.AuId) && af.count(author.AfId) && !af_au.count(make_pair(author.AfId,author.AuId))){
                    ret.push_back(get_path(4, au_id, author.AfId, author.AuId, id));
                    af_au.insert(make_pair(author.AfId, author.AuId));
                }
        }
    }

    void f1(LL au_id, LL id, vector<Paper> &paper_ref_ed){
        paper_ref_ed = getEntities(string("RId=") + to_string(id), _ID); /// Papers that refers to Id
    }
    void f2(LL au_id, LL id, Paper &paper, vector<Paper> &paper_au_af){
        paper = getEntities(string("Id=") + to_string(id), _F_FID | _C_CID | _J_JID | _AA_AUID | _AA_AFID)[0];
        if(paper.AA.size()){
            string expr = string("AA.AuId=") + to_string(paper.AA[0].AuId);
            for(int i = 1; i < paper.AA.size(); ++i){
                expr = "OR(AA.AuId=" + to_string(paper.AA[i].AuId) + string(",") + expr + string(")");
            }
            expr = string("Composite(") + expr + string(")");
            paper_au_af = getEntities(expr, _AA_AUID | _AA_AFID, true);
        }
    }
    void f3(LL au_id, LL id, vector<Paper> &paper_by_au){
        paper_by_au = getEntities(string("Composite(AA.AuId=") + to_string(au_id) + string(")"), _ID | _F_FID | _J_JID | _C_CID | _AA_AUID | _RID | _AA_AFID);
    }
    vector<Path> au2pp(LL au_id, LL id){
        vector<Paper> paper_ref_ed; /// Papers that refers to Id
        Paper paper ; // Id paper
        vector<Paper> paper_by_au; // Papers by Au
        
        vector<Paper> paper_au_af;

        thread t1(f1, au_id, id, ref(paper_ref_ed));
        thread t2(f2, au_id, id, ref(paper), ref(paper_au_af));
        thread t3(f3, au_id, id, ref(paper_by_au));
        t1.join();
        t2.join();
        t3.join();
        
        vector<vector<LL> > ret;
        au2pp_1hop(au_id, id, ret, paper);
        au2pp_2hop(au_id, id, ret, paper_by_au, paper_ref_ed);
        au2pp_3hop(au_id, id, ret, paper_by_au, paper, paper_ref_ed, paper_au_af);
        return ret;
    }

}
namespace PP2AU{
/*------------------------Paper To Author------------------------------*/

    void pp2au_1hop(LL id, LL au_id, vector<Path> &ret, Paper &paper){
        for(auto x : paper.AA){
            if(x.AuId == au_id){
                ret.push_back(get_path(2, id, au_id));
                break;
            }
        }
        return;
    }
    
    void pp2au_2hop(LL id, LL au_id, vector<Path> &ret, vector<Paper> &paper_by_au, Paper &paper){
        vector<LL> paper_adj = paper.RId;

        sort(paper_by_au.begin(), paper_by_au.end(), cmp_by_Id);
        sort(paper_adj.begin(), paper_adj.end());

        int p = 0, n = paper_by_au.size(), m = paper_adj.size();
        for(int i = 0; i < n; i++){
            while(p < m && paper_adj[p] < paper_by_au[i].Id)
                p++;
            if(p >= m)
                break;
            if(paper_by_au[i].Id == paper_adj[p]){
                ret.push_back(get_path(3, id, paper_adj[p], au_id));
            }
        }
    }


    void pp2au_3hop(LL id, LL au_id, vector<Path> &ret, vector<Paper> &paper_by_au, Paper &paper, vector<Paper> &paper_ref_ing2, vector<Paper> &paper_au_af){

        sort(paper_by_au.begin(), paper_by_au.end(), cmp_by_Id);

        vector<LL> &paper_ref_ing = paper.RId;

        int n = paper_by_au.size();
        unordered_map<LL,vector<LL> > au_id_id_id;
        for(int i = 0; i < n; i++){
 
            //Id -> F -> Id -> Au
            for(auto x : paper_by_au[i].F){
                for(auto y : paper.F){
                    if(x.FId == y.FId){
                        ret.push_back(get_path(4, id, x.FId, paper_by_au[i].Id, au_id));
                    }
                }
            }
        
            //Id -> C -> Id -> Au
            if(paper_by_au[i].C == paper.C){
                ret.push_back(get_path(4, id, paper.C.CId, paper_by_au[i].Id, au_id));
            }
            //Id -> J -> Id -> Au
            if(paper_by_au[i].J == paper.J){
                ret.push_back(get_path(4, id, paper.J.JId, paper_by_au[i].Id, au_id));
            }

        }

        //Id -> Id -> Id -> Au
        unordered_set<LL> id_id_id_au; //(y)
        for(Paper &p : paper_by_au){
            id_id_id_au.insert(p.Id);
        }
        for(Paper &x : paper_ref_ing2){
            for(auto &y : x.RId){
                if(id_id_id_au.count(y)){
                    ret.push_back(get_path(4, id, x.Id, y, au_id));
                }
            }
        }
        
        //Id -> Au -> Af -> Au
        unordered_set<LL> au;
        for(auto &author : paper.AA)
            au.insert(author.AuId);
        unordered_set<LL> af;
        set<pair<long long, long long> > af_au;
        for(int i = 0; i < paper_by_au.size(); ++i){
            for(auto &author : paper_by_au[i].AA){
                if(author.AuId == au_id && author.AfId != -1)
                    af.insert(author.AfId);
            }
        }
        for(int i = 0; i < paper_au_af.size(); i++){
            for(auto &author : paper_au_af[i].AA)
                if(au.count(author.AuId) && af.count(author.AfId) && !af_au.count(make_pair(author.AfId,author.AuId))){
                    ret.push_back(get_path(4, id, author.AuId, author.AfId, au_id));
                    af_au.insert(make_pair(author.AfId, author.AuId));
                }
        }
    
    }

    void f1(LL id, LL au_id, vector<Paper> &paper_by_au){
        paper_by_au = getEntities(string("Composite(AA.AuId=") + to_string(au_id) + string(")"), _ID | _F_FID | _J_JID | _C_CID | _AA_AUID); // Papers by Au
    }
    void f21(vector<LL> paper_ref_ing, vector<Paper> &paper_ref_ing2){
        if(paper_ref_ing.size() > 0){
            string expr = string("Id=") + to_string(paper_ref_ing[0]);
            for(int i = 1; i < paper_ref_ing.size(); i++)
                expr = string("OR(Id=") + to_string(paper_ref_ing[i]) + string(",") + expr + string(")");
            paper_ref_ing2 = getEntities(expr, _RID | _ID);
        }
    }
    void f22(vector<Author> AA, vector<Paper> &paper_au_af){
        if(AA.size()){
            string expr = string("AA.AuId=") + to_string(AA[0].AuId);
            for(int i = 1; i < AA.size(); ++i){
                expr = "OR(AA.AuId=" + to_string(AA[i].AuId) + string(",") + expr + string(")");
            }
            expr = string("Composite(") + expr + string(")");
            paper_au_af = getEntities(expr, _AA_AUID | _AA_AFID);
        }
    }
    void f2(LL id, LL au_id, Paper &paper, vector<LL> &paper_ref_ing, vector<Paper> &paper_ref_ing2, vector<Paper> &paper_au_af){
        paper = getEntities(string("Id=") + to_string(id), _F_FID | _C_CID | _J_JID | _AA_AUID)[0]; // Id paper
        paper_ref_ing = paper.RId;
        thread t21(f21, paper_ref_ing, ref(paper_ref_ing2));
        thread t22(f22, paper.AA, ref(paper_au_af));
        t21.join();
        t22.join();
    }
    vector<Path> pp2au(LL id, LL au_id){
        vector<Paper> paper_by_au; // Papers by Au
        Paper paper; // Id paper
        vector<LL> paper_ref_ing;
        vector<Paper> paper_ref_ing2;
        vector<Paper> paper_au_af;
        thread t1(f1, id, au_id, ref(paper_by_au));
        thread t2(f2, id, au_id, ref(paper), ref(paper_ref_ing), ref(paper_ref_ing2), ref(paper_au_af));
        t1.join();
        t2.join();
        /*
        if(paper_ref_ing.size() > 0){
            string expr = string("Id=") + to_string(paper_ref_ing[0]);
            for(int i = 1; i < paper_ref_ing.size(); i++)
                expr = string("OR(Id=") + to_string(paper_ref_ing[i]) + string(",") + expr + string(")");
            paper_ref_ing2 = getEntities(expr, _RID | _ID);
        }

        if(paper.AA.size()){
            string expr = string("AA.AuId=") + to_string(paper.AA[0].AuId);
            for(int i = 1; i < paper.AA.size(); ++i){
                expr = "OR(AA.AuId=" + to_string(paper.AA[i].AuId) + string(",") + expr + string(")");
            }
            expr = string("Composite(") + expr + string(")");
            paper_au_af = getEntities(expr, _AA_AUID | _AA_AFID);
        }
        */
        vector<Path> ret;
        pp2au_1hop(id, au_id, ret, paper);
        pp2au_2hop(id, au_id, ret, paper_by_au, paper);
        pp2au_3hop(id, au_id, ret, paper_by_au, paper, paper_ref_ing2, paper_au_af);
        return ret;
    }

}
using namespace AU2PP;

#ifndef PRODUCT
int main(){
#ifdef QWERTIER
    freopen("in","r",stdin);
#endif
    clock_t ct0, ct1; 
    struct tms tms0, tms1;
    
    ct0 = times (&tms0);
    print_ans(au2pp(
      1982462162ll, 
      2122841972ll));
    ct1 = times (&tms1);
    
    printf("%lld\n", (LL)sysconf(_SC_CLK_TCK));
    printf("%f %f\n", (ct1-ct0) / (double)sysconf(_SC_CLK_TCK), ti);
    //printf("%f\n", (t2-t1) / (double)sysconf(_SC_CLK_TCK));
    //printf("%f\n", (t2-t1) / (double)CLOCKS_PER_SEC);
    return 0;
}
#endif
