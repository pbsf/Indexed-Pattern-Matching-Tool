#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <list>
#include <iterator>
#include <sstream>
#include <iostream>
using namespace std;

class SuffixArray{
	public:
		string *text;
		struct _RS {
		   int rank;
		   string suffix;
		}rs;
		vector <_RS> ranksS1;
		vector <_RS> ranksS2;
		list<int> SA;

	SuffixArray(string *text){
		this->text = text;

	}

	void buildSA(){
		buildS1andS2();
		mergeS1andS2();
	}
	static bool acompare(_RS lhs, _RS rhs) {
		if (lhs.suffix.compare(rhs.suffix) == 0 ){
			return lhs.rank > rhs.rank;
		}else{
			return lhs.suffix < rhs.suffix;
		}
	}
	void buildS1andS2(){
		int size = this->text->length();
		int counterS1 = 0;
		int counterS2 = 0;
		printf("%s\n","Building Suffix Array S1 and S2" );
		for(int i=0;i<size;i++){
			if(i%3!=0){
				string substr = this->text->substr(i,3);
				ranksS1.push_back(rs);
				ranksS1[counterS1].rank = i;
				ranksS1[counterS1].suffix = substr;
				counterS1++;
			}else{
				string substr = this->text->substr(i,3);
				ranksS2.push_back(rs);
				ranksS2[counterS2].rank =i ;
				ranksS2[counterS2].suffix = substr;
				counterS2++;
			}
		}
		sort (ranksS1.begin(), ranksS1.end(), acompare);
		sort (ranksS2.begin(), ranksS2.end(), acompare);

	}

	void mergeS1andS2(){
		int compS1=0;
		int compS2=0;
		printf("%s\n","Merging Suffix Array S1 and S2 to SA" );

		int c=0;


		while (compS1 != ranksS1.size() && compS2 != ranksS2.size()){
			if(ranksS1[compS1].suffix <= ranksS2[compS2].suffix){
				SA.push_back(ranksS1[compS1].rank);
				compS1++;
			}else{
				SA.push_back(ranksS2[compS2].rank);
				compS2++;
				c++;

			}
		}
		if (compS2 == this->ranksS2.size()){
			for(int i = compS1; i<ranksS1.size(); i++){
				SA.push_back(ranksS1[i].rank);

			}
		}else{
			for(int i = compS2; i<ranksS2.size(); i++){
				SA.push_back(ranksS2[i].rank);
			}
		}
	}
	int countMatches(string pat){
		int patLen = pat.size();
		int matches=0;
		for(list<int>::iterator j = SA.begin(); j != SA.end(); ++j){
			if(this->text->substr(*j,patLen) == pat){
				matches++;
			}
		}
		printf("Matches = %d\n",matches );
		return matches;
	}

	list<int> getSA(){
		return SA;
	}

};

SuffixArray* create_sa_from_file(string filepath) {
    ifstream t(filepath);
    stringstream buffer;
    buffer << t.rdbuf();
    string text = buffer.str();
    return new SuffixArray(&text);
}

list<int> index_file(string filepath) {
    SuffixArray* sa = create_sa_from_file(filepath);
    sa->buildSA();
    return sa->getSA();
}

int main(){
    ifstream t("big.txt");
    stringstream buffer;
    buffer << t.rdbuf();
    string text = buffer.str();

    //SuffixArray* sa = create_sa_from_file("big.txt");
    SuffixArray* sa = new SuffixArray(&text);
    sa->buildSA();
    string pat = "herself";
    sa->countMatches(pat);
    list<int> indexes = sa->getSA();

    return 0;
}
