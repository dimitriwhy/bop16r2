#define PRODUCT
#include <bits/stdc++.h>
#include <iostream>
#include "fcgio.h"
#include "academic_api.hpp"
#include "idwithid.cpp"
#include "au2au.cpp"
#include "au2pp.cpp"

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
    FILE* err = fopen("error.txt", "w");
    char *query_string = FCGX_GetParam("QUERY_STRING", request.envp);

    if(*(FCGX_GetParam("SCRIPT_NAME", request.envp) + 1) == 'q')
        test_flag = false;
    else
        test_flag = true;
    pair<long long, long long> ret;
    sscanf(query_string, "id1=%lld&id2=%lld", &ret.first, &ret.second);
    
    return ret;

    /*
    char * content_buffer = new char[content_length];
    cin.read(content_buffer, content_length);
    content_length = cin.gcount();
    // Chew up any remaining stdin - this shouldn't be necessary
    // but is because mod_fastcgi doesn't handle it correctly.

    // ignore() doesn't set the eof bit in some versions of glibc++
    // so use gcount() instead of eof()...
    do cin.ignore(1024); while (cin.gcount() == 1024);

    string content(content_buffer, content_length);
    delete [] content_buffer;

    return content;
    */
}

vector<Path> get_ans(LL id1, LL id2){
    bool pp1 = false, pp2 = false;
    vector<Paper> chk = getEntities(string("OR(Id=") + to_string(id1) + string(",Id=") + to_string(id2) + string(")"), _ID | _AA_AUID);
    for(auto &p : chk){
        if(p.Id == id1 && p.AA.size())
            pp1 = true;
        if(p.Id == id2 && p.AA.size())
            pp2 = true;
    }
    if(pp1 && pp2)
        return Id2Id(id1, id2);
    else if(pp1)
        return pp2au(id1, id2);
    else if(pp2){
        return au2pp(id1, id2);
    }
    else
        return au2au(id1, id2);
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
        //cout<<"[" << query.first <<','<< query.second<<']';
        print_ans(get_ans(query.first, query.second));
        cout<<",[\"ti\":" << ti <<"]" << endl;
        /*
             << "<html>\n"
             << "  <head>\n"
             << "    <title>Hello, World!</title>\n"
             << "  </head>\n"
             << "  <body>\n"
             << "    <h1>Hello " << content.first << content.second << " from " << uri << " !</h1>\n"
             << "  </body>\n"
             << "</html>\n";
        */
        // Note: the fcgi_streambuf destructor will auto flush
    }

// restore stdio streambufsr
    cin.rdbuf(cin_streambuf);
    cout.rdbuf(cout_streambuf);
    cerr.rdbuf(cerr_streambuf);
    
    
    return 0;
}
