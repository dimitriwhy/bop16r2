#include <bits/stdc++.h>
#define REP(i,n) for(int i=0; i<n; i++)
#define FOR(i,n) for(int i=1; i<=n; i++)

#include "academic_api.hpp"

namespace PP2AU{
    const bool cmp_by_Id(const Paper& i, const Paper& j){
        return i.Id < j.Id;
    }

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
                if(author.AuId == au_id && author.AfId != -1){
                    af.insert(author.AfId);
                }
            }
        }
        for(int i = 0; i < paper_au_af.size(); i++){
            for(auto &author : paper_au_af[i].AA){
                if(au.count(author.AuId) && af.count(author.AfId) && !af_au.count(make_pair(author.AfId,author.AuId))){
                    ret.push_back(get_path(4, id, author.AuId, author.AfId, au_id));
                    af_au.insert(make_pair(author.AfId, author.AuId));
                }
            }
        }

        //Id -> Au -> Id -> Au
        for(int i = 0; i < paper_au_af.size(); i++){
            for(auto &author : paper_au_af[i].AA)
                if(id_id_id_au.count(paper_au_af[i].Id) && au.count(author.AuId))
                    ret.push_back(get_path(4, id, author.AuId, paper_au_af[i].Id, au_id));
        }
    
    }

    void f1(LL id, LL au_id, vector<Paper> &paper_by_au){
        paper_by_au = getEntities(string("Composite(AA.AuId=") + to_string(au_id) + string(")"), _ID | _F_FID | _J_JID | _C_CID | _AA_AUID | _AA_AFID); // Papers by Au
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
            paper_au_af = getEntities(expr, _AA_AUID | _AA_AFID | _ID, true);
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
    vector<Path> pp2au(LL id, LL au_id,Paper paper, vector<Paper> paper_by_au){
        vector<LL> paper_ref_ing;
        vector<Paper> paper_ref_ing2;
        vector<Paper> paper_au_af;
        paper_ref_ing = paper.RId;
        thread t21(f21, paper_ref_ing, ref(paper_ref_ing2));
        thread t22(f22, paper.AA, ref(paper_au_af));
        t21.join();
        t22.join();

        vector<Path> ret;
        pp2au_1hop(id, au_id, ret, paper);
        pp2au_2hop(id, au_id, ret, paper_by_au, paper);
        pp2au_3hop(id, au_id, ret, paper_by_au, paper, paper_ref_ing2, paper_au_af);
        return ret;
    }

}
using namespace PP2AU;

#ifndef PRODUCT
int main(){
    freopen("in.json","r",stdin);
    freopen("ans.json","w",stdout);
    LL id,auid;
    scanf("%lld%lld",&id,&auid);
    //pp2au(id,auid);
    print_ans(pp2au(id,auid));
    return 0;
}
#endif
