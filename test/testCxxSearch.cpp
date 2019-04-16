#include"CxxSearch.h"
#include  <unistd.h>  
#include  <stdio.h> 

using std::cout;
using std::endl;

int main(){
	char   buffer[100];   
	getcwd(buffer, 100);
	cout<<buffer<<endl;
	CxxSearch cs(buffer);
	bool b=cs.load("CxxSearch.h");
	cout<<b<<endl;
	vector<string> v;
	cs.printMatch("file",v);
	for(auto s:v){
		cout<<s<<endl;
	}
	vector<int> v2;
	cs.getLineNum("string",v2);
	for(auto s:v2){
		cout<<s<<endl;
	}
	getchar();
}
