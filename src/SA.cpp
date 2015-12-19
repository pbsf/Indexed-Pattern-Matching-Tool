#include <string>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;

class SuffixArray{
	public:
		string text;
		int n;
		int m;
		vector<int> sa;
		vector<int> Llcp;
		vector<int> Rlcp;
		
		SuffixArray(string text){
			this->text = text;
			this->n = text.size();
			this->m = int(ceil(log(this->n))); 
			this->sa.resize(n);
			this->Llcp.resize(n);
			this->Rlcp.resize(n);
			vector< vector<int> > p;
			p = buildP();

			for(int i=0; i< this->n; i++){
				this->sa [p[this->m] [i] -1]=i;
			}
			binarySearch(this->sa,p,0,this->n-1);
			FILE *output = fopen("out.idx","w");
			
			for(int i=0; i<this->sa.size(); i++){
				fprintf(output, "%d ",sa[i]);
			}
			fprintf(output, "\n");	
			for(int i=0; i<this->Llcp.size(); i++){
				fprintf(output, "%d ",Llcp[i]);
			}
			fprintf(output, "\n");
			
			for(int i=0; i<this->Rlcp.size(); i++){
				fprintf(output, "%d ",Rlcp[i]);
			}
			fclose(output);
		}

		~SuffixArray(){
		}

		vector< vector<int> > buildP(){
			vector< vector<int> > p;
			p.resize(this->m+1);
			
			for(int i=0; i<=m; i++){
				p[i].resize(this->n);
			}
			for(int i=0; i<n; i++){
				p[0][i] = this->text.at(i);
			}

			vector< vector<int> > triple;
			triple.resize(this->n);
			int j;
			for(int k=1; k<=m; k++){
				j = pow(2,k-1);

				for(int i=0; i<n; i++){
					triple[i].resize(3);
					if(i + j + 1 > this->n){
						triple[i][0] = p[k-1][i];
						triple[i][1] = 0;
						triple[i][2] = i;
					}else{
						triple[i][0] = p[k-1][i];
						triple[i][1] = p[k-1][i+j];
						triple[i][2] = i;	
					}
				}
			sort (triple.begin(), triple.end()); 
			int v=1;
			p[k][triple[0][2]] = v;

			for(int i=1; i<n; i++){
				if(triple[i][0] != triple[i-1][0] || triple[i][1] != triple[i-1][1]){
					v++;
				}
				p[k][triple[i][2]] = v;

			}

		}
		return p;	
	}

	void binarySearch(vector<int>sa,vector< vector<int> > p,int begin,int end){
		if (end - begin <= 1){
			return;
		}
		else{
			int middle = (begin + end)/2;
			this->Llcp[middle] = computeLcp(sa[begin],sa[middle],p);
			this->Rlcp[middle] = computeLcp(sa[middle],sa[end],p);
			this->binarySearch(sa,p,begin,middle);
			this->binarySearch(sa,p,middle,end);
		}
	}

	int computeLcp(int i,int j,vector< vector<int> > p){
		if (i == j){
			return this->n -1 + i;
		}
		else{
			int k = this->m;
			int lcp = 0;
			while(k >= 0 && i <= this->n && j <= this->n){
				if(p[k][i] == p[k][j]){
					lcp += pow(2,k);
					i = i+ pow(2,k);
					j = j+ pow(2,k);
				}
				k --;
			}
			return lcp;
		}
	}

};

int main(){
	//Para arquivo texto de entrada
	/*
	ifstream infile;
	infile.open("test_file.txt");

	string text;
	for (string line; getline(infile, line);) {
		text+=line;
	}
	SuffixArray sa= SuffixArray(text);
	*/

	//Testes com palavras
	/*
	string word=""
	SuffixArray sa= SuffixArray(word);
	*/
	return 0;
}