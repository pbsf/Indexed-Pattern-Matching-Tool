/**
 * Auxiliar class to write/read bits in/from file.
 * DO NOT USE fclose to close a file, use instead the close_file()
 * from this class.
 *
 * Also responsible for debugging.
 */

bool DEBUG = true;

void debug(string msg) {
    if (DEBUG)
        cout << msg << endl;
}

void debug(int msg) {
    if (DEBUG)
        cout << msg << endl;
}

void debug(char msg) {
    if (DEBUG)
        cout << msg << endl;
}

void debug(char* msg) {
    if (DEBUG)
        cout << msg << endl;
}

void print_vector(vector<bool> b) {
    for (vector<bool>::const_iterator it = b.begin(); it != b.end(); it++) {
        cout << *it;
    }
    cout << endl;
}

int current_bit = 0;
unsigned char bit_buffer;
void write_bit (int bit, FILE *f) {
    if (bit)
        bit_buffer |= (1<<current_bit);

    current_bit++;
    if (current_bit == 8) {
        fwrite (&bit_buffer, 1, 1, f);
        current_bit = 0;
        bit_buffer = 0;
    }
}

void flush_bits (FILE *f) {
    while (current_bit)
        write_bit (0, f);
}

// We must flush bits to complete a byte before closing the file.
void close_file(FILE *f) {
    flush_bits(f);
    fclose(f);
}

// Source: http://stackoverflow.com/questions/2844817/how-do-i-check-if-a-c-string-is-an-int
inline bool isInteger(const std::string & s) {
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}

