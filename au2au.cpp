#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include "academic_api.hpp"
using namespace std;
using namespace Academic;
struct paper_id_less {
	bool operator()(const Paper &lhs, const Paper &rhs) const {
		return lhs.Id < rhs.Id;
	}
};
inline bool id_asc(Paper &i, Paper &j){
    return i.Id < j.Id;
}
vector<vector<long long>> au2au(long long AuId1, long long AuId2, vector<Paper> entities) {
	typedef long long id_type;
	vector<vector<id_type>> ret;
    /*
	stringstream expr;
	expr << "Composite(Or(AA.AuId=" << AuId1 << ",AA.AuId=" << AuId2 << "))";
	//just cheat the interface
	expr << "&orderby=Id:asc";
	vector<Paper> entities(getEntities(expr.str(), _ID | _AA_AUID | _AA_AFID | _RID));
    */
    sort(entities.begin(), entities.end(), id_asc);
	vector<id_type> currentpath(3);
	currentpath[0] = AuId1;
	currentpath[2] = AuId2;
	bool *by1(new bool[entities.size() + 5]()), *by2(new bool[entities.size() + 5]());
	vector<id_type> affiliations1, affiliations2;
	//1-hop
	//	impossible
	//2-hop
	//	Au->P->Au	or	Au->Af->Au
	for (vector<Paper>::iterator p(entities.begin()); p != entities.end(); ++p) {
		for (vector<Author>::iterator au(p->AA.begin()); au != p->AA.end(); ++au) {
			if (au->AuId == AuId1) {
				by1[p - entities.begin()] = true;
				if (au->AfId != -1) affiliations1.push_back(au->AfId);
			}
			if (au->AuId == AuId2) {
				by2[p - entities.begin()] = true;
				if (au->AfId != -1) affiliations2.push_back(au->AfId);
			}
			//Au->P->Au
			if (by1[p - entities.begin()] && by2[p - entities.begin()]) {
				currentpath[1] = p->Id;
				ret.push_back(currentpath);
				break;
			}
		}
	}
	sort(affiliations1.begin(), affiliations1.end());
	affiliations1.erase(unique(affiliations1.begin(), affiliations1.end()), affiliations1.end());
	sort(affiliations2.begin(), affiliations2.end());
	affiliations2.erase(unique(affiliations2.begin(), affiliations2.end()), affiliations2.end());
	for (vector<id_type>::iterator i(affiliations1.begin()); i != affiliations1.end(); ++i)
		for (vector<id_type>::iterator j(affiliations2.begin()); j != affiliations2.end(); ++j)
			//Au->Af->Au
			if (*i == *j) {
				currentpath[1] = *i;
				ret.push_back(currentpath);
				break;
			}
	//3-hop
	//	only	Au->P->P->Au
	currentpath.push_back(AuId2);
	Paper tmp;
	for (vector<Paper>::iterator p(entities.begin()); p != entities.end(); ++p)
		if (by1[p - entities.begin()])
			for (vector<id_type>::iterator r(p->RId.begin()); r != p->RId.end(); ++r) {
				tmp.Id = *r;
				vector<Paper>::iterator pos(lower_bound(entities.begin(), entities.end(), tmp, paper_id_less()));
				//Au->P->P->Au
				if (by2[pos - entities.begin()] && pos->Id == *r) {
					currentpath[1] = p->Id;
					currentpath[2] = *r;
					ret.push_back(currentpath);
				}
			}
	delete[] by1;
	delete[] by2;
	return ret;
}

#ifndef PRODUCT
#include <iostream>
#include <cstdlib>
int main() {
	freopen("au2au.txt", "w", stdout);
	clock_t time = clock();
	vector<vector<long long>> ret = au2au(1982462162, 1982462162);
	cout << (clock() - time) / (double)CLOCKS_PER_SEC << endl;
	cout << "[" << endl;
	bool fi(true);
	for (auto i : ret) {
		if (fi) fi = false;
		else cout << "," << endl;
		bool fj(true);
		cout << "[";
		for (auto j : i) {
			if (fj) fj = false;
			else cout << ",";
			cout << j;
		}
		cout << "]";
	}
	cout << endl << "]" << endl;
	//system("pause");
	return 0;
}
#endif
