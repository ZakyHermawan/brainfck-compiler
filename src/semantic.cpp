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
using Key = std::pair<char, Tok>;
using PTable = std::map<Key, std::string>;
using ll = long long int;
PTable parse_table;
TokVect tokens;

int main(int argc, char* argv[]) {
  if(argc != 2) {
    fprintf(stderr, "Usage: bfc <filename>\n");
    return -1;
  }

  std::string fileName(argv[1]);
  ll symbol_table[SYM_SIZE];

  Source src(fileName);

  // Lexical Analysis
  while(1) {
    int val = getToken(src);
    // printf("%d", val);
    tokens.push_back(val);
    
    if(!val) break;
  }
  // test scanner
  /*
  for(const auto& v: tokens) {
      printf("%d", v);
  }
  printf("\n");
  */

  Stack s(tokens);
  construct_table(parse_table);
  s.start_parsing(parse_table);
  context_check(symbol_table, tokens);
  
  printf("Analisis Selesai\n");
  return 0;

}

void print_memory(ll sym[SYM_SIZE]) {
  for(int i=0; i<20; ++i) {
    printf("%lld ", sym[i]);
  }
  printf("\n");
}

void context_check(ll sym[SYM_SIZE], TokVect& tokens) {
  memset(sym, 0, SYM_SIZE);
  // begin at position 0
  int pos = 0;
  
  // index for read tokens
  int index = 0;

  while(index < tokens.size()) {
    Tok now = tokens[index];
    // aksi semantik
    switch(now) {
      case tok_eof:
        return;
        break;
      case tok_newline:
        break;
      case tok_opSquare:
        // ketika [ dan *ptr tidak 0, maka proses token selanjutnya
        // ketika [ dan *ptr = 0, maka akan searching
        // caranya:
        // loop dengan
        // level_now = 0
        // kalo ketemu [, maka ++level_now
        // kalo ketemu ] dan level_now > 0, maka --level_now
        // kalo ketemu ] dan level_now == 0, maka itulah pairnya, end loop
        // proses token selanjutnya
        if(sym[pos] == 0) {
          int level_now = 0;
          while(index < tokens.size()) {
            ++index;
            if(tokens[index] == tok_opSquare) {
              ++level_now;
            }
            else if(tokens[index] == tok_clSquare) {
              if(level_now > 0) {
                --level_now;
              }
              else {
                break;
              }
            }
          }
          if(index == tokens.size()) {
            throw std::runtime_error("Missing close square bracket\n");
          }
        }
        break;
      case tok_clSquare:
        // ketika ] dan 0, maka proses token selanjutnya
        // ketika ] dan tidak 0, maka
        // level_now = 0
        // loop
        // --index
        // kalo ketemu ], maka ++level_now
        // kalo ketemu [ dan level_now > 0, maka --level_now
        // kalo ketemu [ dan level_now = 0, itulah prinya, end loop
        // proses token selanjutnya
        //
 
        if(sym[pos] != 0) {
          int level_now = 0;
          while(index > 0) {
            --index;
            if(tokens[index] == tok_clSquare) {
              ++level_now;
            }
            else if(tokens[index] == tok_opSquare) {
              if(level_now > 0) {
                --level_now;
              }
              else {
                break;
              }
            }
          }
          if(index == 0 && tokens[index] != tok_opSquare) {
            throw std::runtime_error("Error, missing open square bracket\n");
          }
        }
        break;
      case tok_gt:
        if(pos == SYM_SIZE-1) {
          throw std::runtime_error("Error, you only have 30000 block of memory\n");
        }
        // ketika ] dan 0, maka proses token selanjutnya
        // ketika ] dan tidak 0, maka
        // curr_index = index
        // level_now = 0
        // loop
        // --index
        // kalo ketemu ], maka ++level_now
        // kalo ketemu [ dan level_now > 0, maka -- level_now
        // kalo ketemu [ dan level_now = 0, itulah prinya, end loop
        // proses token selanjutnya
        //
        ++pos;
        break;
      case tok_lt:
        if(pos == 0) {
          printf("Error at: %d\n", index);
          for(int i=0; i<20; ++i) {
            printf("%lld ", sym[i]);
          }
          printf("\n");

          throw std::runtime_error("Error, Memory index cannot be less than zero\n");
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


