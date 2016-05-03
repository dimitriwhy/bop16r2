#include <bits/stdc++.h>
#define REP(i,n) for(int i=0; i<n; i++)
#define FOR(i,n) for(int i=1; i<=n; i++)

#include "academic_api.hpp"


namespace AU2PP{
    const bool cmp_by_Id(const Paper& i, const Paper& j){
        return i.Id < j.Id;
    }

    typedef vector<LL> Path;

/*------------------------Author To Paper--------------------------------*/


    int c = 0;
    Path get_path(int cnt, ...){
        va_list args;
        Path ret;
        va_start(args, cnt);
        while(cnt--){
            LL tmp;
            ret.push_back(tmp=va_arg(args, LL));
            if(tmp==4294953344ll)
                printf("!!%d\n",c);
        }
        va_end(args);
        return ret;
    }

    void au2pp_1hop(LL au_id, LL id, vector<Path> &ret, Paper &paper){
        for(auto x : paper.AA){
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

    void au2pp_3hop(LL au_id, LL id, vector<Path> &ret, vector<Paper> &paper_by_au, Paper &paper, vector<Paper> &paper_ref_ed){
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
                        printf("%lld %lld\n", paper_by_au[i].Id, x.AuId);
                        ret.push_back(get_path(4, au_id, paper_by_au[i].Id, x.AuId, id));
                    }
                }
            }

            //Au -> Af -> Au -> Id
            for(auto x : paper_by_au[i].AA){
                au_af_au_id.insert(x.AfId);
            }

            //Au -> Id -> Id -> Id
            for(auto &x : paper_by_au[i].RId){
                au_id_id_id[x].push_back(paper_by_au[i].Id);
            }
    
        }

        //Au -> Af -> Au -> Id
        for(auto y: paper.AA){
            if(y.AfId != -1 && au_af_au_id.count(y.AfId))
                ret.push_back(get_path(4, au_id, y.AfId, y.AuId, id));
        }
    
        //Au -> Id -> Id -> Id
        for(auto &p : paper_ref_ed){
            if(au_id_id_id.count(p.Id)){
                for(auto &x : au_id_id_id[p.Id])
                    ret.push_back(get_path(4, au_id, x, p.Id, id));
            }
        }

    }

    vector<Path> au2pp(LL au_id, LL id){
        vector<Paper> paper_ref_ed = getEntities(string("RId=") + to_string(id), _ID); /// Papers that refers to Id
        Paper paper = getEntities(string("Id=") + to_string(id), _F_FID | _C_CID | _J_JID | _AA_AUID)[0]; // Id paper
        vector<Paper> paper_by_au = getEntities(string("Composite(AA.AuId=") + to_string(au_id) + string(")"), _ID | _F_FID | _J_JID | _C_CID | _AA_AUID); // Papers by Au

        vector<vector<LL> > ret;
        au2pp_1hop(au_id, id, ret, paper);
        au2pp_2hop(au_id, id, ret, paper_by_au, paper_ref_ed);
        au2pp_3hop(au_id, id, ret, paper_by_au, paper, paper_ref_ed);
        return ret;
    }



/*------------------------Paper To Author--------------------------------*/

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
                vector<LL> path;
                path.push_back(id);
                path.push_back(paper_adj[p]);
                path.push_back(au_id);
                ret.push_back(path);
            }
        }
    }

    void pp2au_3hop(LL id, LL au_id, vector<Path> &ret, vector<Paper> &paper_by_au, Paper &paper, vector<Paper> &paper_ref_ing2, vector<Paper> &paper_ref_ed){

        sort(paper_by_au.begin(), paper_by_au.end(), cmp_by_Id);

        vector<LL> &paper_ref_ing = paper.RId;

        int n = paper_by_au.size();
        unordered_set<LL> au_af_au_id; // Afs in each path(Au, Af, Au, Id) must be distinct
        unordered_map<LL,vector<LL> > au_id_id_id;
        for(int i = 0; i < n; i++){
 
            //Id -> F -> Id -> Au
            for(auto x : paper_by_au[i].F){
                for(auto y : paper.F){
                    if(x.FId == y.FId){
                        ret.push_back(get_path(4, au_id, paper_by_au[i].Id, x.FId, id));
                    }
                }
            }
        
            //Id -> C -> Id -> Au
            if(paper_by_au[i].C == paper.C){
                ret.push_back(get_path(4, au_id, paper_by_au[i].Id, paper.C.CId, id));
            }
            //Id -> J -> Id -> Au
            if(paper_by_au[i].J == paper.J){
                ret.push_back(get_path(4, au_id, paper_by_au[i].Id, paper.J.JId, id));
            }

            //Id -> Au -> Af -> Au
            for(auto x : paper_by_au[i].AA){
                for(auto y : paper.AA){
                    if(x.AuId == y.AuId){
                        ret.push_back(get_path(4, au_id, paper_by_au[i].Id, x.AuId, id));
                    }
                }
            }

        }

        //Id -> Au -> Af -> Au
        for(auto y: paper.AA){
            if(y.AfId != -1 && au_af_au_id.count(y.AfId))
                ret.push_back(get_path(4, au_id, y.AfId, y.AuId, id));
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
                    if(x.Id == -1)
                        exit(-1);
                }
            }
        }
    
    }

    vector<Path> pp2au(LL id, LL au_id){
        vector<Paper> paper_by_au = getEntities(string("Composite(AA.AuId=") + to_string(au_id) + string(")"), _ID | _F_FID | _J_JID | _C_CID | _AA_AUID); // Papers by Au
        Paper paper = getEntities(string("Id=") + to_string(id), _F_FID | _C_CID | _J_JID | _AA_AUID)[0]; // Id paper
        vector<Paper> paper_ref_ed = getEntities(string("RId=") + to_string(id), _ID); /// Papers that refers to Id
        vector<LL> &paper_ref_ing = paper.RId;
        vector<Paper> paper_ref_ing2;
        if(paper_ref_ing.size() > 0){
            string expr = string("Id=") + to_string(paper_ref_ing[0]);
            for(int i = 1; i < paper_ref_ing.size(); i++)
                expr = string("OR(Id=") + to_string(paper_ref_ing[i]) + string(",") + expr + string(")");
            paper_ref_ing2 = getEntities(expr, _RID | _ID);
        }

        vector<Path> ret;
        pp2au_1hop(id, au_id, ret, paper);
        pp2au_2hop(id, au_id, ret, paper_by_au, paper);
        pp2au_3hop(id, au_id, ret, paper_by_au, paper, paper_ref_ing2, paper_ref_ed);
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
    print_ans(au2pp(1982462162ll,2122841972ll));
    ct1 = times (&tms1);
    
    printf("%lld\n", (LL)sysconf(_SC_CLK_TCK));
    printf("%f %f\n", (ct1-ct0) / (double)sysconf(_SC_CLK_TCK), ti);
    //printf("%f\n", (t2-t1) / (double)sysconf(_SC_CLK_TCK));
    //printf("%f\n", (t2-t1) / (double)CLOCKS_PER_SEC);
    return 0;
}
#endif
