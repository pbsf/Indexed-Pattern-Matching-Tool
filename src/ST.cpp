#include <string>
#include <fstream>
#include <vector>
#include <limits>
#include <map>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#define MYINFINITY numeric_limits<int>::max()


using namespace std;


class Node{

	public:
		Node *suffixLink;
		//vector<Node> childrens;
		list<string> childrens;
		map <string, tuple<int, int,Node *> > m;
		string letter;

		Node(string letter){
			this->letter = letter;
		}
		void addSuffixLink(Node *node){
			this->suffixLink = node;

		}
		void addTransition(int left, int right, Node *s, string letter){
			this->m[letter] = make_tuple(left,right,s);
		}
		void addChildren(Node *node){
			//printf("%s\n",node->getLetter().c_str() );
			this->childrens.push_back(node->getLetter());

		}
		Node * getSuffixLink(){
			return this->suffixLink;
		}
		list<string> getChildrens(){
			return this->childrens;
		}
		string getLetter(){
			return this->letter;
		}
};

class SuffixTree {
	public:
		string text;
		vector<Node> nodes;
		Node *root;
		list<string> childrens;

	SuffixTree(string text){
		this->text = text;
	}


	pair<Node* , int> update(Node *s,int k,int i){
		Node *oldr = this->root;
		int isTerminator;
		pair<int,Node* > u;
		u = testAndSplit(s,k,i-1,this->text.substr(i,1));
		Node *r;
		r = get<1> (u);
		isTerminator = get<0> (u);
		pair<Node* , int> x;
		while (not isTerminator){
			
			Node *m = new Node(this->text.substr(i));
			r->addTransition(i,MYINFINITY,m,this->text.substr(i,1));
			r->addChildren(m);
			this->childrens.push_back(m->getLetter());
			if (oldr != this->root){
				oldr->addSuffixLink(r);
			}
			oldr = r;

			x = canonize(s->getSuffixLink(), k,i-1);

			s = get<0> (x);
			k = get<1> (x);
			

			u = testAndSplit(s,k,i-1,this->text.substr(i,1));
			
			r = get<1> (u);
			
			isTerminator = get<0> (u);

		}
		
		if (oldr != this->root){
			oldr->addSuffixLink(s);

		}


		return make_pair(s,k);
	}

	pair<int,Node*> testAndSplit(Node *s,int k,int p,string t){
		int p1;
		int k1;
		Node *s1;

		if (k <= p){
			tuple<int, int,Node *> w1ands1 = s->m[this->text.substr(k,1)];
			s1 = get<2> (w1ands1);
			p1 = get<1> (w1ands1);
			k1 = get<0> (w1ands1);
			
			
			if(t.compare(this->text.substr(k1 + p - k + 1,1)) == 0){
				return make_pair(1,s);
			}else{
				Node *r1 = new Node (this->text.substr(k));				
				s->addTransition(k1, k1 + p - k,r1,this->text.substr(k1,1));
				r1->addTransition(k1 + p - k + 1, p1, s1,this->text.substr(k1 + p - k + 1,1));
				s->addChildren(r1);
				r1->addChildren(s1);
				this->childrens.push_back(s1->getLetter());
				this->childrens.push_back(r1->getLetter());
				return make_pair(0,r1);
			}
		}else{
			tuple<int, int,Node *> p = s->m[this->text.substr(k,1)];
			s1 = get<2> (p);
			if (s1 != 0){
				return make_pair(1,s);
			}else{		
				return make_pair(0,s);
				
			}
		}
	}
	pair<Node* , int> canonize(Node *s,int k,int p){
		int p1;
		int k1;
		Node *s1;
		if (p < k){
			return make_pair(s,k);
		}
		tuple<int, int,Node *> w1ands1 = s->m[this->text.substr(k,1)];
		s1 = get<2> (w1ands1);
		p1 = get<1> (w1ands1);
		k1 = get<0> (w1ands1);
		while (p1 - k1 <= p - k){
			k = k + p1 - k1 + 1;
			s = s1;
			if (k <= p){
				
				tuple<int, int,Node *> w1ands1 = s->m[this->text.substr(k,1)];
				s1 = get<2> (w1ands1);
				p1 = get<1> (w1ands1);
				k1 = get<0> (w1ands1);
			}
		}

		return make_pair(s,k);
	}
	
	void buildST(){
		Node root = Node("root");
    	Node bottom = Node("bottom");
    	this->nodes.push_back(root);
    	int n = this->text.size();
    	for(int i=0; i<n; i++){
    		bottom.addTransition(i,i,&root,this->text.substr(i,1));
    	}

    	this->root = &root;
    	this->root->addSuffixLink(&bottom);
    	
    	Node *s = this->root;
    	int k = 0;
    	pair<Node* , int> x;
    	for(int i=0; i<n; i++){
    		x = update(s, k, i);
    		s = get<0> (x);
    		k = get<1> (x);
			x = canonize(s, k,i);
			s = get<0> (x);
			
			k = get<1> (x);
    	}
	}

	int countSubstring(string str, string sub){
    	if (sub.length() == 0){
    		return 0;
    	}
	    int count = 0;
	    for (size_t offset = str.find(sub); offset != std::string::npos;){
			offset = str.find(sub, offset + sub.length());
	        ++count;
			
		}
    	return count;
    }


	int countMatches(string pat){
		int matches = 0;
		string s;

		for(list<string>::iterator j = this->childrens.begin(); j != this->childrens.end(); ++j){
			if ((j)->substr(0,1).compare(pat.substr(0,1)) == 0 ){
				matches = countSubstring(*(j),pat);
				break;
			}
		}
		return matches;
	}
	
    
};

SuffixTree* create_st_from_file(string filepath) {
    ifstream t(filepath.c_str());
    stringstream buffer;
    buffer << t.rdbuf();
    string text = buffer.str();
    return new SuffixTree(text);
}

int main(){

	SuffixTree* st = create_st_from_file("test_file.txt");
	st->buildST();
	string pat = "to";
	int c = 0;
	c = st->countMatches(pat);
	cout<<c<<endl;

	return 0;
}																			