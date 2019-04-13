#include <iostream>
#include <fstream>
#include <string.h>

// For various tree and maps uses
#include <vector>
#include <map>

// Sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

// Benchmark
#include <sys/time.h>
#include <ctime>

// Some utilities functions
#include "utils.hpp"


/* Main namespace */
using namespace std;


int sock_port = 1234;
string sock_host = "localhost";

/* Tried to "define" it like SOCK_HOST
   but can't "cout" it anymore then. */
string VERSION = "0.1";
bool verbose = false;



/* Process a query string */
string process_query(CxxSearch* cxxsearch, string s) {

    string response = "";

    // First separate corpus list from keyword queried
    string::size_type loc = s.find(" ", 0);
    if( loc == string::npos ) {
        ulf::warning("Separator not found in query " + s);
        return "0";
    }
    string corpus_files = s.substr(0, loc);
    string keyword = ulf::clean(s.substr(loc + 1, s.length()));
    vector<string> files;


    // Get list of corpuses
    ulf::split(files, corpus_files, ',');
    
    if (files.size() == 0) {
        ulf::warning("No file found");
        return "0";
    }

    int found = 0;
    // And finally query each corpus
    for(vector<int>::size_type i = 0; i != files.size(); i++) {

        // Check if exists or load it
        if (!cxxsearch->has(files[i]) && !cxxsearch->load(files[i])) {
            ulf::warning("Unable to load corpus " + files[i]);
            continue;
        }

        int t0 = ulf::get_microtime();
        // Search and append in response if found
        if (cxxsearch->get(files[i])->search(keyword)) {
            response += "," + files[i];
            found += 1;

            if (verbose) {
                ulf::debug(keyword + " found in corpus '" + files[i]
                    + "' in " + ulf::to_string(ulf::get_microtime() - t0) + " microsecs");
            }
        }
    }

    if (found > 0) {
        return ulf::to_string(found) + response;
    }
    return "0";
}


/* Setup socket server and wait for connections */
void setup_server(CxxSearch* cxxsearch) {
    /* Prepare address structre and other variables */
    int socket_reusable=1;
    struct sockaddr_in serv_addr;
    int sockfd;

    // clear address structure
    memset(&serv_addr,0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_port = htons(sock_port);

    /* Create socket */
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        ulf::error("Socket not created");
    }

    /* Sometimes we can't restart the server immediately if it was stopped,
        the port is retained for a few seconds. Allow reusing address let restart
        cxxsearch immediately */
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &socket_reusable,
            sizeof(int)) == -1) {
        ulf::error("Setting socket option SO_REUSEADDR failed");
    }

    /* Bind socket */
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0) {
        ulf::error("Socket not binded");
    }

    /* Listen ! Keep 10 connections waiting before refusing connections */
    listen(sockfd, 10);

    if (verbose) {
        ulf::debug("Server started on " + sock_host + ":" + to_string(sock_port));
    }

    /* And loop over requests :) */
    for(;;) {
        socklen_t clilen;
        struct sockaddr_in cli_addr;

        /* Yes! A new message! */
        int newsockfd = accept(sockfd, 
                (struct sockaddr *) &cli_addr, &clilen);

        if (newsockfd < 0) {
            ulf::warning("Error occured while accepting a message. Transaction aborted.");
            continue;
        }

        char buffer[256];
        int n;
        /* Let's receive it*/
        /* TODO: accept messages longer than 254*/
        n = recv(newsockfd, buffer, 254, 0);
        if (n < 0) {
            ulf::warning("Error occured while reading a message. Transaction aborted.");
            return;
        }

        /* Delimiter */
        buffer[n] = '\0';

        /* Process the query */
        string r = process_query(cxxsearch, string(buffer));

        /* Send answer */
        send(newsockfd, r.c_str(), r.size(), 0);

        /* And close socket! */
        close(newsockfd);
    }

    // Never reached but looks cleaner ?
    close(sockfd);
}

int main(int argc, char* argv[]) {

    CxxSearch* cxxsearch = new CxxSearch();

    /* Check arguments */
    for(int i = 0; i < argc; i++) {
        /* verbose mode */
        if (strncmp(argv[i], "-v", 2) == 0) {
            verbose = true;
        /* root path for cxxsearch files */
        } else if (strncmp(argv[i], "-r", 2) == 0) {
            i++;
            cout << "Initializing with root " << argv[i] << endl;
            cxxsearch->root(argv[i]); 
        /* Port */
        } else if (strncmp(argv[i], "-p", 2) == 0) {
            i++;
            sock_port = ulf::to_int(argv[i]);
        /* Host */
        } else if (strncmp(argv[i], "-h", 2) == 0) {
            i++;
            sock_host = argv[i];
        /* Help: show how to use */
        } else if (strncmp(argv[i], "--help", 2) == 0) {
            cout << "\nCxxSearch v" << VERSION << "\n" << endl;
            cout << "Options:" << endl;
            cout << "  --help         Show help and exit" << endl;
            cout << "  -r [root]      Set root path for corpus files" << endl;
            cout << "  -p [port]      Socket port" << endl;
            cout << "  -h [host]      Socket host" << endl;
            cout << "  -v             Activate verbose mode" << endl;
            cout << "\nCreated for Minethat in 2014\n" << endl;
            exit(0);
        }
    }

    setup_server(cxxsearch);
}
