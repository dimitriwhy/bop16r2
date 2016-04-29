#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;
namespace Academic
{
     class Field{
         string FN ;      // Field of study name	        String
         long long FId ;       // Field of study ID	        Int64
	 };
     class Journal{
         string JN ;      // Journal name	                String
         long long JId ;       // Journal ID                  	Int64	
	 };
     class Conference{
         string CN ;      // Conference series name	    String
         long long CId ;       // Conference series ID	Int64	Equals
	 };
     class Author{
         string AuN ;     // Author name                  String
         long long AuId ;      // Author ID                    Int64
         string AfN ;     // Author affiliation name  	String
         long long AfId ;      // Author affiliation ID	    Int64
	 };
     class Paper{
         long long Id ;        // Entity ID
         vector<Author> AA ;    // List of Authors
         Journal J ;      // Journal
         Conference C ;   // Conference
         string Ti ;      // Paper title	                String
         int Y ;          // Paper year	                Int32
         int D ;     // Paper date	                Date
         int CC ;         // Citation count               Int32
         vector<long long> RId ;     // Reference ID                 Int64
         vector<string> W ;     // Words from paper title/abs   String
                                // tract for full text search   
	 };
     class Info{
         string expr ; // Query Expression
         double logprob ; // Log probability?
         vector<Paper> entities ; // List of Entities  
	 };
}
using namespace Academic;
vector<Paper> getEntities(string expr, vector<string> items);
Paper getEntity(long long ID, vector<string> items);
void idWithid(long long id1,long long id2){
	vector <vector <int> > ans;
	vector<string> fuck;
	Paper p1 = getEntity(id1,fuck),p2 = getEntity(id2,fuck);
	//hop-1
	for (unsigned int i = 0;i < p1.RId.size();i++)
		if (p1.RId[i] == id2){
			vector <int> tt;
			tt.push_back(id1);tt.push_back(id2);
			ans.push_back(tt);
		}
	//hop-2
	
}
int main(){
	
	return 0;
}
Paper getEntity(long long ID, vector<string> items){
	
}
