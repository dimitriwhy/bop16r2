#ifndef ACADEMIC_API_HPP
#define ACADEMIC_API_HPP

#include <vector>
#include <string>
#include <curl/curl.h>
#include <sys/times.h> 
#include <unistd.h> 
#include <time.h> 
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace std;
using namespace rapidjson;


double ti = 0;

#define _M          8
#define _ID         1
#define _F_FID      2
#define _J_JID      4
#define _C_CID      8
#define _AA_AUID    16
#define _AA_AFID    32
#define _RID        64
#define _CC         128
const char *_ITEMS[_M] = {"Id", "F.FId", "J.JId", "C.CId", "AA.AuId", "AA.AfId", "RId", "CC"};

typedef long long LL;
typedef vector<LL> Path;


namespace Academic
{
    class Field{
    public:
        //string FN ;      // Field of study name	        String
        long long FId ;       // Field of study ID	        Int64
    };
    class Journal{
    public:
        //string JN ;      // Journal name	                String
        long long JId ;       // Journal ID                  	Int64
        bool operator==(const Journal &rhs){
            return rhs.JId == JId && JId != -1;
        }
    };
    class Conference{
    public:
        //string CN ;      // Conference series name	    String
        long long CId ;       // Conference series ID	Int64	Equals
        bool operator==(const Conference &rhs){
            return rhs.CId == CId && CId != -1;
        }
    };
    class Author{
    public:
        //string AuN ;     // Author name                  String
        long long AuId ;      // Author ID                    Int64
        //string AfN ;     // Author affiliation name  	String
        long long AfId ;      // Author affiliation ID	    Int64
    };
    class Paper{
    public:
        long long Id ;        // Entity ID
        vector<Author> AA ;    // List of Authors
        Journal J ;      // Journal
        Conference C ;   // Conference
        vector<Field> F;
        //string Ti ;      // Paper title	                String
        //int Y ;          // Paper year	                Int32
        //string D ;     // Paper date	                Date
        int CC ;         // Citation count               Int32
        vector<long long> RId ;     // Reference ID                 Int64
        //vector<string> W ;     // Words from paper title/abs   String
        // tract for full text search
    };
    class Info{
    public:
        string expr ; // Query Expression
        double logprob ; // Log probability?
        vector<Paper> entities ; // List of Entities  
    };
    
    void print_ans(vector<Path> ans){
        cout<<"[";
        for(int i = 0; i< ans.size(); i++){
            cout<<"[";
            for(int j = 0; j < ans[i].size(); j++){
                cout<< ans[i][j];
                if(j < ans[i].size() - 1)
                    cout << ',';
            }
            cout<<"]";
            if(i < ans.size()-1)cout<<",";
        }
        cout<<"]";
    }
}
using namespace Academic;


size_t save_data(void *ptr, size_t size, size_t nmemb, pair<char*,int> *stream){
    size_t written = size * nmemb;
    memcpy(stream->first + stream->second, ptr, size * nmemb);
    stream->second += strlen(stream->first + stream->second);
    return written;
}
bool getUrl(const char *url, char *bStr){
    CURL *curl;
    CURLcode res;
    
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: T-shirt");
    curl = curl_easy_init(); 
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url);

        int len = 0;
        pair<char*, int> stream = make_pair(bStr, len);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, save_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream);
        
        res = curl_easy_perform(curl);
        /*
        if (res != 0) {
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        }
        */
        return true;
    }
}

Paper get_paper(const Value &p){
    Paper paper;
    if(p.HasMember("Id"))
        paper.Id = p["Id"].GetInt64();
    else
        paper.Id = -1;
    
    if(p.HasMember("AA")){
        const Value &a = p["AA"];
        for(SizeType i = 0; i< a.Size(); ++i){
            Author author;
            
            if(a[i].HasMember("AuId"))
                author.AuId = (long long)a[i]["AuId"].GetInt64();
            else
                author.AuId = -1;

            if(a[i].HasMember("AfId"))
                author.AfId = (long long)a[i]["AfId"].GetInt64();
            else
                author.AfId = -1;
            paper.AA.push_back(author);
        }
    }

    if(p.HasMember("F")){
        const Value &a = p["F"];
        for(int i = 0; i < a.Size(); ++i){
            Field field;
            field.FId = a[i]["FId"].GetInt64();
            paper.F.push_back(field);
        }
    }


    if(p.HasMember("J") && p["J"].HasMember("JId"))
        paper.J.JId = p["J"]["JId"].GetInt64();
    else
        paper.J.JId = -1;

    if(p.HasMember("C") && p["C"].HasMember("CId"))
        paper.C.CId = p["C"]["CId"].GetInt64();
    else
        paper.C.CId = -1;

    if(p.HasMember("RId")){
        const Value &a = p["RId"];
        for(int i = 0; i< a.Size(); ++i)
            paper.RId.push_back(a[i].GetInt64());
    }
    
    if(p.HasMember("CC"))
        paper.CC = p["CC"].GetInt();
    else
        paper.CC = -1;

    return paper;
}

void get_entities_from_url(string url, vector<Paper> &entities){
    char *json = new char[10000000]();
    getUrl(url.c_str(), json);

    Document document;
    document.Parse(json);

    //printf("%s %s\n", url.c_str(), json);
    const Value &a = document["entities"];
    if(a.IsArray())
        for(SizeType i = 0; i < a.Size(); ++i)
            entities.push_back(get_paper(a[i]));

    //printf("=======\n=======\n%s\n%d\n%d\n", url.c_str(), a.IsArray()?a.Size():0, document.HasMember("aborted"));
    if(a.IsArray() && a.Size() == 1000)
        cout<<"OK"<<endl;
    delete[] json;
    
}

vector<Paper> getEntities(string expr, int items, bool many = false){
    vector<Paper> entities;
    
    char *json = new char[10000]();
    string url("https://oxfordhk.azure-api.net/academic/v1.0/evaluate?subscription-key=f7cc29509a8443c5b3a5e56b0e38b5a6&count=1000");
    url += "&expr=" + expr + "&attributes=";

    string attr;
    int fst = 1;
    for (int i = 0; i < _M; ++i){
        if( (items >> i ) & 1 ){
            if(!fst)
                url += ',';
            fst = 0;
            url += _ITEMS[i];
        }
    }

    Document document;
    
    const int N_PER_Q = 1000;
    if(many){
        string url2 = string("https://oxfordhk.azure-api.net/academic/v1.0/calchistogram?count=0&attributes=Id&subscription-key=f7cc29509a8443c5b3a5e56b0e38b5a6&expr=") + expr;
        getUrl(url2.c_str(), json);
        document.Parse(json);
        int tot = document["num_entities"].GetInt();
        printf("%d\n", tot);
        if(tot <= 0)
            return entities;
        int t_num = (tot - 1) / N_PER_Q + 1;
        thread t[t_num];
        for(int i = 0; i < t_num; i++){
            string str = url + string("&offset=") + to_string(i*N_PER_Q);
            t[i] = thread(get_entities_from_url, str, ref(entities));
        }
        for(int i = 0; i < t_num; i++)
            t[i].join();
        return entities;
    }else{
        get_entities_from_url(url, entities);
    }
    
    delete[] json;
}

#endif
