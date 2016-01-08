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

// MODE:
// < 1 or > 4 = unknown
// 1 = index
// 2 = search
// 3 = compress
// 4 = decompress
int MODE = 0;

char* pvalue = NULL;    // Filepath to a file containing patterns.
char* compvalue = NULL; // Compression value (lz77 or lz78)
char* idxvalue = NULL;  // Index value (suffixtree or suffixarray)
vector<string> patList; // List of patterns we are searching for.

const char *help_text = "ipmt has four possible modes: index, search, compress and decompress. The index mode indexes a file passed as argument\n"
                        "in a compressed file with an .idx extension: \n"
                    "   ipmt index [OPTIONS] textfile\n"
                    "The search mode searches for a text pattern passed as argument in the indexed file passed as argument:\n"
                    "   ipmt search [-c, --pattern=FILE] pattern indexfile\n"
                    "The compress mode compresses a file passed as argument: \n"
                    "   ipmt compress [--compression=lz77,lz78] textfile\n"
                    "The decompress mode decompresses a file passed as argument in a file with the same name but the .decompressed extension: \n"
                    "   ipmt decompress indexfile\n"
                    "\n"
                    "-------------------------------------------\n"
                    "Options:\n"
                    "-h, --help: Shows this.\n"
                    "-p, --pattern: Can only be used on the search mode to pass as argument the path to a file\n"
                    "             containing a list of patterns. If this argument is used, no pattern should\n"
                    "             be passed as argument before the indexfile.\n"
                    "-c, --count: Can only be used on the search mode to print only the number of occurrences\n"
                    "             instead of all lines containing occurrences.\n"
                    "-z, --compression: Receives as argument either lz77 or lz78, the algorithm used for\n"
                    "             compression/decompression. If a file is compressed by one of these\n"
                    "             algorithms, it can only be decompressed by the same algorithm.\n"
                    "             lz78 is the default.\n"
                    "-i, --indextype: Receives as argument either suffixtree or suffixarray, the algorithm\n"
                    "             used for indexing and matching. suffixarray is the default.\n"
                    "-------------------------------------------\n"
                    "Example of a index mode call for a file named file.txt:\n"
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
pair<string, list<int>*> decode_index(string& filepath) {
    string decoded = decode_file(filepath);
    stringstream f(decoded);
    stringstream text;
    string line;
    getline(f, line);
    int number_of_lines = atoi(line.c_str());
    for (int i = 0; i < number_of_lines && getline(f, line); i++) {
        text << line;
        text << '\n';
    }
    list<int>* idx_list = new list<int>;
    string buf; // Have a buffer string
    while (f >> buf) {
        if (isInteger(buf.c_str()))
            idx_list->push_back(atoi(buf.c_str()));
    }
    return make_pair(text.str(), idx_list);
}

pair<string, list<int>*> decompress(string& filepath) {
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
    return make_pair("", new list<int>);    // Unreachable code.
}

SuffixArray* sa = NULL; // Pointer to SA, to make sure it is not created more than once.
void search(string& text, list<int>* idx_list, string& pattern) {
    cout << "Searching for pattern: " << pattern << endl;
    if (idxflag == 1) {
        if (!strcmp(idxvalue, "suffixtree")) {
            // TODO: Call suffix tree algorithm.
            unsupported_operation("Suffix tree search");
        } else if (!strcmp(idxvalue, "suffixarray")) {
            if (sa == NULL) {
                sa = new SuffixArray(text);
                sa->SA = *idx_list;
            }
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
        if (sa == NULL) {
            sa = new SuffixArray(text);
            sa->SA = *idx_list;
        }
        int count = sa->countMatches(pattern);
        if (cflag == 1) {
            cout << "Count: " << count << endl;
        } else {
            sa->printMatches();
        }
    }
}

void decompress_and_search(string& filename) {
    pair<string, list<int>*> decompress_result = decompress(filename);
    for (int i = 0; i < patList.size(); ++i) {
        search(decompress_result.first, decompress_result.second, patList[i]);
    }
}

list<int> index(string& filename) {
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

unsigned int count_lines(const string& s) {
    int newlines = 0;
    int size = s.size();
    for (int i = 0; i < size; i++ ) {
        if (s[i] == '\n') {
            newlines++;
        }
    }
    return newlines;
}

void encode_index(list<int>& idx_list, string& output_file, string& orig_file) {
    ifstream t(orig_file.c_str());
    stringstream buffer;
    buffer << t.rdbuf();
    const string &orig_text = buffer.str();
    int size = count_lines(orig_text);
    buffer.seekp(0);
    buffer << size;
    buffer << '\n';
    buffer << orig_text;

    for(list<int>::iterator j = idx_list.begin(); j != idx_list.end(); ++j){
        buffer << *j << " ";
    }
    string str = buffer.str();
    encode_text(str, output_file);
}

void decode_f(string& orig_file) {
    string output = decode_file(orig_file);
    ofstream out(orig_file + ".decomp");
    out << output;
    out.close();
}

void compress_file(string& orig_file) {
    if (compflag == 1) {
        if (!strcmp(compvalue, "lz78")) {
            encode_file(orig_file);
        } else if (!strcmp(compvalue, "lz77")) {
            // TODO: Call lz77 algorithm.
            unsupported_operation("lz77 compress");
        } else {
            printf("Unrecognized --compression argument: %s\n", compvalue);
            print_help();
        }
    } else {
        encode_file(orig_file);
    }
}

void compress(string& filepath, list<int>& to_compress, string& orig_file) {
    if (compflag == 1) {
        if (!strcmp(compvalue, "lz78")) {
            encode_index(to_compress, filepath, orig_file);
        } else if (!strcmp(compvalue, "lz77")) {
            // TODO: Call lz77 algorithm.
            unsupported_operation("lz77 compress");
        } else {
            printf("Unrecognized --compression argument: %s\n", compvalue);
            print_help();
        }
    } else {
        encode_index(to_compress, filepath, orig_file);
    }
}

void index_and_compress(string& filename) {
    list<int> to_compress = index(filename);
    // Finding new filename -> .txt extension
    size_t lastindex = filename.find_last_of(".");
    string new_name = filename.substr(0, lastindex) + ".idx";
    cout << "Compressing in file: " << new_name << endl;
    compress(new_name, to_compress, filename);
}

int main (int argc, char **argv) {
    int c;
    if (argc <= 2) {
        print_help();
    } else if (!strcmp(argv[1],"index")) {
        MODE = 1;
    } else if (!strcmp(argv[1], "search")) {
        MODE = 2;
    } else if (!strcmp(argv[1], "compress")) {
        MODE = 3;
    } else if (!strcmp(argv[1], "decompress")) {
        MODE = 4;
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
    string last_arg = string(argv[argc-1]);
    if (MODE == 2) {
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
        decompress_and_search(last_arg);
    } else if (MODE == 1) {
        index_and_compress(last_arg);
    } else if (MODE == 3) {
        compress_file(last_arg);
    } else if (MODE == 4) {
        decode_f(last_arg);
    } else {
        printf("Unexpected block being called. What event is this?");
    }
    return 0;
}

