using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include "LSA.cpp"
#include "lz78.cpp"
/* ipmt command-line interface
   Reference: http://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html
*/

int pflag = 0; // -p or --pattern
int cflag = 0; // -c or --count
int compflag = 0; // --compression
int idxflag = 0; // --indextype

int indexmode = 0;
int searchmode = 0;

char* pvalue = NULL;    // Pattern value (pattern) TODO: Handle list of patterns
char* compvalue = NULL; // Compression value (lz77 or lz78)
char* idxvalue = NULL;  // Index value (tree or array)

// TODO:help_text below.
string help_text = "HELP TEXT";

void print_help() {
    cout << help_text << endl;
    exit(0);
}

list<int>* decompress(string filepath) {
    if (compflag == 1) {
        if (!strcmp(compvalue, "lz78")) {
            return decode_index(filepath);
        } else if (!strcmp(compvalue, "lz77")) {
            // TODO: Call lz77 algorithm.
        } else {
            printf("Unrecognized --compression argument: %s\n", compvalue);
            print_help();
        }
    } else {
        return decode_index(filepath);
    }
}

// TODO: Currently only prints count. Must print all occuring lines like  grep.
void search(string filename, list<int>* idx_list) {
    if (idxflag == 1) {
        if (!strcmp(idxvalue, "suffixtree")) {
            // TODO: Call suffix tree algorithm.
        } else if (!strcmp(idxvalue, "suffixarray")) {
            SuffixArray* sa = create_sa_from_file(filename);
            sa->SA = *idx_list;
            cout << sa->countMatches(pvalue);
        } else {
            printf("Unrecognized --indextype argument: %s\n", idxvalue);
            print_help();
        }
    } else {
        SuffixArray* sa = create_sa_from_file(filename);
        sa->SA = *idx_list;
        cout << sa->countMatches(pvalue);
    }
}

void decompress_and_search(string filename) {
    list<int>* idx_list = decompress(filename);

    // Finding new filename -> .txt extension
    size_t lastindex = filename.find_last_of(".");
    string new_name = filename.substr(0, lastindex) + ".txt";
    search(new_name, idx_list);
}

list<int> index(string filename) {
    cout << "1" << endl;
    if (idxflag == 1) {
        if (!strcmp(idxvalue, "suffixtree")) {
            // TODO: Call suffix tree algorithm.
        } else if (!strcmp(idxvalue, "suffixarray")) {
            return index_file(filename);
        } else {
            printf("Unrecognized --indextype argument: %s\n", idxvalue);
            print_help();
        }
    } else {
        return index_file(filename);
    }
    cout << "foi" << endl;
}

void compress(string filepath) {
    if (compflag == 1) {
        if (!strcmp(compvalue, "lz78")) {
            encode_file(filepath);
        } else if (!strcmp(compvalue, "lz77")) {
            // TODO: Call lz77 algorithm.
        } else {
            printf("Unrecognized --compression argument: %s\n", compvalue);
            print_help();
        }
    } else {
        encode_file(filepath);
    }
}

void index_and_compress(char* filename) {
    list<int> to_compress = index(filename);
    encode_index(to_compress, filename);
}

int main (int argc, char **argv) {
  int c;
    if (argc <= 2) {
        print_help();
    } else if (!strcmp(argv[1],"index")) {
        indexmode = 1;
    } else if (!strcmp(argv[1], "search")) {
        searchmode = 1;
    } else {
        printf("Unexpected argument passed as input: '%s'\n", argv[0]);
        print_help();
    }

    while (1) {
      static struct option long_options[] = {
          {"pattern", required_argument, 0, 'p'},
          {"count",   no_argument,       0, 'c'},
          {"compression", required_argument, 0, 'z'},
          {"indextype",    required_argument, 0, 'i'},
          {"help",    no_argument, 0, 'h'},
          {0, 0, 0, 0}
        };
      /* getopt_long stores the option index here. */
      int option_index = 0;

      c = getopt_long (argc, argv, "pczi:h",
                       long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;
      switch (c) {
        case 'p':
            pflag = 1;
            pvalue = optarg;
            break;

        case 'c':
            cflag = 1;
            break;

        case 'z':
            compflag = 1;
            compvalue = optarg;
            break;

        case 'i':
            idxflag = 1;
            idxvalue = optarg;
            break;

        case 'h':
            print_help();

        default:
              abort ();
        }
    }

    if (searchmode == 1) {
        decompress_and_search(string(argv[argc-1]));
    } else if (indexmode == 1) {
        index_and_compress(argv[argc-1]);
    } else {
        printf("Unexpected block being called. What event is this?");
    }
    return 0;
}

