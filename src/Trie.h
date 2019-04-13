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
   private:
		typedef map<char,Node*>::iterator MapIterator;
    public:
        Node():freq(0),lineIndexs(),childs() { 
        }
        ~Node() {
		}
        void addFreq() { 
            ++freq; 
        }

		/*添加以该Node结束的单词所在的行号*/
        void pushLine(int index) {
			if(index!=lines.back())
				lines.push_back(index);
        }

		/*找是否有字符为c的子节点，并返回*/
        Node* findNode(char c){
			MapIterator mit =childs.find(c);
			if(mit==childs.end())
				return null;
			return mit->second();
		}
		/*添加子节点*/
		void append(char c,Node* p) { 
            childs.insert({c,p});
        }

		int getFreq() { 
            return freq; 
        }
		vector<int>& getLines(){
			return lines;
		}

    private:
		int freq;                 //当前节点是否可以作为字符串的结尾,如果是则freq>0,如果存在重复单词，freq表示该单词的词频
        vector<int> lines;        //存储该单词所在某个文章的所有行号
		map<char,Node*> childs;  
		
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
		void addWord(const string& s，int lineNum);
		int getFreq();
        vector<int>& getLines();

    private:
		void clearNode();
		void clear();
		/*返回记录该string的信息的节点*/
		Node* findNode(const string& s);
};

void Trie::clearNode(Node* father){
	if(null==father)
		return ;
	for(Node::MapIterator mit=father.begin();mit!=father.end();++mit)
	{
		clearNode(mit->second);
	}
	delete father;
}

void Trie::clear(){
    clearNode(root);
}

void Trie::addWord(string s,int lineNum) {
    Node* curr = root;
    int i,len = s.length();

	assert(l>0&&lineNum>0);

    for (i = 0; i < len; i++) { 
        Node* child = curr->find(s[i]);

        if (NULL==child){ 
			break;
		}
        curr = child;
	}
	while(i<len){
            Node* tmp = new Node();
            curr->append(s[i],tmp);
            curr = tmp;
			i++;
     }
	 curr->addFreq();
	 curr->pushLine(lineNum);
}

Node* Trie::findNode(const string & s){  //找到匹配单词的最后一个存储Node
  Node* curr = root;
  int len = s.length();
  for (i = 0; i < len; i++) { 
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

vector<int>& Trie::getLines(const string& s){
	Node* node=findNode(s);
    if(null==node)
		return vector<int> ();
	return node->getLines();
}




