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
vector<vector<long long> > au2au(long long AuId1, long long AuId2) {
	typedef long long id_type;
	vector<vector<id_type> > ret;
	stringstream expr;
	expr << "Composite(AA.AuId=" << AuId2 << ")";
	//just cheat the interface
	expr << "&orderby=Id:asc";
	vector<Paper> entities2 = getEntities(expr.str(), _ID | _AA_AUID | _AA_AFID);
	expr.str("");
	expr << "Composite(AA.AuId=" << AuId1 << ")";
	vector<Paper> entities1 = getEntities(expr.str(), _ID | _AA_AUID | _AA_AFID | _RID);
	//1-hop
	//	impossible
	//2-hop
	//	Au->P->Au	or	Au->Af->Au
	vector<id_type> currentpath(3);
	currentpath[0] = AuId1;
	currentpath[2] = AuId2;
	vector<id_type> affiliations1, affiliations2;
	for (vector<Paper>::iterator p(entities2.begin()); p != entities2.end(); ++p)
		for (vector<Author>::iterator au(p->AA.begin()); au != p->AA.end(); ++au)
			if (au->AuId == AuId2) {
				if (au->AfId != -1) affiliations2.push_back(au->AfId);
			}
			//Au->P->Au
			else if (au->AuId == AuId1) {
				currentpath[1] = p->Id;
				ret.push_back(currentpath);
			}
	sort(affiliations2.begin(), affiliations2.end());
	affiliations2.erase(unique(affiliations2.begin(), affiliations2.end()), affiliations2.end());
	for (vector<Paper>::iterator p(entities1.begin()); p != entities1.end(); ++p)
		for (vector<Author>::iterator au(p->AA.begin()); au != p->AA.end(); ++au)
			if (au->AuId == AuId1) {
				if (au->AfId != -1) affiliations1.push_back(au->AfId);
				break;
			}
	sort(affiliations1.begin(), affiliations1.end());
	affiliations1.erase(unique(affiliations1.begin(), affiliations1.end()), affiliations1.end());
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
    for (vector<Paper>::iterator p(entities1.begin()); p != entities1.end(); ++p)
		for (vector<id_type>::iterator r(p->RId.begin()); r != p->RId.end(); ++r) {
			tmp.Id = *r;
			vector<Paper>::iterator pos(lower_bound(entities2.begin(), entities2.end(), tmp, paper_id_less()));
			//Au->P->P->Au
			if (pos != entities2.end() && pos->Id == *r) {
				currentpath[1] = p->Id;
				currentpath[2] = *r;
				ret.push_back(currentpath);
			}
		}
    return ret;
}

/*
	a simple test
#include <iostream>
#include <cstdlib>
int main() {
	vector<vector<long long > > ret = au2au(2191458383ll, 2329307016ll);
	for (auto i : ret) {
		for (auto j : i) cout << j << ' ';
		cout << endl;
	}
	system("pause");
	return 0;
}
*/