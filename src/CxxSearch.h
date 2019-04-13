#pragma once

#include"Trie.h"
#include <string.h>
#include <vector>
#include <map>
#include <stdio.h>


using std::vector;
using std::map;
using std::string;


/* Contain tries for cxxsearch */
class CxxSearch {
    public:
        CxxSearch():cRoot(),trieMaps() { 
        }
		~CxxSearch(){
			for(map<string, Trie*>::iterator it=trieMaps.begin();it!=trieMaps.end();++it){
				delete it->second();
			}
		}
		/* Check if a file exists */
		bool has(const string &fileName);
        bool load(const string &fileName);
        Trie* get(const string &fileName);


    private:
        string cRoot;
        map<string, Trie*> trieMaps;
};


bool CxxSearch::has(string &fileName) {
    return trieMaps.count(fileName) > 0;
};


bool CxxSearch::load(string &fileName) {
    int t0 = ulf::get_time();

    ifstream file(cRoot + fileName + ".txt");
    if (!file) {
        ulf::warning("CxxSearch cant load file " + cRoot + s + ".txt");
        return false;
    }

    string line,word;
    Trie* trie = new Trie();
    int n = 0;
    while(getline(file, line)){
		istringstream  iis(line);
		while(iis>>word){
			trie->addWord(word);
			++n;
		}
    };


    int t1 = ulf::get_time();
    ulf::info("Loaded " + ulf::to_string(n) + " keywords from " + fileName + " in " + 
        ulf::to_string(t1 - t0) + "ms");

    trieMaps.insert(make_pair(fileName, trie));
    return true;
};


Trie* CxxSearch::getTrie(string &fileName) {
    return trieMaps.find(fileName)->second;
};


