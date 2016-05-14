#define PRODUCT
#include <bits/stdc++.h>
#include <iostream>
#include "fcgio.h"
#include "academic_api.hpp"
#include "pp2pp.cpp"
#include "au2au.cpp"
#include "au2pp.cpp"
#include "pp2au.cpp"

using namespace std;

const unsigned long STDIN_MAX = 1000000;

bool test_flag = false;
pair<long long, long long > get_query(const FCGX_Request & request) {
    char * content_length_str = FCGX_GetParam("CONTENT_LENGTH", request.envp);
    unsigned long long content_length = STDIN_MAX;

    if (content_length_str) {
        content_length = strtol(content_length_str, &content_length_str, 10);
        if (*content_length_str) {
            cerr << "Can't Parse 'CONTENT_LENGTH='"
                 << FCGX_GetParam("CONTENT_LENGTH", request.envp)
                 << "'. Consuming stdin up to " << STDIN_MAX << endl;
        }

        if (content_length > STDIN_MAX) {
            content_length = STDIN_MAX;
        }
    } else {
        // Do not read from stdin if CONTENT_LENGTH is missing
        content_length = 0;
    }
    
    pair<long long, long long> ret;
    
    char c = *((char*)FCGX_GetParam("SCRIPT_NAME", request.envp) + 1);
    if(c == 'q')
        test_flag = false;
    else if(c == 't')
        test_flag = true;
    else
        return ret;
    
    FILE* err = fopen("error.log", "a+");
    fprintf(err, "SCRIPT_NAME=%s\n", FCGX_GetParam("SCRIPT_NAME", request.envp));
    fclose(err);
    
    char *query_string = FCGX_GetParam("QUERY_STRING", request.envp);

    sscanf(query_string, "id2=%lld&id1=%lld", &ret.second, &ret.first);
    return ret;
}

vector<Path> get_ans(LL id1, LL id2){
    if(!id1 || !id2){
        vector<Path> ret;
        return ret;
    }
    bool pp1 = false, pp2 = false;
    Paper p1, p2;
    string q1 = string("OR(Composite(AA.AuId=") + to_string(id1) + string("),AND(Y>0,Id=") + to_string(id1) + string("))");
    string q2 = string("OR(Composite(AA.AuId=") + to_string(id2) + string("),AND(Y>0,Id=") + to_string(id2) + string("))");
    vector<Paper> chk = getEntities(string("OR(") + q1 + string(",") + q2 + string(")"), _ID|_F_FID|_J_JID|_C_CID|_AA_AUID|_RID |_CC | _AA_AFID);
    vector<Paper> paper_by_au;
    for(auto &p : chk){
        if(p.Id == id1){
            p1 = p;
            pp1 = true;
        }
        if(p.Id == id2){
            p2 = p;
            pp2 = true;
        }
        int flag = 0;
        for(auto &author : p.AA)
            if(author.AuId == id1 || author.AuId == id2){
                flag = 1;
            }
        if(flag)
            paper_by_au.push_back(p);
    }
    if(pp1 && pp2)
        return pp2pp(id1, id2, p1, p2);
    else if(pp1)
        return PP2AU::pp2au(id1, id2, p1, paper_by_au);
    else if(pp2)
        return AU2PP::au2pp(id1, id2, paper_by_au, p2);
    else
        return au2au(id1, id2, chk);
}

int main(void) {
    // Backup the stdio streambufs
    streambuf * cin_streambuf  = cin.rdbuf();
    streambuf * cout_streambuf = cout.rdbuf();
    streambuf * cerr_streambuf = cerr.rdbuf();

    FCGX_Request request;

    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);
    

    while (FCGX_Accept_r(&request) == 0) {

        fcgi_streambuf cin_fcgi_streambuf(request.in);
        fcgi_streambuf cout_fcgi_streambuf(request.out);
        fcgi_streambuf cerr_fcgi_streambuf(request.err);

        cin.rdbuf(&cin_fcgi_streambuf);
        cout.rdbuf(&cout_fcgi_streambuf);
        cerr.rdbuf(&cerr_fcgi_streambuf);
        

        const char * uri = FCGX_GetParam("REQUEST_URI", request.envp);
        pair<long long, long long> query = get_query(request);


        cout << "Content-type: application/json\r\n"
             << "\r\n";
        clock_t ct0, ct1; 
        struct tms tms0, tms1;
        ct0 = times (&tms0);

        printf("%lld %lld\n", query.first, query.second);
        try{        
            print_ans(get_ans(query.first, query.second));
            ct1 = times (&tms1);
            double ti1 = (ct1 - ct0) / (double)sysconf (_SC_CLK_TCK);
            printf("%f\n",ti1);
        }catch(...){
            time_t nowtime = time(NULL);
            struct tm *ptm;  
            ptm=localtime(&nowtime);
            FILE* err = fopen("error.log", "a+");
            fprintf(err, "%d %d %d %d:%d:%d %lld %lld\n", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, query.first, query.second);
            fclose(err);
        }
        
        if(test_flag){
            double ti1 = (ct1 - ct0) / (double)sysconf (_SC_CLK_TCK);
            cout<<",[\"ti\":" << ti1 <<"]" << endl;
        }
    }

// restore stdio streambufsr
    cin.rdbuf(cin_streambuf);
    cout.rdbuf(cout_streambuf);
    cerr.rdbuf(cerr_streambuf);
    
    
    return 0;
}
