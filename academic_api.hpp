#ifndef ACADEMIC_API_H
#define ACADEMIC_API_H

#include <vector>
#include <string>
#include <curl/curl.h>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace std;
using namespace rapidjson;

#define _M          7
#define _ID         1
#define _F_FID      2
#define _J_JID      4
#define _C_CID      8
#define _AA_AUID    16
#define _AA_AFID    32
#define _RID        64
const char *_ITEMS[_M] = {"Id", "F.FId", "J.JId", "C.CId", "AA.AuId", "AA.AfId", "RId"};

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
        //int CC ;         // Citation count               Int32
        vector<long long> RId ;     // Reference ID                 Int64
        //vector<string> W ;     // Words from paper title/abs   String
        // tract for full text search
        Paper(){
            Id = 0;
            vector<Author> AA = vector<Author>(0);
        }
        ~Paper(){
            AA.clear();
            RId.clear();
        }
    };
    class Info{
    public:
        string expr ; // Query Expression
        double logprob ; // Log probability?
        vector<Paper> entities ; // List of Entities  
    };
    
}
using namespace Academic;

int len;
size_t save_data(void *ptr, size_t size, size_t nmemb, char* stream){
    size_t written = size * nmemb;
    memcpy(stream + len, ptr, size * nmemb);
    len += strlen(stream + len);
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
        len = 0;
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, save_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, bStr);
        
        res = curl_easy_perform(curl);   
        if (res != 0) {
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        }
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
        paper.C.CId = -1;

    if(p.HasMember("C") && p["C"].HasMember("CId"))
        paper.C.CId = p["C"]["CId"].GetInt64();
    else
        paper.C.CId = -1;


    if(p.HasMember("RId")){
        const Value &a = p["RId"];
        for(int i = 0; i< a.Size(); ++i)
            paper.RId.push_back(a[i].GetInt64());
    }
    
    return paper;
}

vector<Paper> getEntities(string expr, int items){
    vector<Paper> entities;
    
    char *json = new char[100000000];
    string url("https://oxfordhk.azure-api.net/academic/v1.0/evaluate?count=1000&subscription-key=f7cc29509a8443c5b3a5e56b0e38b5a6");
    url += "&expr=" + expr + "&attributes=";
    
    int fst = 1;
    for (int i = 0; i < _M; ++i){
        if( (items >> i ) & 1 ){
            if(!fst)
                url += ',';
            fst = 0;
            url += _ITEMS[i];
        }
    }

    getUrl(url.c_str(), json);

    //printf("%s\n%s\n",url.c_str(),json);
    
    Document document;
    document.Parse(json);
    

    const Value &a = document["entities"];
    for(SizeType i = 0; i < a.Size(); ++i){
        entities.push_back(get_paper(a[i]));
    }

    return entities;
}

#endif
