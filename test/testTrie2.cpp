
#include<iostream>
#include"Trie2.h"

using namespace std;

int main(){
	Trie t;
	t.addWord("weixin",1);
	t.addWord("weixi",2);
	t.addWord("weixn",3);
	t.addWord("wexin",4);
	t.addWord("weixi",8);
	cout<<"getFreq="<<t.getFreq("weixn")<<endl;
	t.addWord("ascw",2);
	t.addWord("asce",3);
	t.addWord("ahce",9);
	vector<string> v;
	t.printMatch("wei",v);
	for(auto i:v){
		cout<<"v="<<i<<endl;
	}
	vector<string> v2;
	t.printMatch("as",v2);
	for(auto i:v2){
		cout<<"v2="<<i<<endl;
	}
	vector<string> v3;
	t.getAllWord(v3);
	for(auto i:v3){
		cout<<"v3="<<i<<endl;
	}
	vector<int> v4;
	bool b2=t.getLineNum("weixi",v4);
	if(b2){
		for(auto i:v4){
			cout<<"v4="<<i<<endl;
		}
	}
	else{
		cout<<"v4没找到"<<endl;
	}
}
