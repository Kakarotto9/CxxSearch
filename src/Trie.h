#pragma once


#include <assert.h>
#include <string.h>
#include <vector>
#include <map>

#define MAXLEN 100

using std::vector;
using std::map;
using std::string;


// 可以用来查找 具有某前缀的所有单词
// 可以查找某单词所在的行

class Node {
    public:
        Node():freq(0),childList() { 
        }
        ~Node() {
			printf("my value=%c,  deleted\n",value);
		}
        void setValue(char c) { 
            value=c;
        }
        void addFreq() { 
            ++freq; 
        }
        void pushLine(int index) {
			if(lines.empty()||index!=lines.back()) //因为vector如果为空，那么back（）会抛出错误
				lines.push_back(index);
        }
		char getValue(){
			return value;
		}
        int getFreq() { 
            return freq; 
        }
		void getLines(vector<int> &v){
			v=lines;
		}
        void append(Node* p) { 
			childList.push_back(p);
        }
        Node* findCharNode(char c){
			int len=childList.size();
			if(len==0) return NULL;
			int i;
			for(i=0;i<len;++i){
				if(c==childList[i]->getValue())
					break;
			}
			if(i>=len)
				return NULL;
		   return childList[i];	
		}

    private:
		char value;            
		int freq;                 //当前节点是否可以作为字符串的结尾,如果是freq>0,如果存在重复单词，freq表示该单词的词频
	public:
        vector<Node*> childList;
        vector<int> lines;        //存储该单词所在某个文章的所有行号
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
        void addWord(const string& s,int lineNum);
		int getFreq(const string& s);
		bool getLineNum(const string& s,vector<int>& v);
		bool printMatch(const string &s,vector<string>& v);
		void getAllWord(vector<string>& v);

    private:
		void clearNode(Node* node);
		void clear();
		/*找到记录该string所有信息的Node节点，也就是匹配该string的路径的最后一个节点*/
		Node* findNode(const string& s);
		/*得到该Node下所有子节点与str指向的字符串构成的新字符串的集合*/
		void getWord(Node* node,char str[],int i,vector<string>& v);
};

void Trie::clearNode(Node* father){
	if(NULL==father)
		return ;
	int i, len=father->childList.size();
	for(i=0;i<len;++i)
	{
		clearNode(father->childList[i]);
	}
	delete father;
}

void Trie::clear(){
    clearNode(root);
}

Node* Trie::findNode(const string& s){
  Node* curr = root;
  int len = s.length();
  for (int i = 0; i < len; i++) { 
        Node* child = curr->findCharNode(s[i]);
		if(NULL==child)
			return NULL;
        curr = child;
  }
  return curr;
}
int Trie::getFreq(const string& s){
	Node* node=findNode(s);
    if(NULL==node)
		return 0;
	return node->getFreq();
}
bool  Trie::getLineNum(const string& s,vector<int> & v){
	Node* node=findNode(s);
    if(NULL==node||node->getFreq()<=0)
		return false; 
	 node->getLines(v);
	 return true;
}

void Trie::addWord(const string& s,int lineNum) {
    Node* curr = root;
    int i,l = s.length();
	assert(l>0);
    for (i = 0; i < l; ++i) { 
        Node* child = curr->findCharNode(s[i]);

        if (NULL==child){ 
			break;
		}
        curr = child;
	}
    for (; i < l; i++) { 
            Node* tmp = new Node();
            tmp->setValue(s[i]);
			curr->append(tmp);
            curr = tmp;
     }
	 curr->addFreq();
	 curr->pushLine(lineNum);
}

void Trie::getWord(Node* node,char str[],int i,vector<string>& v){
   str[i]=node->getValue();
   if(node->getFreq()>0)
   {
	   v.push_back(string(str));
   }
   for(auto ptr:node->childList){
      getWord(ptr,str,i+1,v);
   }
   str[i]=0;//必须的步骤
}

bool Trie::printMatch(const string &s,vector<string>& v){
   Node* node=findNode(s);
   if(NULL==node)
	   return false;
   char c[MAXLEN];
   for(int i=0;i<MAXLEN;i++)
	   c[i]=0;
   int len=s.size();
   s.copy(c,len-1,0);  //因为在getWord中会把s[len-1]给c，所以在s只需要拷贝0-len-2的
   getWord(node,c,len-1,v);
   return true; // 当findNode找到非NULL的node时，就表明一定有匹配的单词,所以直接返回true就行
}
void Trie::getAllWord(vector<string> & v){
   char c[MAXLEN];
   for(int i=0;i<MAXLEN;i++)
	   c[i]=0;
    /* 这样是错的，会导致c[0]为空，root的子节点从c[1]开始存储
	getWord(root,c,0,v);
	*/
   for(auto ptr:root->childList){
      getWord(ptr,c,0,v);
   }
}

