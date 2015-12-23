#include <bitset>

using namespace std;

//Reference: https://msdn.microsoft.com/en-us/library/ee916854.aspx

//Sizes in bits
#define OFFSET_SIZE 12
#define LENGTH_SIZE 6
#define BYTE_SIZE 8

struct token{
bitset<OFFSET_SIZE> offset;
bitset<LENGTH_SIZE> length;
bitset<BYTE_SIZE> byte;
};
