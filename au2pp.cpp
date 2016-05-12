#include <bits/stdc++.h>
#define REP(i,n) for(int i=0; i<n; i++)
#define FOR(i,n) for(int i=1; i<=n; i++)

#include "academic_api.hpp"

namespace AU2PP{

    const bool cmp_by_Id(const Paper& i, const Paper& j){
        return i.Id < j.Id;
    }


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

    void f21(Paper paper, vector<Paper> &paper_ref_ed){
        paper_ref_ed = getEntities(string("RId=") + to_string(paper.Id), _ID, paper.CC>=5000); /// Papers that refers to Id
    }
    void f22(Paper paper, vector<Paper> &paper_au_af){
        if(paper.AA.size()){
            string expr = string("AA.AuId=") + to_string(paper.AA[0].AuId);
            for(int i = 1; i < paper.AA.size(); ++i){
                expr = "OR(AA.AuId=" + to_string(paper.AA[i].AuId) + string(",") + expr + string(")");
            }
            expr = string("Composite(") + expr + string(")");
            paper_au_af = getEntities(expr, _AA_AUID | _AA_AFID, true);
        }
    }
    void f2(LL au_id, LL id, Paper &paper, vector<Paper> &paper_au_af, vector<Paper> &paper_ref_ed){
        paper = getEntities(string("Id=") + to_string(id), _ID | _F_FID | _C_CID | _J_JID | _AA_AUID | _AA_AFID | _CC)[0];
        thread t21(f21, paper, ref(paper_ref_ed));
        thread t22(f22, paper, ref(paper_au_af));
        t21.join();
        t22.join();
    }
    void f3(LL au_id, LL id, vector<Paper> &paper_by_au){
        paper_by_au = getEntities(string("Composite(AA.AuId=") + to_string(au_id) + string(")"), _ID | _F_FID | _J_JID | _C_CID | _AA_AUID | _RID | _AA_AFID);
    }
    vector<Path> au2pp(LL au_id, LL id){
        vector<Paper> paper_ref_ed; /// Papers that refers to Id
        Paper paper ; // Id paper
        vector<Paper> paper_by_au; // Papers by Au
        
        vector<Paper> paper_au_af;

        thread t2(f2, au_id, id, ref(paper), ref(paper_au_af), ref(paper_ref_ed));
        thread t3(f3, au_id, id, ref(paper_by_au));
        t2.join();
        t3.join();
        
        vector<vector<LL> > ret;
        au2pp_1hop(au_id, id, ret, paper);
        au2pp_2hop(au_id, id, ret, paper_by_au, paper_ref_ed);
        au2pp_3hop(au_id, id, ret, paper_by_au, paper, paper_ref_ed, paper_au_af);
        return ret;
    }

}

using namespace AU2PP;

#ifndef PRODUCT
int main(){
    freopen("in.json","r",stdin);
    freopen("ans.json","w",stdout);
    LL auid, id;
    scanf("%lld%lld", &auid, &id);
    print_ans(au2pp(auid, id));
    return 0;
}
#endif
