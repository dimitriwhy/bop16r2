#include <vector>
#include <string>
#include <algorithm>
#include "academic_api.hpp"
using namespace std;
using namespace Academic;
namespace test {
	using namespace std;
	using namespace Academic;
	typedef long long id_type;
	typedef vector<id_type> path_type;
	const size_t expr_max(1920);
	bool paper_id_less(const Paper &a, const Paper &b) {
		return a.Id < b.Id;
	}
	bool paper_id_equal(const Paper &a, const Paper &b) {
		return a.Id == b.Id;
	}
	vector<path_type> pp2au(id_type Id, id_type AuId) {
		Paper pp(getEntities("Id=" + to_string(Id), _AA_AFID | _AA_AUID | _C_CID | _F_FID | _J_JID | _RID)[0]);
		vector<Paper> papers2(getEntities("Composite(AA.AuId=" + to_string(AuId) + ")" + "&orderby=Id:asc", _AA_AFID | _AA_AUID | _C_CID | _F_FID | _ID | _J_JID | _RID));
		string expr;
		bool fst(true);
		for (const auto &au : pp.AA)
			if (fst) {
				expr = "Composite(AA.AuId=" + to_string(au.AuId) + ")";
				fst = false;
			} else
				expr = "Or(" + expr + ",Composite(AA.AuId=" + to_string(au.AuId) + "))";
		vector<Paper> papers1;
		if (expr.size()) papers1 = getEntities(expr + "&orderby=Id:asc", _AA_AFID | _AA_AUID | _ID);
		path_type currentpath = {Id, AuId};
		vector<path_type> ret;
		//1-hop
		//	P-Au
		for (const auto &i : pp.AA)
			if (i.AuId == AuId) {
				ret.push_back(currentpath);
				break;
			}
		//2-hop
		//	P->P-Au
		currentpath.push_back(AuId);
		Paper pt;
		for (const auto &ref : pp.RId) {
			pt.Id = ref;
			auto pos = lower_bound(papers2.begin(), papers2.end(), pt, paper_id_less);
			if (pos != papers2.end() && ref == pos->Id) {
					currentpath[1] = ref;
					if (currentpath[1] != -1) ret.push_back(currentpath);
					break;
			}
		}
		//3-hop
		//	P-CFJ-P-Au
		currentpath.push_back(AuId);
		for (const auto &pps : papers2) {
			currentpath[2] = pps.Id;
			if (pps.C.CId == pp.C.CId) {
				currentpath[1] = pp.C.CId;
				if (currentpath[1] != -1) ret.push_back(currentpath);
			}
			for (const auto &fpps : pps.F)
				for (const auto &fpp : pp.F)
					if (fpps.FId == fpp.FId) {
						currentpath[1] = fpp.FId;
						if (currentpath[1] != -1) ret.push_back(currentpath);
						break;
					}
			if (pps.J.JId == pp.J.JId) {
				currentpath[1] = pp.J.JId;
				if (currentpath[1] != -1) ret.push_back(currentpath);
			}
		}
		//	P-Au-Af-Au
		vector<id_type> afs2;
		for (const auto &pp2 : papers2)
			for (const auto &au : pp2.AA)
				if (au.AuId == AuId) afs2.push_back(au.AfId);
		sort(afs2.begin(), afs2.end());
		afs2.erase(unique(afs2.begin(), afs2.end()), afs2.end());
		for (const auto &pp1 : papers1)
			for (const auto &au1 : pp1.AA)
				for (const auto &af : afs2)
					if (af == au1.AfId) {
						for (const auto &au : pp.AA)
							if (au.AuId == au1.AuId) {
								currentpath[1] = au1.AuId;
								currentpath[2] = au1.AfId;
								if (currentpath[1] != -1 && currentpath[2] != -1) ret.push_back(currentpath);
								break;
							}
						break;
					}
		//	P->P->P-Au
		vector<Paper> papersr, tmp;
		fst = true;
		expr.clear();
		for (const auto &ref : pp.RId) {
			if (expr.size() + to_string(ref).size() + 8 > expr_max) {
				//string("Or(,Id=)").size() == 8
				tmp = getEntities(expr, _ID | _RID);
				papersr.insert(papersr.end(), tmp.begin(), tmp.end());
				fst = true;
			}
			if (fst) {
				expr = "Id=" + to_string(ref);
				fst = false;
			} else
				expr = "Or(" + expr + ",Id=" + to_string(ref) + ")";
		}
		if (expr.size()) {
			tmp = getEntities(expr, _ID | _RID);
			papersr.insert(papersr.end(), tmp.begin(), tmp.end());
		}
		for (const auto &ppr : papersr) {
			currentpath[1] = ppr.Id;
			for (const auto &ref : ppr.RId) {
				pt.Id = ref;
				auto pos = lower_bound(papers2.begin(), papers2.end(), pt, paper_id_less);
				if (pos != papers2.end() && ref == pos->Id) {
					currentpath[2] = ref;
					if (currentpath[1] != -1 && currentpath[2] != -1) ret.push_back(currentpath);
				}
			}
		}
		//	P-Au-P-Au
		for (const auto &pp2 : papers2) {
			auto pos = lower_bound(papers1.begin(), papers1.end(), pp2, paper_id_less);
			if (pos != papers1.end() && pp2.Id == pos->Id) {
				currentpath[2] = pp2.Id;
				for (const auto &au2 : pp2.AA)
					for (const auto &au : pp.AA)
						if (au.AuId == au2.AuId) {
							currentpath[1] = au.AuId;
							if (currentpath[1] != -1 && currentpath[2] != -1) ret.push_back(currentpath);
							break;
						}
			}
		}
		sort(ret.begin(), ret.end());
		ret.erase(unique(ret.begin(), ret.end()), ret.end());
		return ret;
	}

}
#ifndef PRODUCT
#include <ctime>
#include <cstdio>
#include <iostream>
#include <fstream>
using namespace test;
int main() {
    freopen("in.json","r",stdin);
    freopen("out.json","w",stdout);
    LL id, auid;
    scanf("%lld%lld",&id,&auid);
    vector<Path> ans = pp2au(id,auid);
    print_ans(ans);
	return 0;
}
#endif
