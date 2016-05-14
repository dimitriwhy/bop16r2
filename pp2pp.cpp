#include <bits/stdc++.h>
#include "academic_api.hpp"
using namespace std;
int pp_cmp(const Paper & a,const Paper & b){
	return a.Id < b.Id;
}
void ff1(LL id1,LL id2,vector <Paper> p1,vector <Paper> &fuck,int kk){
	string query = string("");
	int maxn = max((kk + 1) * 100,(int)p1[0].RId.size());
	for (int i = kk * 100;i < maxn;kk++){
		if (i < maxn - 1) query += string("OR(");
		query += string("Id=") + to_string(p1[0].RId[i]);
		if (i != maxn - 1) query += string(",");
		else for (int j = 0;j < maxn - 1;j++) query += string(")");
	}
	fuck = getEntities(query,_ID|_F_FID|_J_JID|_C_CID|_AA_AUID|_RID);
}
void f1(LL id1, LL id2, vector<Paper> p1, vector<Paper> &pp1){
    string query = string("");
	if (p1[0].RId.size() <= 100){
		for (int i = 0;i < p1[0].RId.size();i++){
			if (i < p1[0].RId.size() - 1) query += string("OR(");
			query += string("Id=") + to_string(p1[0].RId[i]);
			if (i != p1[0].RId.size() - 1) query += string(",");
			else 
				for (int j = 0;j < p1[0].RId.size() - 1;j ++) query += string(")");
		}
		if (p1[0].RId.size())
			pp1 = getEntities(query,_ID|_F_FID|_J_JID|_C_CID|_AA_AUID|_RID);
	}else {
		int tot = p1[0].RId.size()/100 + (p1[0].RId.size() %100 != 0);
		thread tt[tot];
		vector <vector <Paper> > fuck;
		fuck.resize(tot);
		for (int kk = 0;kk < tot;kk++)
			tt[kk] = thread(ff1,id1,id2,p1,ref(fuck[kk]),kk);
		for (int kk = 0;kk < tot;kk++){
			tt[kk].join();
			pp1.insert(pp1.begin(),fuck[kk].begin(),fuck[kk].end());
		}
	}
}
void f2(LL id1, LL id2, vector<Paper> p2, vector<Paper> &pp2){
    pp2 = getEntities(string("RId=")+to_string(id2),_ID|_F_FID|_J_JID|_C_CID|_AA_AUID, p2[0].CC>=5000);
}
vector <vector <long long> > pp2pp(long long id1,long long  id2, Paper p10, Paper p20){
	vector <vector <long long> > ans;

	vector <Paper> p1,p2;
    p1.push_back(p10);
    p2.push_back(p20);
	vector <Paper> pp2,pp1;
    thread t1(f1, id1, id2, p1,ref(pp1));
    thread t2(f2, id1, id2, p2,ref(pp2));
    t1.join();
    t2.join();

	//1-HOP
	for (int i = 0;i < p1[0].RId.size();i++)
		if (p1[0].RId[i] == id2){
			vector <long long> tt;
			tt.push_back(id1);tt.push_back(id2);
			ans.push_back(tt);
		}

	//2-HOP
	//p-p-p pp1 join
	for (int i = 0;i < pp1.size();i++)
		for (int j = 0;j < pp1[i].RId.size();j++)
			if (pp1[i].RId[j] == id2){
				vector <long long> tt;
				tt.push_back(id1),tt.push_back(pp1[i].Id),tt.push_back(id2);
				ans.push_back(tt);
			}
	//p-CJ-p
	if (p1[0].J.JId != -1 && p1[0].J.JId == p2[0].J.JId){
		vector <long long> tt;
		tt.push_back(id1),tt.push_back(p1[0].J.JId),tt.push_back(id2);
		ans.push_back(tt);
	}
	if (p1[0].C.CId != -1 && p1[0].C.CId == p2[0].C.CId){
		vector <long long > tt;
		tt.push_back(id1),tt.push_back(p1[0].C.CId),tt.push_back(id2);
		ans.push_back(tt);
	}
	//p-A-p
	for (int i = 0;i < p1[0].AA.size();i++)
		for (int j = 0;j < p2[0].AA.size();j++)
			if (p1[0].AA[i].AuId == p2[0].AA[j].AuId){
				vector <long long> tt;
				tt.push_back(id1),tt.push_back(p1[0].AA[i].AuId);tt.push_back(id2);
				ans.push_back(tt);
			}
	//p-F-p
	for (int i = 0;i < p1[0].F.size();i++)
		for (int j = 0;j < p2[0].F.size();j++)
			if (p1[0].F[i].FId == p2[0].F[j].FId){
				vector <long long> tt;
				tt.push_back(id1),tt.push_back(p1[0].F[i].FId);tt.push_back(id2);
				ans.push_back(tt);				
			}

	//3-HOP
	//p-p-p-p pp2.join
	sort(pp2.begin(),pp2.end(),pp_cmp);
	for (int i = 0;i < pp1.size();i++)
		for (int j = 0;j < pp1[i].RId.size();j++){
			int ll = 0,rr = pp2.size() - 1,mid;long long p = pp1[i].RId[j],res = -1;
			while (ll <= rr){
				mid = (ll+rr) >> 1;
				if (pp2[mid].Id < p)	ll = mid + 1;
				else if (pp2[mid].Id > p) rr = mid - 1;
				else {res = pp2[mid].Id;break;}
			}
			if (res != -1){
				vector <long long> tt;
				tt.push_back(id1),tt.push_back(pp1[i].Id),tt.push_back(res),tt.push_back(id2);
				ans.push_back(tt);
			}
		}
	//p-p-CJAF-p
	for (int i = 0;i < pp1.size();i++){
		if (pp1[i].C.CId != -1 && pp1[i].C.CId == p2[0].C.CId){
			vector <long long> tt;
			tt.push_back(id1),tt.push_back(pp1[i].Id),tt.push_back(pp1[i].C.CId),tt.push_back(id2);
			ans.push_back(tt);
		}
		if (pp1[i].J.JId != -1 && pp1[i].J.JId == p2[0].J.JId){
			vector <long long> tt;
			tt.push_back(id1),tt.push_back(pp1[i].Id),tt.push_back(pp1[i].J.JId),tt.push_back(id2);
			ans.push_back(tt);
		}
		for (int j = 0;j < pp1[i].AA.size();j++)
			for (int k = 0;k < p2[0].AA.size();k++)
				if (pp1[i].AA[j].AuId == p2[0].AA[k].AuId){
					vector <long long> tt;
					tt.push_back(id1),tt.push_back(pp1[i].Id),tt.push_back(pp1[i].AA[j].AuId),tt.push_back(id2);
					ans.push_back(tt);
				}
		for (int j = 0;j < pp1[i].F.size();j++)
			for (int k = 0;k < p2[0].F.size();k++)
				if (pp1[i].F[j].FId == p2[0].F[k].FId){
					vector <long long> tt;
					tt.push_back(id1),tt.push_back(pp1[i].Id),tt.push_back(pp1[i].F[j].FId),tt.push_back(id2);
					ans.push_back(tt);
				}		
	}
	//p-CJAF-p-p
	for (int i = 0;i < pp2.size();i++){
		if (p1[0].C.CId != -1 && p1[0].C.CId == pp2[i].C.CId){
			vector <long long> tt;
			tt.push_back(id1),tt.push_back(p1[0].C.CId),tt.push_back(pp2[i].Id),tt.push_back(id2);
			ans.push_back(tt);
		}
		if (p1[0].J.JId != -1 && p1[0].J.JId == pp2[i].J.JId){
			vector <long long> tt;
			tt.push_back(id1),tt.push_back(p1[0].J.JId),tt.push_back(pp2[i].Id),tt.push_back(id2);
			ans.push_back(tt);
		}
		for (int j = 0;j < p1[0].AA.size();j++)
			for (int k = 0;k < pp2[i].AA.size();k++)
				if (p1[0].AA[j].AuId == pp2[i].AA[k].AuId){
					vector <long long> tt;
					tt.push_back(id1),tt.push_back(p1[0].AA[j].AuId),tt.push_back(pp2[i].Id),tt.push_back(id2);
					ans.push_back(tt);
				}
		for (int j = 0;j < p1[0].F.size();j++)
			for (int k = 0;k < pp2[i].F.size();k++)
				if (p1[0].F[j].FId == pp2[i].F[k].FId){
					vector <long long> tt;
					tt.push_back(id1),tt.push_back(p1[0].F[j].FId),tt.push_back(pp2[i].Id),tt.push_back(id2);
					ans.push_back(tt);
				}
	}
	return ans;
}

#ifndef PRODUCT
int main(){
    freopen("in.json", "r", stdin);
    freopen("ans.json", "w", stdout);
    LL id1, id2;
    scanf("%lld%lld", &id1, &id2);
	//    print_ans(pp2pp(2026027122ll, 2056192258ll));
	return 0;
}
#endif
