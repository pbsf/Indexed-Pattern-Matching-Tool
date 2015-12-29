using namespace std;
#include <fstream>
#include <map>
#include <iostream>
#include <cassert>
#include <vector>
#include <sstream>
#include <iterator>
#include "util.cpp"

// LZ78
// References: https://docs.google.com/document/d/17rxbuMELIvZBUP_FGxQXGg0xCe-3d7vGUaQg1JDU3cw
//             faculty.kfupm.edu.sa/ICS/jauhar/ics202/Unit31_LZ78.ppt

// Vector pointers used in the implementation
vector<bool>* first;
vector<bool>* second;
vector<bool>* new_code;

// Represents a Node in the Dictionary, which has a Trie's structure.
class DictNode {

   private:
    map<char, DictNode*> children;

   public:
    unsigned long int idx;
    char byte;

    DictNode(unsigned long int i, char c, bool isFirst) {
        idx = i;
        byte = c;
    }

    void add_node(char c, unsigned long int new_idx) {
        children[c] = new DictNode(new_idx, c, false);
    }

    DictNode* get_node(char c) {
        map<char, DictNode*>::iterator i = children.find(c);
        if (i == children.end()) {
            return NULL;
        }
        return i->second;
    }
};

vector<bool> b;

vector<bool>* _int_to_bits(unsigned int i, int n) {
    b.resize(n);
    for (int pos = n-1; pos >= 0; --pos) {
        b[pos] = (i & 1) == 1;
        i = i >> 1;
    }
    return &b;
}

vector<bool>* int_to_bits(unsigned int i) {
    int copyI = i;
    unsigned int number_of_bits = 0;
    while (copyI > 0) {
        ++number_of_bits;
        copyI = copyI >> 1;
    }
    if (number_of_bits == 0) ++number_of_bits;
    return _int_to_bits(i, number_of_bits);
}

vector<bool> g1;
vector<bool> g2;

vector<bool>* rev_encode(unsigned int vec_id) {
    vector<bool>* y = first;
    vector<bool>* g = &g1;
    if (vec_id == 2) {
        y = second;
        g = &g2;
    }
    y->insert(y->begin(), 1);
    g->resize(1); (*g)[0] = 0;

    while (true) {
        g->insert(g->begin(), y->begin(), y->end());
        if (y->size() > 1) {
            y = int_to_bits(y->size()-2);
        } else {
            break;
        }
    }

    return g;
}

vector<bool>* cw_encode(unsigned int idx, char c) {
    first = int_to_bits(idx);
    rev_encode(1);
    second = int_to_bits(c);
    rev_encode(2);
    g1.insert(g1.end(), g2.begin(), g2.end());
    return &g1;
}

vector<bool> lz78_encode(string& txt) {
    txt += "$";
    unsigned int size = txt.length();
    DictNode* first = new DictNode(0, '$', true);
    vector<bool> code;
    unsigned long int i = 0;
    unsigned long int d = 1;
    DictNode* cur = first;
    while (i < size) {
        DictNode* temp = cur->get_node(txt[i]);
        if (temp != NULL) {
            cur = temp;
        } else {
            new_code = cw_encode(cur->idx, txt[i]);
            code.insert(code.end(), new_code->begin(), new_code->end());
            cur->add_node(txt[i], d);
            ++d;
            cur = first;
        }
        ++i;
    }
    txt.erase(txt.size()-1);
    return code;
}

unsigned int vector_bool_to_int(vector<bool>& v) {
    unsigned int multiplier = 1;
    unsigned int result = 0;
    for (int i = v.size()-1; i >= 0; --i) {
        result += v[i] * multiplier;
        multiplier *= 2;
    }
    return result;
}

unsigned int vector_bool_to_int(vector<bool>& v, int ini_pos, int final_pos) {
    unsigned int multiplier = 1;
    unsigned int result = 0;
    for (int i = final_pos; i >= ini_pos; --i) {
        result += v[i] * multiplier;
        multiplier *= 2;
    }
    return result;
}

pair<int, int> cw_decode(vector<bool>& v, int ini_pos) {
    unsigned int k = 1;
    unsigned int j = ini_pos;
    unsigned int lastK = -1;
    unsigned int lastJ = -1;
    vector<bool> Y;
    while (j == ini_pos || v[j] != 0) {
        int temp = vector_bool_to_int(v, j, j+k-1);
        lastJ = j;
        j += k;
        lastK = k;
        k = temp + 2;
    }
    pair<int, int> output;
    output.first = vector_bool_to_int(v, lastJ+1, lastJ+lastK-1);
    output.second = j+1-ini_pos;
    return output;
}

string lz78_decode(vector<bool>& v) {
    string txt = "";
    vector< pair<int, int> > D;
    D.push_back(make_pair(0,0));
    unsigned int d = 1;
    unsigned int i = 0;
    unsigned int size = v.size();
    while (i < size) {
        pair<int, int> p = cw_decode(v, i);
        i += p.second;
        if (i >= size) break; // Necessary to avoid filling 0s in a byte.
        pair<int, int> p2 = cw_decode(v, i);
        i += p2.second;
        char c = p2.first;
        txt += txt.substr(D[p.first].first, D[p.first].second - D[p.first].first) + c;
        D.push_back(make_pair(txt.size()-((D[p.first].second-D[p.first].first)+1), txt.size()));
        d += 1;
    }
    return txt.substr(0, txt.size()-1); // Removes last char.
}

string decode_file(string filepath) {
    ifstream infile (filepath, ifstream::binary);
    vector<bool> encoded;
    char c;
    while (infile.get(c)) {
        for (unsigned int i = 0; i < 8; ++i)
            encoded.push_back(((c >> i) & 1));
    }
    return lz78_decode(encoded);
}

vector<bool> encode_text(string& text, string& output_file) {
    vector<bool> encoded = lz78_encode(text);
    FILE *f = fopen(output_file.c_str(), "wb");
    for (unsigned int i = 0; i < encoded.size(); ++i)
        write_bit(encoded[i], f);
    close_file(f);
    return encoded;
}

vector<bool> encode_file(string filepath) {
    ifstream t(filepath);
    stringstream buffer;
    buffer << t.rdbuf();
    string to_encode = buffer.str();

    // Finding new filename -> .idx extension
    size_t lastindex = filepath.find_last_of(".");
    string idx_name = filepath.substr(0, lastindex) + ".idx";
    return encode_text(to_encode, idx_name);
}

void test(string& txt) {
    vector<bool> code = lz78_encode(txt);
    string output = lz78_decode(code);
    cout << "Input   string: " << txt << endl;
    cout << "Decoded string: " << output << endl;
    assert(txt == output);
}

void tests() {
    vector<string> tests;
    tests.push_back("a");
    tests.push_back("aa");
    tests.push_back("aaa");
    tests.push_back("abracadabra");
    tests.push_back("shfda asd de12 $%!!@ -adsd");
    tests.push_back("a028H082G 2g08h08h02JG0J 24011111111");
    tests.push_back("");
    tests.push_back(" ");
    tests.push_back("Break \n line \n test!");
    for (vector<int>::size_type i = 0; i != tests.size(); ++i) {
        cout << "Test #" << i+1 << endl;
        test(tests[i]);
        cout << endl;
    }
}

int main() {
    tests();
    //vector<bool> code = encode_file("big.txt");
    //string decoded = decode_file("big.idx");
    //debug(decoded);
    //cout << decoded << endl;
    return 0;
}

