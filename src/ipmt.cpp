using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include "LSA.cpp"
#include "lz78.cpp"
#include <cstring>
/* ipmt command-line interface
   Reference: http://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html
*/

int pflag = 0; // -p or --pattern
int cflag = 0; // -c or --count
int compflag = 0; // --compression
int idxflag = 0; // --indextype

int indexmode = 0;
int searchmode = 0;

char* pvalue = NULL;    // Filepath to a file containing patterns.
char* compvalue = NULL; // Compression value (lz77 or lz78)
char* idxvalue = NULL;  // Index value (tree or array)
vector<string> patList; // List of patterns we are searching for.

const char *help_text = "ipmt has two possible modes: index and search. The first mode indexes a file passed as argument\n"
                        "in a compressed file with an .idx extension: \n"
                    "   ipmt index [OPTIONS] textfile\n"
                    "The second mode searches for a text pattern passed as argument in the indexed file passed as argument:\n"
                    "   ipmt search [OPTIONS] pattern indexfile\n"
                    "\n"
                    "-------------------------------------------\n"
                    "Options:\n"
                    "-h, --help: Shows this.\n"
                    "-p, --pattern: Can be used on the search mode to pass as argument the path to a file\n"
                    "             containing a list of patterns.\n"
                    "-c, --count: Can be used on the search mode to print only the number of occurrences\n"
                    "             instead of all lines containing occurrences.\n"
                    "-z, --compression: Receives as argument either lz77 or lz78, the algorithm used for\n"
                    "             compression/decompression. lz78 is the default.\n"
                    "-i, --indextype: Receives as argument either suffixtree or suffixarray, the algorithm\n"
                    "             used for indexing and matching. suffixarray is the default.\n"
                    "-------------------------------------------\n"
                    "Example of index mode call for a file named file.txt:\n"
                    "   ipmt index file.txt\n"
                    "Example of a search to the pattern \"herself\" in the indexed file above:\n"
                    "   ipmt search herself file.idx\n";

void print_help() {
    cout << help_text << endl;
    exit(0);
}

void unsupported_operation(string s) {
    cout << endl << "Aborting due to unsupported operation: " << s << endl;
    exit(0);
}

list<int>* decompress(string filepath) {
    if (compflag == 1) {
        if (!strcmp(compvalue, "lz78")) {
            return decode_index(filepath);
        } else if (!strcmp(compvalue, "lz77")) {
            // TODO: Call lz77 algorithm.
            unsupported_operation("lz77 decompress");
        } else {
            printf("Unrecognized --compression argument: %s\n", compvalue);
            print_help();
        }
    } else {
        return decode_index(filepath);
    }
    return new list<int>;    // Unreachable code.
}

void search(string filename, list<int>* idx_list, string pattern) {
    cout << "Searching for pattern: " << pattern << endl;
    if (idxflag == 1) {
        if (!strcmp(idxvalue, "suffixtree")) {
            // TODO: Call suffix tree algorithm.
            unsupported_operation("Suffix tree search");
        } else if (!strcmp(idxvalue, "suffixarray")) {
            SuffixArray* sa = create_sa_from_file(filename);
            sa->SA = *idx_list;
            int count = sa->countMatches(pattern);
            if (cflag == 1) {
                cout << "Count: " << count << endl;
            } else {
                sa->printMatches();
            }
        } else {
            printf("Unrecognized --indextype argument: %s\n", idxvalue);
            print_help();
        }
    } else {
        SuffixArray* sa = create_sa_from_file(filename);
        sa->SA = *idx_list;
        int count = sa->countMatches(pattern);
        if (cflag == 1) {
            cout << "Count: " << count << endl;
        } else {
            sa->printMatches();
        }
    }
}

void decompress_and_search(string filename) {
    list<int>* idx_list = decompress(filename);

    // Finding new filename -> .txt extension
    size_t lastindex = filename.find_last_of(".");
    string new_name = filename.substr(0, lastindex) + ".txt";
    for (int i = 0; i < patList.size(); ++i) {
        search(new_name, idx_list, patList[i]);
    }
}

list<int> index(string filename) {
    if (idxflag == 1) {
        if (!strcmp(idxvalue, "suffixtree")) {
            // TODO: Call suffix tree algorithm.
            unsupported_operation("Suffix tree index");
        } else if (!strcmp(idxvalue, "suffixarray")) {
            return index_file(filename);
        } else {
            printf("Unrecognized --indextype argument: %s\n", idxvalue);
            print_help();
        }
    } else {
        return index_file(filename);
    }
    return *(new list<int>); // Unreachable code.
}

void compress(string filepath, list<int>& to_compress) {
    if (compflag == 1) {
        if (!strcmp(compvalue, "lz78")) {
            encode_index(to_compress, filepath);
        } else if (!strcmp(compvalue, "lz77")) {
            // TODO: Call lz77 algorithm.
            unsupported_operation("lz77 compress");
        } else {
            printf("Unrecognized --compression argument: %s\n", compvalue);
            print_help();
        }
    } else {
        encode_index(to_compress, filepath);
    }
}

void index_and_compress(string filename) {
    list<int> to_compress = index(filename);
    // Finding new filename -> .txt extension
    size_t lastindex = filename.find_last_of(".");
    string new_name = filename.substr(0, lastindex) + ".idx";
    cout << "Creating index file: " << new_name << endl;
    compress(new_name, to_compress);
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

      c = getopt_long (argc, argv, "p:cz:i:h",     // Having a ':' means that it expects an arg following.
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
        if (pvalue == NULL) {
            // If no pattern file was passed as arg, consider the before-last
            // arg as the pattern.
            patList.push_back(string(argv[argc-2]));
        } else {
            // Otherwise, parse the file and call the search function for each
            // pattern.
            ifstream infile(pvalue);
            string line;
            while (getline(infile, line)) {
                patList.push_back(line);
            }
            infile.close();
        }
        decompress_and_search(string(argv[argc-1]));
    } else if (indexmode == 1) {
        if (pvalue == NULL) pvalue = argv[argc-2];
        index_and_compress(string(argv[argc-1]));
    } else {
        printf("Unexpected block being called. What event is this?");
    }
    return 0;
}

