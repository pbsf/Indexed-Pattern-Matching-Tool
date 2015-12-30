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
		string text;
		struct _RS {
		   int rank;
		   string* suffix;
		}rs;
		vector<_RS> ranksS1;
		vector<_RS> ranksS2;
		list<int> SA;
		vector<int> lines;
		vector<int> occ;

	SuffixArray(string txt){
		text = txt;
        int size = text.length();
		for (int i=0; i < size; i++) {
            if (text[i] == '\n') {
                lines.push_back(i);
            }
		}
	}

	void buildSA(){
		buildS1andS2();
		mergeS1andS2();
	}
	static bool acompare(_RS lhs, _RS rhs) {
		if (lhs.suffix->compare(*rhs.suffix) == 0 ){
			return lhs.rank > rhs.rank;
		}else{
			return lhs.suffix < rhs.suffix;
		}
	}
	void buildS1andS2(){
		int size = text.length();
		int counterS1 = 0;
		int counterS2 = 0;
		printf("%s\n","Building Suffix Array S1 and S2" );
		for(int i=0;i<size;i++){
			if(i%3!=0){
				string substr = text.substr(i,3);
				ranksS1.push_back(rs);
				ranksS1[counterS1].rank = i;
				ranksS1[counterS1].suffix = &substr;
				counterS1++;
			}else{
				string substr = text.substr(i,3);
				ranksS2.push_back(rs);
				ranksS2[counterS2].rank =i ;
				ranksS2[counterS2].suffix = &substr;
				counterS2++;
			}
		}
		//sort (ranksS1.begin(), ranksS1.end(), acompare);
		//sort (ranksS2.begin(), ranksS2.end(), acompare);
		radixsort(ranksS1);
		radixsort(ranksS2);

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
		if (compS2 == ranksS2.size()){
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
			if(text.substr(*j,patLen) == pat){
				matches++;
				occ.push_back(*j);
			}
		}
		printf("Matches = %d\n",matches );
		return matches;
	}
	void printMatches(){
		int countOcc = occ.size();
		int countLines = lines.size();
		sort(occ.begin(),occ.end());
		int i = 0;
		int j = 1;
		vector<int> newLines;

		while((i<countOcc) && (j<countLines)){
			int currentLine = lines[j];
			int previousLine = lines[j-1];
			if(occ[i]<currentLine){
                if(!count(newLines.begin(),newLines.end(),currentLine)){
					printf("%s",text.substr(previousLine+1,currentLine-previousLine).c_str() );
					newLines.push_back(currentLine);
                }
				i++;
			}else{
				j++;
			}
		}

	}
	void radixSort(vector <_RS> a){
		int i;
		int arraySize = a.size();
		const size_t sz = sizeof(a)/sizeof(a[0]);
		int bucket[sz];
		int maxVal = 0;
		int digitPosition =1 ;
		
		for(i = 0; i < arraySize; i++) {
			if(a[i].rank > maxVal) maxVal = a[i].rank;
		}

		int pass = 1;

		while(maxVal/digitPosition > 0) {

			int digitCount[10] = {0};

			for(i = 0; i < arraySize; i++){
				digitCount[a[i].rank/digitPosition%10]++;
			}
			
			for(i = 1; i < 10; i++){
				digitCount[i] += digitCount[i-1];
			}
			
			for(i = arraySize - 1; i >= 0; i--){
				bucket[--digitCount[a[i].rank/digitPosition%10]] = a[i].rank;
			}
			for(i = 0; i < arraySize; i++)
				a[i].rank = bucket[i];

			digitPosition *= 10;
		}   
	}

	int getMax(vector <_RS> arr, int n){
	    int max = arr[0].rank;
	    for (int i = 1; i < n; i++)
	        if (arr[i].rank > max)
	            max = arr[i].rank;
	    return max;
	}
	void countSort(vector <_RS> arr, int n, int exp){
	    int output[n];
	    int i;
	    int count[10] = {0};
	    for (i = 0; i < n; i++){
	        count[(arr[i].rank / exp) % 10]++;
	    }
	    for (i = 1; i < 10; i++){
	        count[i] += count[i - 1];
	    }
	    for (i = n - 1; i >= 0; i--){
	        output[count[(arr[i].rank / exp) % 10] - 1] = arr[i].rank;
	        count[(arr[i].rank / exp) % 10]--;
	    }
	    for (i = 0; i < n; i++){
	        arr[i].rank = output[i];
	    }
	}
	void radixsort(vector <_RS> arr){
	    int n = sizeof(arr)/sizeof(arr[0]);
	    int m = getMax(arr, n);
	    for (int e = 1; m / e > 0; e *= 10)
	        countSort(arr, n, e);
	}

	list<int> getSA(){
		return SA;
	}

};

SuffixArray* create_sa_from_file(string filepath) {
    ifstream t(filepath.c_str());
    stringstream buffer;
    buffer << t.rdbuf();
    string text = buffer.str();
    return new SuffixArray(text);
}

list<int> index_file(string filepath) {
    SuffixArray* sa = create_sa_from_file(filepath);
    sa->buildSA();
    return sa->getSA();
}

int main(){
   SuffixArray* sa = create_sa_from_file("big.txt");
   sa->buildSA();
   string pat = "herself";
   sa->countMatches(pat);
   sa->printMatches();
   return 0;
}
