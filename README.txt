Esse é o README.txt da ferramenta IPMT, que indexa um arquivo para buscas por
padrões futuras.

Grupo:
João Guilherme Farias Duda
Paulo de Barros e Silva Filho
Raul Maia Falcão

Requisitos:
1. g++:
$ sudo apt-get install g++

Testado com clang na seguinte versão:
Apple LLVM version 5.1 (clang-503.0.40) (based on LLVM 3.4svn)

Para compilar, executar na raiz do projeto:
$ make
(O executável ipmt será gerado na pasta bin.)

Para ver todos os possíveis modos da ferramenta, também na raiz do projeto:
$ bin/ipmt -h

=============================================================================
Abaixo está o menu de help da ferramenta:

ipmt has four possible modes: index, search, compress and decompress. The index mode indexes a file passed as argument
in a compressed file with an .idx extension:
   ipmt index [OPTIONS] textfile
The search mode searches for a text pattern passed as argument in the indexed file passed as argument:
   ipmt search [-c, --pattern=FILE] pattern indexfile
The compress mode compresses a file passed as argument:
   ipmt compress textfile
The decompress mode decompresses a file passed as argument in a file with the same name but the .decompressed extension:
   ipmt decompress indexfile

-------------------------------------------
Options:
-h, --help: Shows this.
-p, --pattern: Can only be used on the search mode to pass as argument the path to a file
             containing a list of patterns. If this argument is used, no pattern should
             be passed as argument before the indexfile.
-c, --count: Can only be used on the search mode to print only the number of occurrences
             instead of all lines containing occurrences.
-------------------------------------------
Example of a index mode call for a file named file.txt:
   ipmt index file.txt
Example of a search to the pattern "herself" in the indexed file above:
   ipmt search herself file.idx

