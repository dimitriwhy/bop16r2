#include <vector>
#include <string>
#include <algorithm>
#include "academic_api.hpp"
using namespace std;
using namespace Academic;
namespace test {
	typedef long long id_type;
	typedef vector<id_type> path_type;
	const size_t expr_max(1920);
	bool paper_id_less(const Paper &a, const Paper &b) {
		return a.Id < b.Id;
	}
	bool paper_id_equal(const Paper &a, const Paper &b) {
		return a.Id == b.Id;
	}
	vector<path_type> pp2pp(id_type Id1, id_type Id2) {
		Paper pp1, pp2;
		for (auto const &pp : getEntities("Or(Id=" + to_string(Id1) + ",Id=" + to_string(Id2) + ")", _AA_AUID | _C_CID | _F_FID | _ID | _J_JID | _RID)) {
			if (pp.Id == Id1) pp1 = pp;
			if (pp.Id == Id2) pp2 = pp;
		}
		string expr;
		bool fst;
		vector<Paper> plist;
		path_type currentpath = {Id1, Id2};
		vector<path_type> ret;
		//1-hop
		//	P->P
		for (const auto &ref : pp1.RId)
			if (ref == Id2) {
				ret.push_back(currentpath);
				break;
			}
		//2-hop
		//	P-AuCFJ-P
		currentpath.push_back(Id2);
		for (const auto &au1 : pp1.AA)
			for (const auto &au2 : pp2.AA)
				if (au1.AuId == au2.AuId) {
					currentpath[1] = au1.AuId;
					if (currentpath[1] != -1) ret.push_back(currentpath);
					break;
				}
		if (pp1.C.CId == pp2.C.CId) {
			currentpath[1] = pp1.C.CId;
			if (currentpath[1] != -1) ret.push_back(currentpath);
		}
		for (const auto &f1 : pp1.F)
			for (const auto &f2 : pp2.F)
				if (f1.FId == f2.FId) {
					currentpath[1] = f1.FId;
					if (currentpath[1] != -1) ret.push_back(currentpath);
					break;
				}
		if (pp1.J.JId == pp2.J.JId) {
			currentpath[1] = pp1.J.JId;
			if (currentpath[1] != -1) ret.push_back(currentpath);
		}
		//	P->P->P
		vector<Paper> papersr, tmp;
		fst = true;
		expr.clear();
		for (const auto &ref : pp1.RId) {
			if (expr.size() + to_string(ref).size() + 8 > expr_max) {
				//string("Or(,Id=)").size() == 8
				tmp = getEntities(expr, _AA_AUID | _C_CID | _F_FID | _ID | _J_JID | _RID);
				papersr.insert(papersr.end(), tmp.begin(), tmp.end());
				fst = true;
			}
			if (fst) {
				expr = "Id=" + to_string(ref);
				fst = false;
			}
			else
				expr = "Or(" + expr + ",Id=" + to_string(ref) + ")";
		}
		if (expr.size()) {
			tmp = getEntities(expr, _AA_AUID | _C_CID | _F_FID | _ID | _J_JID | _RID);
			papersr.insert(papersr.end(), tmp.begin(), tmp.end());
		}
		for (const auto &pp : papersr)
			for (const auto &ref : pp.RId)
				if (ref == Id2) {
					currentpath[1] = pp.Id;
					if (currentpath[1] != -1) ret.push_back(currentpath);
					break;
				}
		//3-hop
		//	P-AuCFJ-P->P
		currentpath.push_back(Id2);
		fst = true;
		for (const auto &au : pp1.AA)
			if (fst) {
				expr = "Composite(AA.AuId=" + to_string(au.AuId) + ")";
				fst = false;
			} else
				expr = "Or(" + expr + ",Composite(AA.AuId=" + to_string(au.AuId) + "))";
		if (fst) {
			expr = "Composite(C.CId=" + to_string(pp1.C.CId) + ")";
			fst = false;
		} else
			expr = "Or(" + expr + ",Composite(C.CId=" + to_string(pp1.C.CId) + "))";
		for (const auto &f : pp1.F)
			if (fst) {
				expr = "Composite(F.FId=" + to_string(f.FId) + ")";
				fst = false;
			}
			else
				expr = "Or(" + expr + ",Composite(F.FId=" + to_string(f.FId) + "))";
		if (fst) {
			expr = "Composite(J.JId=" + to_string(pp1.J.JId) + ")";
			fst = false;
		} else
			expr = "Or(" + expr + ",Composite(J.JId=" + to_string(pp1.J.JId) + "))";
		if (expr.size()) expr = "And(" + expr + ",RId=" + to_string(Id2) + ")";
		plist.clear();
		if (expr.size()) plist = getEntities(expr, _AA_AUID | _C_CID | _F_FID | _ID | _J_JID);
		for (const auto &p : plist) {
			currentpath[2] = p.Id;
			for (const auto &au1 : pp1.AA)
				for (const auto &aup : p.AA)
					if (au1.AuId == aup.AuId) {
						currentpath[1] = au1.AuId;
						if (currentpath[1] != -1 && currentpath[2] != -1) ret.push_back(currentpath);
						break;
					}
			if (pp1.C.CId == p.C.CId) {
				currentpath[1] = pp1.C.CId;
				if (currentpath[1] != -1 && currentpath[2] != -1) ret.push_back(currentpath);
			}
			for (const auto &f1 : pp1.F)
				for (const auto &fp : p.F)
					if (f1.FId == fp.FId) {
						currentpath[1] = f1.FId;
						if (currentpath[1] != -1 && currentpath[2] != -1) ret.push_back(currentpath);
						break;
					}
			if (pp1.J.JId == p.J.JId) {
				currentpath[1] = pp1.J.JId;
				if (currentpath[1] != -1 && currentpath[2] != -1) ret.push_back(currentpath);
			}
		}
		//	P->P-AuCFJ-P
		for (const auto &ppr : papersr) {
			currentpath[1] = ppr.Id;
			for (const auto &aur : ppr.AA)
				for (const auto &au2 : pp2.AA)
					if (aur.AuId == au2.AuId) {
						currentpath[2] = aur.AuId;
						if (currentpath[1] != -1 && currentpath[2] != -1) ret.push_back(currentpath);
						break;
					}
			if (ppr.C.CId == pp2.C.CId) {
				currentpath[2] = ppr.C.CId;
				if (currentpath[1] != -1 && currentpath[2] != -1) ret.push_back(currentpath);
			}
			for (const auto &fr : ppr.F)
				for (const auto &f2 : pp2.F)
					if (fr.FId == f2.FId) {
						currentpath[2] = fr.FId;
						if (currentpath[1] != -1 && currentpath[2] != -1) ret.push_back(currentpath);
						break;
					}
			if (ppr.J.JId == pp2.J.JId) {
				currentpath[2] = ppr.J.JId;
				if (currentpath[1] != -1 && currentpath[2] != -1) ret.push_back(currentpath);
			}
		}
		//	P->P->P->P
		plist.clear();
		fst = true;
		for (const auto &p : papersr)
			for (const auto &ref : p.RId) {
				if (expr.size() + to_string(ref).size() + to_string(Id2).size() + 18 > expr_max) {
					//string("And(Or(,Id=),RId=)").size() == 18
					tmp = getEntities("And(" + expr + ",RId=" + to_string(Id2) + ")", _ID);
					plist.insert(plist.end(), tmp.begin(), tmp.end());
					fst = true;
				}
				if (fst) {
					expr = "Id=" + to_string(ref);
					fst = false;
				}
				else
					expr = "Or(" + expr + ",Id=" + to_string(ref) + ")";
			}
		if (expr.size()) {
			tmp = getEntities("And(" + expr + ",RId=" + to_string(Id2) + ")", _ID);
			plist.insert(plist.end(), tmp.begin(), tmp.end());
		}
		sort(plist.begin(), plist.end(), paper_id_less);
		plist.erase(unique(plist.begin(), plist.end(), paper_id_equal), plist.end());
		Paper pt;
		for (const auto &ppr : papersr) {
			currentpath[1] = ppr.Id;
			for (const auto &ref : ppr.RId) {
				pt.Id = ref;
				auto pos = lower_bound(plist.begin(), plist.end(), pt, paper_id_less);
				if (pos != plist.end() && ref == pos->Id) {
					currentpath[2] = ref;
					if (currentpath[1] != -1 && currentpath[2] != -1) ret.push_back(currentpath);
				}
			}
		}
		sort(ret.begin(), ret.end());
		ret.erase(unique(ret.begin(), ret.end()), ret.end());
		return ret;
	}
}

using namespace test;

int main(){
    freopen("in.json","r",stdin);
    freopen("out.json","w",stdout);
    LL id1,id2;
    scanf("%lld%lld",&id1,&id2);
    print_ans(pp2pp(id1,id2));
    return 0;
}
