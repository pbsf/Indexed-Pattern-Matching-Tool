using namespace std;
#include <fstream>
#include <map>
#include <iostream>
#include <cassert>
#include <vector>
#include <sstream>
#include <iterator>
// TODO: Comentário de paguso: com um alfabeto pequeno como o ascii, você pode
// iniciar o dicionário já com uma entrada para cada letra e, assim, sempre vai
// ter pelo menos um match com alguma entrada no dicionário, eliminando assim a
// necessidade de representar o caractere do mismatch na palavra de código. tem
// um pequeno custo acrescido de já iniciar o dicionário com uma entrada para
// cada letra, o que aumenta uns bits nos índices das entradas, mas essa
// diferença é rapidamente compensada.

// Implementation of the LZ78.
// References: https://docs.google.com/document/d/17rxbuMELIvZBUP_FGxQXGg0xCe-3d7vGUaQg1JDU3cw
// faculty.kfupm.edu.sa/ICS/jauhar/ics202/Unit31_LZ78.ppt

// Represents a Node in the Dictionary, which has a Trie's structure.
class DictNode {
  public:
    int idx;
    char byte;
    map<char, DictNode*> children;

    DictNode(int i, char c) {
        idx = i;
        byte = c;
    }

    void add_node(char c, int new_idx) {
        DictNode* new_node = new DictNode(new_idx, c);
        children[c] = new_node;
    }

    bool has_node(char c) {
        return children.find(c) != children.end();
    }

    DictNode* get_node(char c) {
        map<char, DictNode*>::iterator i = children.find(c);
        assert(i != children.end());
        return i->second;
    }
};

// For debug purposes only
void print_vector(vector<bool> b) {
    for (vector<bool>::const_iterator it = b.begin(); it != b.end(); it++) {
        cout << *it;
    }
    cout << endl;
}

vector<bool> _int_to_bits(int i, int n) {
    vector<bool> b(n); //convent number into bit array
    for (int pos = b.size()-1; pos >= 0; pos--) {
        if ((i & 1) == 1) {
            b[pos] = 1;
        }
        i = i >> 1;
    }
    return b;
}

vector<bool> int_to_bits(int i) {
    int copyI = i;
    int number_of_bits = 0;
    while (copyI > 0) {
        number_of_bits++;
        copyI = copyI >> 1;
    }
    if (number_of_bits == 0) number_of_bits = 1;
    return _int_to_bits(i, number_of_bits);
}

vector<bool> _rev_encode(vector<bool> code) {
    int n = code.size();
    if (n <= 1) return code;
    vector<bool> new_vector = _rev_encode(int_to_bits(n-2));
    new_vector.insert(new_vector.end(), code.begin(), code.end());
    return new_vector;
}

vector<bool> rev_encode(vector<bool> code) {
    code.insert(code.begin(), 1);
    code = _rev_encode(code);
    code.push_back(0);
    return code;
}

vector<bool> cw_encode(int idx, char c) {
    vector<bool> first = rev_encode(int_to_bits(idx));
    vector<bool> second = rev_encode(int_to_bits(c));
    first.insert(first.end(), second.begin(), second.end());
    return first;
}

vector<bool> lz78_encode(string txt) {
    txt += "$";
    int size = txt.length();
    DictNode* first = new DictNode(0, '$');
    map<int, DictNode*> dict;
    vector<bool> code;
    int i = 0;
    int d = 1;
    DictNode* cur = first;
    while (i < size) {
        if (cur->has_node(txt[i])) {
            cur = cur->get_node(txt[i]);
        } else {
            vector<bool> new_code = cw_encode(cur->idx, txt[i]);
            code.insert(code.end(), new_code.begin(), new_code.end());
            cur->add_node(txt[i], d);
            d++;
            cur = first;
        }
        i++;
    }
    return code;
}

int vector_bool_to_int(vector<bool> v) {
    int multiplier = 1;
    int result = 0;
    for (int i = v.size()-1; i >= 0; i--) {
        result += multiplier * v[i];
        multiplier *= 2;
    }
    return result;
}

pair<int, int> cw_decode(vector<bool> v) {
    int k = 1;
    int j = 0;
    vector<bool> Y;
    while (j == 0 || v[j] != 0) {
        vector<bool> temp(v.begin() + j, v.begin() + j + k);
        Y = temp;
        j += k;
        k = vector_bool_to_int(Y) + 2;
    }
    // TODO: Check what happens when this vector is empty.
    vector<bool> voutput(Y.begin()+1, Y.end());
    pair<int, int> output;
    output.first = vector_bool_to_int(voutput);
    output.second = j+1;
    return output;
}

string lz78_decode(vector<bool> v) {
    string txt = "";
    vector< pair<int, int> > D;
    D.push_back(make_pair(0,0));
    int d = 1;
    int i = 0;
    int size = v.size();
    while (i < size) {
        vector<bool> Y(v.begin() + i, v.end());
        pair<int, int> p = cw_decode(Y);
        i += p.second;
        if (i >= size) break; // Necessary to avoid filling 0s in a byte.
        vector<bool> Y2(v.begin() + i, v.end());
        pair<int, int> p2 = cw_decode(Y2);
        i += p2.second;
        char c = p2.first;
        txt += txt.substr(D[p.first].first, D[p.first].second - D[p.first].first) + c;
        D.push_back(make_pair(txt.size()-((D[p.first].second-D[p.first].first)+1), txt.size()));
        d += 1;
    }
    return txt.substr(0, txt.size()-1);
}

int current_bit = 0;
unsigned char bit_buffer;
void WriteBit (int bit, FILE *f) {
    if (bit)
        bit_buffer |= (1<<current_bit);

    current_bit++;
    if (current_bit == 8) {
        fwrite (&bit_buffer, 1, 1, f);
        current_bit = 0;
        bit_buffer = 0;
    }
}

void Flush_Bits (FILE *f) {
    while (current_bit)
        WriteBit (0, f);
}

string decode_file(string filepath) {
    ifstream infile (filepath, ifstream::binary);
    vector<bool> encoded;
    char c;
    while (infile.get(c)) {
        for (int i = 0; i < 8; i++)
            encoded.push_back(((c >> i) & 1));
    }
    return lz78_decode(encoded);
}

vector<bool> encode_text(string text, string output_file) {
    vector<bool> encoded = lz78_encode(text);
    FILE *f = fopen(output_file.c_str(), "wb");
    for (bool b : encoded)
        WriteBit(b, f);
    Flush_Bits(f);
    fclose(f);
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

void test(string txt) {
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
    tests.push_back("a028H082G 2g08h08h02JG0J 240");
    tests.push_back("");
    tests.push_back(" ");
    tests.push_back("_______________");
    for (vector<int>::size_type i = 0; i != tests.size(); i++) {
        cout << "Test #" << i+1 << endl;
        test(tests[i]);
        cout << endl;
    }
}

int main() {
    //vector<bool> code = encode_file("temp.txt");
    //string decoded = decode_file("temp.idx");
    //cout << decoded << endl;
    tests();
    return 0;
}

