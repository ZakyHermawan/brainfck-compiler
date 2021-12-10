#include "scanner.hpp"
#include "parser.hpp"
#include "semantic.hpp"
#include <map>
#include <string>
#include <utility>
#include <cstring>
#include <climits>
#include <stdexcept>
#define mp std::make_pair
#define SYM_SIZE 30000
using Key = std::pair<char, Tok>;
using PTable = std::map<Key, std::string>;
using ll = long long int;
PTable parse_table;
TokVect tokens;

ll symbol_table[SYM_SIZE];

void context_check(ll sym[SYM_SIZE], TokVect& tokens) {
  memset(sym, 0, SYM_SIZE);
  // begin at position 0
  int pos = 0;
  
  // index for read tokens
  int index = 0;
  int ctr = 0;
  while(index < tokens.size()) {
    ++ctr;
    if(ctr == 30) {
      exit(-1);
    }
    Tok now = tokens[index];
    printf("%d %lld\n", now, sym[pos]);
    // aksi semantik
    switch(now) {
      case tok_eof:
        return;
        break;
      case tok_newline:
        break;
      case tok_opSquare:
        if(sym[pos] == 0) {
          printf("End\n");
          while(index < tokens.size() and tokens[index] != tok_clSquare) {
            ++index;
          }
          if(index == tokens.size()) {
            throw std::runtime_error("Missing close square bracket\n");
          }
        }
        break;
      case tok_clSquare:
        if(sym[pos] != 0) {

          while(index > 0 and tokens[index] != tok_opSquare) {
            --index;
          }
          if(index == 0 && tokens[index] != tok_opSquare) {
            throw std::runtime_error("Error, missing open square bracket\n");
          }
          --index;
        }
        break;
      case tok_gt:
        if(pos == SYM_SIZE-1) {
          throw std::runtime_error("Error, you only have 30000 block of memory\n");
        }
        ++pos;
        break;
      case tok_lt:
        if(pos == 0) {
          throw std::runtime_error("Error, Memory index cannot be zero\n");
        }
        --pos;
        break;
      case tok_plus:
        if(sym[pos] == LLONG_MAX-1) {
          char buff[100];
          snprintf(buff, sizeof(buff), "Error, maximum size of a block of memory is %lld", LLONG_MAX);
          throw std::runtime_error(buff);
        }
        ++sym[pos];
        break;
      case tok_min:
        if(sym[pos] == LLONG_MIN+1) {
          char buff[100];
          // char* fmt = "Error, minimum size of a block of memory is %ll"
          // int fsize = snprintf(nullptr, 0, fmt, LONG_MIN);

          snprintf(buff, sizeof(buff), "Error, minimum size of a block of memory is %lld", LLONG_MIN);
          throw std::runtime_error(buff);
        }
        --sym[pos];
        break;
      case tok_com:
        break;
      case tok_dot:
        break;
      case tok_dollar:
        break;
    } 
    ++index;
  }
  

}

int main(int argc, char* argv[]) {
  if(argc != 2) {
    fprintf(stderr, "Usage: bfc <filename>\n");
    return -1;
  }

  std::string fileName(argv[1]);
  Source src(fileName);

  // Lexical Analysis
  while(1) {
    int val = getToken(src);
    // printf("%d", val);
    tokens.push_back(val);
    
    if(!val) break;
  }
  // test scanner
  for(const auto& v: tokens) {
      printf("%d", v);
  }
  printf("\n");

  
  Stack s(tokens);
  construct_table(parse_table);
  s.start_parsing(parse_table);
  context_check(symbol_table, tokens);
  
  printf("Analisis Selesai\n");
  return 0;

}
