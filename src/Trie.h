#pragma once


#include <string.h>
#include <vector>
#include <map>


using std::vector;
using std::map;
using std::string;

// 适用于流式文件，另一个版本请看Trie2.h
// 可以查找文件内某单词的所有所在行

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

        void pushLine(int index) { 
            lineIndexs.push_back(index);
        }
        void append(char c,Node* p) { 
            childs.insert({c,p});
        }
 
        /* Find a child */
        Node* find(char c){
			childs.find(c)->second();
		}

    private:
		int freq;                 //当前节点是否可以作为字符串的结尾,如果是freq>0,如果存在重复单词，freq表示该单词的词频
        vector<int> lineIndexs;   //存储该单词所在的所有行数
		map<char,Node*> childs;  
        //vector<Node*> mChildren;
		
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
        void add(string s，int lineNum);
		int getFreq();
		vector<int> getLines();

    private:
		void clearNode();
		void clear();
		Node* findNode();
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

void Trie::add(string s,int lineNum) {
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
            curr->append(s[i],tmp);
            curr = tmp;
     }
	 curr->addFreq();
	 curr->addLine(lineNum);
}

Node* Trie::findNode(const string & s){  //找到匹配单词的最后一个存储Node
  Node* curr = root;
  int l = s.length();
  for (i = 0; i < l; i++) { 
        Node* child = curr->find(s[i]);
		if(null==child)
			return null;
        curr = child;
  }
  return curr;
}

int Trie::getFreq(const string& s){
	Node* node=findNode(s);
    if(null==node)
		return 0;
	return node->getFreq();
}

vector<int> Trie::getLines(const string& s){
	Node* node=findNode(s);
    if(null==node)
		return vector<int> ();
	return node->getLine();
}




