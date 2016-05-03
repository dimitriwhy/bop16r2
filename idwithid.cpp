#include <bits/stdc++.h>
#include "academic_api.hpp"
using namespace std;
int pp_cmp(const Paper & a,const Paper & b){
	return a.Id < b.Id;
}
vector <vector <long long> > Id2Id(long long id1,long long  id2){
	vector <vector <long long> > ans;
	vector <Paper> p1 = getEntities(string("Id=") + to_string(id1),_ID|_F_FID|_J_JID|_C_CID|_AA_AUID|_RID);
	vector <Paper> p2 = getEntities(string("Id=") + to_string(id2),_ID|_F_FID|_J_JID|_C_CID|_AA_AUID|_RID);
	//1-HOP
	if (p1.size() != 1 || p2.size() != 1) return ans;
	for (int i = 0;i < p1[0].RId.size();i++)
		if (p1[0].RId[i] == id2){
			vector <long long> tt;
			tt.push_back(id1);tt.push_back(id2);
			ans.push_back(tt);
		}
	
	//2-HOP
	//p-p-p
	string query = string("");
	for (int i = 0;i < p1[0].RId.size();i++){
		if (i < p1[0].RId.size() - 1) query += string("OR(");
		query += string("Id=") + to_string(p1[0].RId[i]);
		if (i != p1[0].RId.size() - 1) query += string(",");
		else 
			for (int j = 0;j < p1[0].RId.size() - 1;j ++) query += string(")");
	}
	vector <Paper> pp1 = getEntities(query,_ID|_F_FID|_J_JID|_C_CID|_AA_AUID|_RID);
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
	//p-p-p-p
	vector <Paper> pp2 = getEntities(string("RId=")+to_string(id2),_ID|_F_FID|_J_JID|_C_CID|_AA_AUID|_RID);
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
    long long  t1 = clock();
	freopen("output.txt","w",stdout);
	vector <vector <long long> > ans = Id2Id(2140251882,2143554828);
	for (int i = 0;i < ans.size();i++){
		for (int j = 0;j < ans[i].size();j++)
			printf("%lld ",ans[i][j]);
		printf("\n");
	}
    long long t2 = clock();
    fprintf(stderr, "%f\n",double(t2-t1)/CLOCKS_PER_SEC);
	return 0;
}
#endif
