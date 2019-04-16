#pragma once

#include"Trie.h"
#include"utils.hpp"
#include <fstream>
#include <sstream>
#include <string.h>
#include <vector>
#include <map>
#include <stdio.h>


using std::vector;
using std::map;
using std::string;
using std::ifstream;
using std::istringstream;


/* Contain tries for cxxsearch */
class CxxSearch {
	public:
		typedef map<string,Trie*>::iterator MapIterator; 
    public:
        CxxSearch():cRoot("~/"),trieMaps() { 
        }
        CxxSearch(const string& path):cRoot(path),trieMaps() { 
        }
		~CxxSearch(){
			for(map<string, Trie*>::iterator it=trieMaps.begin();it!=trieMaps.end();++it){
				delete it->second;
			}
		}
		/* Check if a file exists */
		bool has(const string &fileName);
        bool load(const string &fileName);
		bool printMatch(const string& fileName, const string& str,vector<string> & v);
		bool printMatch(const string& str,vector<string> & v);
		bool getLineNum(const string& fileName, const string& str,vector<int> & v);
		bool getLineNum(const string& str,vector<int> & v);

    private:
        Trie* getTrie(const string &fileName);

        string cRoot;
        map<string, Trie*> trieMaps;

};


bool CxxSearch::has(const string &fileName) {
    return trieMaps.count(fileName) > 0;
};


bool CxxSearch::load(const string &fileName) {
    int t0 = ulf::get_time();

    ifstream file(cRoot +"/"+fileName); 
    if (!file) {
        ulf::warning("CxxSearch cant load file " + cRoot+"/"+  fileName  );
        return false;
    }
    int n = 0,lineNum=1;
    Trie* trie = new Trie();
    string line,word;

    while(getline(file, line)){
		istringstream  iis(line);
		while(iis>>word){
			trie->addWord(word,lineNum);
			++n;
		}
		++lineNum;
    };

    int t1 = ulf::get_time();
    ulf::info("Loaded " + ulf::to_string(n) + " keywords from " + fileName + " in " + 
        ulf::to_string(t1 - t0) + "ms");

    trieMaps.insert(make_pair(fileName, trie));
    return true;
};


Trie* CxxSearch::getTrie(const string &fileName) {
    return trieMaps.find(fileName)->second;
};


bool CxxSearch::printMatch(const string& fileName, const string& str,vector<string> & v){
	if(!has(fileName))
		return false;
	Trie* t=getTrie(fileName);
	return t->printMatch(str,v);
}
bool CxxSearch::printMatch(const string& str,vector<string> & v){
	int oldSize=v.size();
	for(MapIterator mit=trieMaps.begin();mit!=trieMaps.end();++mit){
		mit->second->printMatch(str,v);
	}
	if(oldSize==v.size()) //oldSize代表v原本大小，如果v.size()还是oldSize,代表没有找到任何一个匹配的
		return false;
	return true;
}
bool CxxSearch::getLineNum(const string& fileName, const string& str,vector<int> & v){
	if(!has(fileName))
		return false;
	Trie* t=getTrie(fileName);
	return t->getLineNum(str,v);
}
bool CxxSearch::getLineNum(const string& str,vector<int> & v){
	int oldSize=v.size();
	//两种保存文件名和行号的思路：
	//1 使用map<string,vector<int>>的格式
	//2 使用vector<string>保存具有该单词的文件，使用vector<int>存储所有行，为了区别不同文件的行号，在for循环中，每搜索一个Trie，就往v添加一个0作为隔断符 
	for(MapIterator mit=trieMaps.begin();mit!=trieMaps.end();++mit){
		mit->second->getLineNum(str,v);
	}
	if(oldSize==v.size()) //oldSize代表v原本大小，如果v.size()还是oldSize,代表没有找到任何一个匹配的
		return false;
	return true;
}
