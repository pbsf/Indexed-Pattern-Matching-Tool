using namespace std;
#include <map>
#include <iostream>
#include <cassert>
#include <cmath>
#include <stdlib.h>
#include <vector>

class DictNode {
  public:
    int idx;
    char byte;
    DictNode* parent;
    map<char, DictNode*> children;

    DictNode(int i, char c, DictNode* p) {
        idx = i;
        byte = c;
        parent = p;
    }

    void add_node(char c, int new_idx) {
        DictNode* new_node = new DictNode(new_idx, c, this);
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

class Dict {
    public:
        map<int, DictNode*> dict;

};

// Stores the compression's output and the compression's dictionary.
// codeInt[i],codeStr[i] is the token at position i.
class TokenSet {
    public:
        string codeStr;
        vector<int> codeInt;
        map<int, DictNode*> dict;
        TokenSet(string s, vector<int> v, map<int, DictNode*> d) {
            codeStr = s;
            codeInt = v;
            dict = d;
        }

        string decode() {
            int size = codeInt.size();
            string output;
            for(int i = 0; i < size; i++) {
                output += decode_token_at_position(i);
            }
            return output;
        }

        string decode_token_at_position(int p) {
            int i = codeInt[p];
            char c = codeStr[p];
            string output = decode_node(dict[i]);
            if (c != '$') output += c;
            return output;
        }

        string decode_node(DictNode* node) {
            string output = "";
            DictNode* cur = node;
            while (cur->byte != '$') {
                output = cur->byte + output;
                cur = cur->parent;
            }
            return output;
        }
};

TokenSet* lz78_encode(string txt, char inputA[], char outputA[]) {
    txt = txt+"$";
    int size = txt.length();

    DictNode* first = new DictNode(0, '$', NULL);
    map<int, DictNode*> dict;
    dict[0] = first;

    string codeStr = "";
    vector<int> codeInt;
    int i = 0;
    int d = 1;
    DictNode* cur = first;
    while (i < size) {
        if (cur->has_node(txt[i])) {
            cur = cur->get_node(txt[i]);
        } else {
            //code = code + cur->idx + txt[i];
            codeStr += txt[i];
            codeInt.push_back(cur->idx);
            cur->add_node(txt[i], d);
            DictNode* new_node = cur->get_node(txt[i]);
            dict[d] = new_node;
            d++;
            cur = first;
        }
        i++;
    }
    return new TokenSet(codeStr, codeInt, dict);
}

int main() {
    string txt = "abragoisgjiost 230j 023jf0jfew0fj sdofij cadabra";
    char outputA [2];
    outputA[0] = '0'; outputA[1] = '1';
    char inputA [256];
    for (int i = 0; i < 256; i++) {
        inputA[i]= '0' + i;
    }

    TokenSet* token_set = lz78_encode(txt, inputA, outputA);
    string output = token_set->decode();
    assert(txt == output);
    return 0;
}
