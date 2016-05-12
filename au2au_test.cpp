#include <bits/stdc++.h>
#include "academic_api.hpp"
using namespace std;
void a2a_hop2(vector <Paper> &p1,vector <Paper> &p2,vector <vector <LL> > & ans,LL aid1,LL aid2){
	unordered_map <LL,int> mm;
	for (int i = 0;i < p1.size();i++){
		for (int j = 0;j < p1[i].AA.size();j++)
			if (p1[i].AA[j].AuId == aid2){
				Path tt;
				tt.push_back(aid1);tt.push_back(p1[i].Id);tt.push_back(aid2);
				ans.push_back(tt);
			}
	}
	for (int i = 0;i < p1.size();i++)
		for (int j = 0;j < p1[i].AA.size();j++)
			if (p1[i].AA[j].AuId == aid1 && p1[i].AA[j].AfId != -1) mm[p1[i].AA[j].AfId] = 1;
	for (int i = 0;i < p2.size();i++)
		for (int j = 0;j < p2[i].AA.size();j++)
			if (p2[i].AA[j].AuId == aid2 && mm.count(p2[i].AA[j].AfId)){
				Path tt;
				tt.push_back(aid1);tt.push_back(p2[i].AA[j].AfId);tt.push_back(aid2);
				ans.push_back(tt);
			}
}
void a2a_hop3(vector <Paper> & p1,vector <Paper> & p2,vector <vector <LL> > & ans,LL aid1,LL aid2){
	unordered_map <LL,int> mm;
	for (int i = 0;i < p2.size();i++)
			mm[p2[i].Id] = 1;
	for (int i = 0;i < p1.size();i++)
		for (int j = 0;j < p1[i].RId.size();j++)
			if (mm.count(p1[i].RId[j])){
				Path tt;
				tt.push_back(aid1);tt.push_back(p1[i].Id);tt.push_back(p1[i].RId[j]);tt.push_back(aid2);
				ans.push_back(tt);
			}
}
vector <vector <LL> > a2acheck(LL aid1,LL aid2){
	vector <vector <LL> > ans;
	vector <Paper> p1 = getEntities(string("Composite(AA.AuId=")+to_string(aid1)+string(")"),_ID|_F_FID|_J_JID|_C_CID|_AA_AUID|_RID);
	vector <Paper> p2 = getEntities(string("Composite(AA.AuId=")+to_string(aid2)+string(")"),_ID|_F_FID|_J_JID|_C_CID|_AA_AUID|_RID);
	a2a_hop2(p1,p2,ans,aid1,aid2);
	a2a_hop3(p1,p2,ans,aid1,aid2);
	return ans;
}
int main(){
	freopen("ff.out","w",stdout);
	vector <Path> ff = a2acheck(57898110,2014261844);
	for (int i = 0;i < ff.size();i++){
		printf("[");
		for (int j = 0;j < ff[i].size();j++)
			printf("%lld,",ff[i][j]);
		printf("],");
	}
	return 0;
}
