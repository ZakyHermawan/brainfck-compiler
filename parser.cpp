#include "scanner.hpp"
#include "parser.hpp"
#include <map>
#include <string>
#include <utility>
#include <cstring>
#define mp std::make_pair
using Key = std::pair<char, Tok>;
using PTable = std::map<Key, std::string>;
PTable parse_table;
TokVect tokens;

Stack::Stack(TokVect& tv) {
  m_idx_top = 0;
  m_token = std::move(tv);
  // remove EOF and newline at the end 
  m_token.pop_back();
  m_token.pop_back();
  m_token.push_back(tok_dollar);
  
  printf("\n%d %d\n", m_token[0], m_token[5]);
}

void Stack::init_parsing() {
  m_stack.push_back('$');
  m_stack.push_back('E');
  ++m_idx_top;
}

void Stack::start_parsing () {
  init_parsing();
  int tok_idx = 0;
  int ctr = 0;
  while(1) {
    
    char top = peek();
    Tok f = m_token[tok_idx];

    printf("Token: %d\n", f);
    printf("Top stack: %c\n", top);
    printStack();    
    
    if(top == '+' && f == tok_plus) {
      pop();
      ++tok_idx;
      printf("Token match, next!\n");
      continue;
    }
    else if(top == '-' && f == tok_min) {
      pop();
      ++tok_idx;
      printf("Token match, next!\n");
      continue;
    }
    else if(top == '[' && f == tok_opSquare) {
      pop();
      ++tok_idx;
      printf("Token match, next!\n");
      continue;
    }
    else if(top == ']' && f == tok_clSquare) {
      pop();
      ++tok_idx;
      printf("Token match, next!\n");
      continue;
    }
    else if(top == '<' && f == tok_lt) {
      pop();
      ++tok_idx;
      printf("Token match, next!\n");
      continue;
    }
    else if(top == '>' && f == tok_gt) {
      pop();
      ++tok_idx;
      printf("Token match, next!\n");
      continue;
    }
    else if(top == ',' && f == tok_com) {
      pop();
      ++tok_idx;
      printf("Token match, next!\n");
      continue;
    }
    else if(top == '.' && f == tok_dot) {
      pop();
      ++tok_idx;
      printf("Token match, next!\n");
      continue;
    }
    // Finish condition
    // because F->epsilon defined on pada M[F, $]
    // for M parse table
    // If thr last condition is: $F, the string is accepted
    else if(top == 'F' && tok_idx == m_token.size()-1) {
      pop();
      top = peek();
      printf("%c\n", top);
      printf("Accepted!\n");
      return;
    }
    

    
    
    std::string s = parse_table[mp(top, f)];
    printf("Hasil produksi CFG: %s\n", s.c_str());
    
    printf("\n");
    if(s.compare("D -> ><+-.,") == 0) {      
      pop();
      if(f == tok_plus) {
        push('+');
      }
      else if(f == tok_min) {
        push('-');
      }
      else if(f == tok_lt) {
        push('<');
      }
      else if(f == tok_gt) {
        push('>');
      }
      else if(f == tok_dot) {
        push('.');
      }
      else{
        push(',');
      }
    }
    else if(s.compare("E -> D F") == 0) {
      pop();
      push('F');
      push('D');
    }
    else if(s.compare("E -> [E]F") == 0) {
      pop();
      push('F');
      push(']');
      push('E');
      push('[');
    }
    else if(s.compare("F -> E") == 0) {
      pop();
      push('E');
    }
    else if(s.compare("F -> epsilon") == 0) {
      pop();
    }
    else {
      printf("Wrong Syntax\n");
      return;    
    }

    printf("%s\n", s.c_str());
    
  }
  printf("Selesai\n");
  return;
}

Tok Stack::peek() {
  return m_stack[m_idx_top];
}

void Stack::push(char c) {
  m_stack.push_back(c);
  ++m_idx_top;
}

void Stack::pop() {
  if(m_idx_top == -1) {
    printf("Stack is empty!");
    return;  
  }
  m_stack.pop_back();
  --m_idx_top;
}

void Stack::printStack() {
  printf("Kondisi stack: ");
  for(const auto& stack: m_stack) {
    printf("%c", stack);
  }
  printf("\n");
}

/*
    |  ><+-.,  |   [    |    ]   |
---+----------+--------+---------+
D | D -> ><+-., |        |         |
E |  E -> D F   | E -> [E]F |         |
F|    F -> E     |   F-> E    | F-> epsilon |
---+----------+--------+---------+
*/

// we use F for E'
void construct_table() {
  parse_table[mp('D', tok_gt)] = "D -> ><+-.,";
  parse_table[mp('D', tok_lt)] = "D -> ><+-.,";
  parse_table[mp('D', tok_plus)] = "D -> ><+-.,";
  parse_table[mp('D', tok_min)] = "D -> ><+-.,";
  parse_table[mp('D', tok_dot)] = "D -> ><+-.,";
  parse_table[mp('D', tok_com)] = "D -> ><+-.,";

  parse_table[mp('E', tok_gt)] = "E -> D F";
  parse_table[mp('E', tok_lt)] = "E -> D F";
  parse_table[mp('E', tok_plus)] = "E -> D F";
  parse_table[mp('E', tok_min)] = "E -> D F";
  parse_table[mp('E', tok_dot)] = "E -> D F";
  parse_table[mp('E', tok_com)] = "E -> D F";
  
  parse_table[mp('E', tok_opSquare)] = "E -> [E]F";
  
  parse_table[mp('F', tok_lt)] = "F -> E";
  parse_table[mp('F', tok_gt)] = "F -> E";
  parse_table[mp('F', tok_plus)] = "F -> E";
  parse_table[mp('F', tok_min)] = "F -> E";
  parse_table[mp('F', tok_dot)] = "F -> E";
  parse_table[mp('F', tok_com)] = "F -> E";

  parse_table[mp('F', tok_opSquare)] = "F -> E";
  parse_table[mp('F', tok_clSquare)] = "F -> epsilon";
}

int main(int argc, char* argv[]) {
  if(argc != 2) {
    fprintf(stderr, "Usage: bfc <filename>");
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
  construct_table();  
  s.start_parsing();
  
  printf("Analisis sintaks selesai\n");
  return 0;
}
