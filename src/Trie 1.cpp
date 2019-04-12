#pragma once


#include <string.h>
#include <vector>
#include <map>

using std::vector;
using std::map;
using std::string;


// 适用于记录文件
// 可以用来查找 具有某前缀的所有单词

class Node {
    public:
        Node():freq(0),lineIndexs(),childs() { 
        }
        ~Node() {
		}
        void addFreq() { 
            ++freq; 
        }
        int getFreq() { 
            return freq; 
        }

        void setValue(char c) { 
            value=c;
        }
		char getValue(){
			return value;
		}
        void append(Node* p) { 

        }
 
        /* Find a child */
        Node* find(char c){
			childs.find(c)->second();
		}

    private:
		char value;            
		int freq;                 //当前节点是否可以作为字符串的结尾,如果是freq>0,如果存在重复单词，freq表示该单词的词频
        vector<Node*> mChildren;
		
};
class Trie {
	private:
		Node* root;
    public:
        Trie():root(new Node()){
	    }
        ~Trie(){
		  clear();
		}
        void add(const string& s);
		int getFreq();
		void printMatch(string &s,vector<string>& v);
		void getAllWord(vector<string>& v);

    private:
		void clearNode();
		void clear();
		Node* findNode(const string& s);
		void getWord(Node* node,char* str,int i,vector<string>& v);
};

void Trie::clearNode(Node* father){
	if(null==father)
		return ;
	for(map<char,Node*>::iterator it=m.begin();it!=m.end();++it)
	{
		clearNode(it->second);
	}
	delete father;
}

void Trie::clear(){
    clearNode(root);
}

void Trie::add(string s) {
    Node* curr = root;
    int l = s.length();

    if (l == 0)    
		return;

    for (i = 0; i < l; i++) { 
        Node* child = curr->find(s[i]);

        if (child == NULL){ 
			break;
		}
        curr = child;
	}
    for (; i < l; i++) { 
            Node* tmp = new Node();
            curr->append(s[i]);
            curr = tmp;
     }
	 curr->addFreq();
}

void Trie::getWord(Node* node,char* str,int i,vector<string>& v){
   str[i]=node->value;
   if(node->freq>0)
   {
	   v.push_bac(string(str));
   }
   for(auto ptr:child){
      getWord(ptr,str,i+1,v);
   }
   str[i]=0;
}

void Trie::printMatch(string &s,vector<string>& v){
   Node* node=findNode(s);
   if(null==node)
	   return ;
   char c[100];
   for(int i=0;i<100;i++)
	   c[i]=0;
   s.copy(c,len,0);
   getWord(node,&c,len,v);
}


