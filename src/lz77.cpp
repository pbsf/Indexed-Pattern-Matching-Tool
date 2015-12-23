#include <bitset>
#include <iostream>
#include <vector>
#include <string>
#include <cassert>

using namespace std;

//Reference: https://msdn.microsoft.com/en-us/library/ee916854.aspx

//Sizes in bits
//TODO - Parametize the sizes
#define OFFSET_SIZE 12
#define LENGTH_SIZE 6
#define BYTE_SIZE 8

struct token{
//bitset<OFFSET_SIZE> offset;
//bitset<LENGTH_SIZE> length;
//bitset<BYTE_SIZE>* byte;
int offset;
int length;
char* byte;
};


//TODO - Refactor to use bits and the bit_decompressor_tests
vector<char> decompressor(const vector<token> &tokens){
  vector<char> output;

  for(token t : tokens ){
    if(!(t.offset == 0 || t.length == 0)){
      int start_point = output.size() - t.offset;
      int end_point   = start_point + t.length;
      for(int i = start_point; i < end_point ; i++ ){
        output.push_back(output[i]);
      }
    }else{
      output.push_back(*t.byte);
    }

  }

  return output;

}

void bit_decompressor_tests(){

  /*struct token tokens[7];
  bitset<OFFSET_SIZE> zero_off(0);
  bitset<LENGTH_SIZE> zero_length(0);
  bitset<OFFSET_SIZE> one_off(1);
  bitset<LENGTH_SIZE> one_length(1);
  bitset<OFFSET_SIZE> two_off(2);
  bitset<LENGTH_SIZE> three_length(3);
  bitset<OFFSET_SIZE> five_off(5);
  bitset<BYTE_SIZE> a_byte('a');
  bitset<BYTE_SIZE> b_byte('b');
  bitset<BYTE_SIZE> c_byte('c');
  token t;

  t.offset = zero_off;
  t.length = three_length;
  t.byte   = &a_byte;
  tokens[0] = t;

  t.offset = one_off;
  t.length = one_length;
  t.byte   = NULL;
  tokens[1] = t;

  t.offset = one_off;
  t.length = one_length;
  t.byte   = NULL;
  tokens[2] = t;

  t.offset = zero_off;
  t.length = zero_length;
  t.byte   = &b_byte;
  tokens[3] = t;

  t.offset = zero_off;
  t.length = zero_length;
  t.byte   = &c_byte;
  tokens[4] = t;

  t.offset = two_off;
  t.length = one_length;
  t.byte   = NULL;
  tokens[5] = t;

  t.offset = five_off;
  t.length = three_length;
  t.byte   = NULL;
  tokens[6] = t;*/

  //bitset<>* result = decompressor(tokens);


}

void char_decompressor_tests(){
  vector<token> tokens;

  char a = 'A';
  char b = 'B';
  char c = 'C';

  token t1;
  t1.offset = 0;
  t1.length = 0;
  t1.byte   = &a;
  tokens.push_back(t1);

  token t2;
  t2.offset = 1;
  t2.length = 1;
  t2.byte   = NULL;
  tokens.push_back(t2);

  token t3;
  t3.offset = 0;
  t3.length = 0;
  t3.byte   = &b;
  tokens.push_back(t3);

  token t4;
  t4.offset = 0;
  t4.length = 0;
  t4.byte   = &c;
  tokens.push_back(t4);

  token t5;
  t5.offset = 2;
  t5.length = 1;
  t5.byte   = NULL;
  tokens.push_back(t5);

  token t6;
  t6.offset = 1;
  t6.length = 1;
  t6.byte   = NULL;
  tokens.push_back(t6);

  token t7;
  t7.offset = 5;
  t7.length = 3;
  t7.byte   = NULL;
  tokens.push_back(t7);

  vector<char> output = decompressor(tokens);
  string s_output = "";
  for(auto c:output){
    s_output += c;
  }

  assert(s_output.compare("AABCBBABC") == 0);
}

void tests(){
  char_decompressor_tests();
  return;
}
int main() {
    tests();
    while(1){}
    return 0;
}
