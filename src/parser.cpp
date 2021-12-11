#include "scanner.hpp"
#include "parser.hpp"
#include <map>
#include <string>
#include <utility>
#include <cstring>
#define mp std::make_pair
using Key = std::pair<char, Tok>;
using PTable = std::map<Key, std::string>;
// PTable parse_table;

Stack::Stack(TokVect& tv) {
  m_idx_top = 0;
  m_token = tv;
  // remove EOF and newline at the end 
  m_token.pop_back();
  m_token.pop_back();
  m_token.push_back(tok_dollar);
  
}

void Stack::init_parsing() {
  m_stack.push_back('$');
  m_stack.push_back('E');
  ++m_idx_top;
}

void Stack::start_parsing (PTable& parse_table) {
  init_parsing();
  int tok_idx = 0;
  int ctr = 0;
  while(1) {
    
    char top = peek();
    Tok f = m_token[tok_idx];


    
    if(top == '+' && f == tok_plus) {
      pop();
      ++tok_idx;
      continue;
    }
    else if(top == '-' && f == tok_min) {
      pop();
      ++tok_idx;
      continue;
    }
    else if(top == '[' && f == tok_opSquare) {
      pop();
      ++tok_idx;
      continue;
    }
    else if(top == ']' && f == tok_clSquare) {
      pop();
      ++tok_idx;
      continue;
    }
    else if(top == '<' && f == tok_lt) {
      pop();
      ++tok_idx;
      continue;
    }
    else if(top == '>' && f == tok_gt) {
      pop();
      ++tok_idx;
      continue;
    }
    else if(top == ',' && f == tok_com) {
      pop();
      ++tok_idx;
      continue;
    }
    else if(top == '.' && f == tok_dot) {
      pop();
      ++tok_idx;
      continue;
    }
    // Finish condition
    // because F->epsilon defined on pada M[F, $]
    // for M parse table
    // If thr last condition is: $F, the string is accepted
    else if(top == 'F' && tok_idx == m_token.size()-1) {
      pop();
      top = peek();
      return;
    }
    

    
    
    std::string s = parse_table[mp(top, f)];
    
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
      exit(-1);
      return;    
    }

    
  }
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
    return;  
  }
  m_stack.pop_back();
  --m_idx_top;
}

void Stack::printStack() {
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
void construct_table(PTable& parse_table) {
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


