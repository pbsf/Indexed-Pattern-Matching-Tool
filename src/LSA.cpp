#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <list>
#include <iterator>
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
		list<int> lista;

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
				lista.push_back(ranksS1[compS1].rank);
				compS1++;
			}else{
				lista.push_back(ranksS2[compS2].rank);
				compS2++;
				c++;

			}
		}
		if (compS2 == this->ranksS2.size()){
			for(int i = compS1; i<ranksS1.size(); i++){
				lista.push_back(ranksS1[i].rank);

			}
		}else{
			for(int i = compS2; i<ranksS2.size(); i++){
				lista.push_back(ranksS2[i].rank);
			}
		}
	}
	int countMatches(string pat){
		int patLen = pat.size();
		int matches=0;
		for(list<int>::iterator j = lista.begin(); j != lista.end(); ++j){
			if(this->text->substr(*j,patLen) == pat){
				matches++;
			}
		}
		printf("Matches = %d\n",matches );
		return matches;
	}

};
//int main(){


	//ifstream infile;
	//infile.open("big.txt");

	//string text;
	//for (string line; getline(infile, line);) {
		//text+=line;
	//}

	//string pat = "herself";


	//SuffixArray sa = SuffixArray(&text);
	//sa.buildSA();
	//sa.countMatches(pat);

	//return 0;
//}
