#ifndef ACADEMIC_API_H
#define ACADEMIC_API_H

#include <vector>
#include <string>
#include <curl/curl.h>
#include "rapidjson-1.0.2/include/rapidjson/rapidjson.h"
#include "rapidjson-1.0.2/include/rapidjson/document.h"
#include "rapidjson-1.0.2/include/rapidjson/writer.h"
#include "rapidjson-1.0.2/include/rapidjson/stringbuffer.h"

using namespace std;
using namespace rapidjson;

namespace Academic
{
    class Field{
    public:
        string FN ;      // Field of study name	        String
        long long FId ;       // Field of study ID	        Int64
    };
    class Journal{
    public:
        string JN ;      // Journal name	                String
        long long JId ;       // Journal ID                  	Int64	
    };
    class Conference{
    public:
        string CN ;      // Conference series name	    String
        long long CId ;       // Conference series ID	Int64	Equals
    };
    class Author{
    public:
        string AuN ;     // Author name                  String
        long long AuId ;      // Author ID                    Int64
        string AfN ;     // Author affiliation name  	String
        long long AfId ;      // Author affiliation ID	    Int64
    };
    class Paper{
    public:
        long long Id ;        // Entity ID
        vector<Author> AA ;    // List of Authors
        Journal J ;      // Journal
        Conference C ;   // Conference
        Field F;
        string Ti ;      // Paper title	                String
        int Y ;          // Paper year	                Int32
        string D ;     // Paper date	                Date
        int CC ;         // Citation count               Int32
        vector<long long> RId ;     // Reference ID                 Int64
        vector<string> W ;     // Words from paper title/abs   String
        // tract for full text search   
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
size_t save_data(void *ptr, size_t size, size_t nmemb, char* stream)
{
    size_t written = size * nmemb;
    memcpy(stream + len, ptr, size * nmemb);
    len += strlen(stream + len);
    return written;
}
bool getUrl(const char *url, char *bStr)
{
    CURL *curl;
    CURLcode res;
    
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: T-shirt");
    curl = curl_easy_init();    // 初始化
    if (curl)
    {
        //curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");// 代理
        len = 0;
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);// 改协议头
        curl_easy_setopt(curl, CURLOPT_URL, url);
        
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, save_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, bStr);
        
        //curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, save_data);
        //curl_easy_setopt(curl, CURLOPT_HEADERDATA, hStr); 
        res = curl_easy_perform(curl);   // 执行
        if (res != 0) {
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        }
        return true;
    }
}

inline Paper get_paper(const Value &p){
    Paper paper;
    if(p.HasMember("Id"))
        paper.Id = p["Id"].GetInt64();
    if(p.HasMember("Ti"))
        paper.Ti = p["Ti"].GetString();
    if(p.HasMember("Y"))
        paper.Y = p["Y"].GetInt();
    if(p.HasMember("D"))
        paper.D = p["D"].GetString();
    if(p.HasMember("CC"))
        paper.CC = p["CC"].GetInt();

    if(p.HasMember("AA")){
        const Value &a = p["AA"];
        for(int i = 0; i< a.Size(); ++i){
            const Value &t  = a[i];
            Author author;
            if(t.HasMember("AuId"))
                author.AuId = t["AuId"].GetInt64();
            if(t.HasMember("AuN"))
                author.AuN = t["AuN"].GetString();
            if(t.HasMember("AfId"))
                author.AfId = t["AfId"].GetInt64();
            if(t.HasMember("AfN"))
                author.AfN = t["AfN"].GetString();
            paper.AA.push_back(author);
        }
    }
    
    if(p.HasMember("F")){
        paper.F.FId = p["F"]["FId"].GetInt64();
        paper.F.FN = p["F"]["FN"].GetString();
    }

    if(p.HasMember("C")){
        paper.C.CId = p["C"]["CId"].GetInt64();
        paper.C.CN = p["C"]["CN"].GetString();
    }

    if(p.HasMember("RId")){
        const Value &a = p["RId"];
        for(Value::ConstValueIterator itr = a.Begin(); itr != a.End(); ++itr)
            paper.RId.push_back(itr -> GetInt64());
    }
     
    return paper;   
}

vector<Paper> getEntities(string expr, vector<string> items){
    vector<Paper> entities;
    
    char *json = new char[10000000];
    string url("https://oxfordhk.azure-api.net/academic/v1.0/evaluate?count=10000&subscription-key=f7cc29509a8443c5b3a5e56b0e38b5a6");
    url += "&expr=" + expr + "&attributes=";
    
    for (int i = 0; i < items.size(); ++i) {
        if(i)
            url += ',';
        url += items[i];
    }
    getUrl(url.c_str(), json);

    //printf("%s %s\n",url.c_str(),json);
                                           
    
    Document document;
    document.Parse(json);
    
    const Value &a = document["entities"];
    for(SizeType i = 0; i < a.Size(); ++i)
        entities.push_back(get_paper(a[i]));

    return entities;
}

#endif
