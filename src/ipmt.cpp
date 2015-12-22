using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
/* ipmt command-line interface
   Reference: http://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html
*/

int pflag = 0; // -p or --pattern
int cflag = 0; // -c or --count
int compflag = 0; // --compression
int idxflag = 0; // --indextype

int indexmode = 0;
int searchmode = 0;

char* pvalue = NULL;
char* compvalue = NULL;
char* idxvalue = NULL;
// TODO:help_text below.
string help_text = "HELP TEXT";

void print_help() {
    cout << help_text << endl;
    exit(0);
}

// TODO: This method will probably return a byte array to be compressed.
void index(char* filename) {
    if (idxflag == 1) {
        if (!strcmp(idxvalue, "suffixtree")) {
            // TODO: Call suffix tree algorithm.
        } else if (!strcmp(idxvalue, "suffixarray")) {
            // TODO: Call suffix array algorithm.
        } else {
            printf("Unrecognized --indextype argument: %s\n", idxvalue);
            print_help();
        }
    } else {
        // TODO: Call either suffix tree or suffix array algorithm.
    }
}

void index_and_compress(char* filename) {
    // TODO
}

void decompress_and_search(char* filename) {
    // TODO
}

void search() {
    // TODO
}

void decompress() {
    // TODO
}

void compress() {
    if (compflag == 1) {
        if (!strcmp(compvalue, "lz78")) {
            // TODO: Call lz78 algorithm.
        } else if (!strcmp(compvalue, "lz77")) {
            // TODO: Call lz77 algorithm.
        } else {
            printf("Unrecognized --compression argument: %s\n", compvalue);
            print_help();
        }
    } else {
        // TODO: Call lz78
    }
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

    if (compflag == 1) {
        decompress_and_search(argv[argc-1]);
    } else if (idxflag == 1) {
        index_and_compress(argv[argc-1]);
    }
    return 0;
}

