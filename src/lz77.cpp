#include <bitset>
#include <iostream>
#include <string>

using namespace std;

//Reference: https://msdn.microsoft.com/en-us/library/ee916854.aspx

//Sizes in bits
//TODO - Parametize the sizes
#define OFFSET_SIZE 12
#define LENGTH_SIZE 6
#define BYTE_SIZE 8

struct token{
bitset<OFFSET_SIZE> offset;
bitset<LENGTH_SIZE> length;
bitset<BYTE_SIZE>* byte;
};



void decompressor_tests(){

  struct token tokens[7];
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
  tokens[6] = t;



}

void tests(){
  decompressor_tests();
  return;
}
int main() {
    tests();
    while(1){}
    return 0;
}
