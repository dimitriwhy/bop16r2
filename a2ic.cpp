#include <bits/stdc++.h>
#include "academic_api.hpp"
using namespace std;
void a2ihop1(vector <Path> & ans,vector <Paper> & ap,Paper & p,LL aid,LL id){
	for (int i = 0;i < ap.size();i++)
		if (ap[i].Id == id){
			Path tt;
			tt.push_back(aid);tt.push_back(id);
			ans.push_back(tt);
		}
}
void a2ihop2(vector <Path> & ans,vector <Paper> & ap,Paper &p,LL aid,LL id){
	for (int i = 0;i < ap.size();i++)
		for (int j = 0;j < ap[i].RId.size();j++)
			if (ap[i].RId[j] == id){
				Path tt;
				tt.push_back(aid);tt.push_back(ap[i].Id);tt.push_back(id);
				ans.push_back(tt);
			}
}
void a2ihop3(vector <Path> & ans,vector <Paper> & ap,Paper & p,LL aid,LL id){
	for (int i = 0;i < ap.size();i++){
		if (ap[i].J.JId != -1 && ap[i].J.JId == p.J.JId){
			Path tt;
			tt.push_back(aid);tt.push_back(ap[i].Id);tt.push_back(p.J.JId);tt.push_back(id);
			ans.push_back(tt);
		}
		if (ap[i].C.CId != -1 && ap[i].C.CId == p.C.CId){
			Path tt;
			tt.push_back(aid);tt.push_back(ap[i].Id);tt.push_back(p.C.CId);tt.push_back(id);
			ans.push_back(tt);
		}
		for (int j = 0;j < ap[i].F.size();j++)
			for (int k = 0;k < p.F.size();k++)
				if (ap[i].F[j].FId == p.F[k].FId){
					Path tt;
					tt.push_back(aid);tt.push_back(ap[i].Id);tt.push_back(p.F[k].FId);tt.push_back(id);
					ans.push_back(tt);
				}
		for (int j = 0;j < ap[i].AA.size();j++)
			for (int k = 0;k < p.AA.size();k++)
				if (ap[i].AA[j].AuId == p.AA[k].AuId){
					Path tt;
					tt.push_back(aid);tt.push_back(ap[i].Id);tt.push_back(p.AA[k].AuId);tt.push_back(id);
					ans.push_back(tt);
				}
	}
	vector <Paper> rp = getEntities(string("RId=") + to_string(id),_ID|_F_FID|_J_JID|_C_CID|_AA_AUID|_RID);
	unordered_map <LL,int> mp,maf;
	for (int i = 0;i < rp.size();i++)
		mp[rp[i].Id] = 1;
	for (int i = 0;i < ap.size();i++)
		for (int j = 0;j < ap[i].RId.size();j++)
			if (mp.count(ap[i].RId[j])){
				Path tt;
				tt.push_back(aid);tt.push_back(ap[i].Id);tt.push_back(ap[i].RId[j]);tt.push_back(id);
				ans.push_back(tt);
			}
	for (int i = 0;i < ap.size();i++)
		for (int j = 0;j < ap[i].AA.size();j++)
			if (ap[i].AA[j].AuId == aid)
				maf[ap[i].AA[j].AfId] = 1;
	for (int i = 0;i < p.AA.size();i++){
		vector <Paper> ppp = getEntities(string("Composite(AA.AuId=")+to_string(p.AA[i].AuId)+string(")"),_ID|_F_FID|_J_JID|_C_CID|_AA_AUID|_RID);
		for (int j = 0;j < ppp.size();j++)
			for (int k = 0;k < ppp[j].AA.size();k++)
				if ( ppp[j].AA[k].AfId == p.AA[i].AuId && maf.count(ppp[j].AA[k].AfId) ){
					Path tt;
					tt.push_back(aid);tt.push_back(ppp[j].AA[k].AfId);tt.push_back(p.AA[i].AuId);tt.push_back(id);
					ans.push_back(tt);
				}
	}
	
}
vector <Path> a2icheck(LL aid,LL id){
	vector <Path> ans;
	vector <Paper> ap = getEntities(string("Composite(AA.AuId=")+to_string(aid)+string(")"),_ID|_F_FID|_J_JID|_C_CID|_AA_AUID|_RID);
	vector <Paper> pp = getEntities(string("Id=") + to_string(id),_ID|_F_FID|_J_JID|_C_CID|_AA_AUID|_RID);
	a2ihop1(ans,ap,pp[0],aid,id);
	a2ihop2(ans,ap,pp[0],aid,id);
	a2ihop3(ans,ap,pp[0],aid,id);
	return ans;
}
int main(){
	freopen("ff.out","w",stdout);
	vector <Path> ff = a2icheck(2251253715LL,2180737804LL);
	for (int i = 0;i < ff.size();i++){
		printf("[");
		for (int j = 0;j < ff[i].size();j++)
			printf("%lld,",ff[i][j]);
		printf("],");
	}

	return 0;
}
