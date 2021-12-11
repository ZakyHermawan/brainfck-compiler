#include "scanner.hpp"

#include <string>
#include <exception>
#include <cstdio>
#include <iostream>
#include <vector>
#include <fstream>


void logErr(const char* errMsg, int row, int col) {
  fprintf(stderr, "Error at %d %d\n%s\n", row, col, errMsg);
  return;
}

void Source::readFile(std::string& fileName) {
  m_curr_line = m_curr_pos = 0;
  
  std::ifstream src;
  src.open(fileName);
  if(src.fail()) {
    fprintf(stderr, "File did not exist\n");
    throw std::exception();
  }
  std::string line;
  
  char c;
  std::vector<char> buf;
  // to handle eof at the end of file
  buf.push_back('\n');
  
  while(src >> std::noskipws >> c) {
    buf.push_back(c);
    if (c == '\n') {
      std::string s(buf.begin(), buf.end());
      buf.clear();
      m_vect.push_back(s);
    }
  }
  
  if(m_vect.size() == 0) {
    m_last_line = m_last_pos = 0;
    m_eof = true;
    return;
  }
  m_last_line = m_vect.size() - 1;
  m_last_pos = m_vect[m_last_line].size() - 1;  
  m_eof = false;
}

char Source::getChar() {
  char c = m_vect[m_curr_line][m_curr_pos];
  if(m_curr_pos < m_vect[m_curr_line].size()-1) {
    ++m_curr_pos;
  }
  // end of line
  else {
    if(m_curr_line < m_last_line) {
      m_curr_pos = 0;
      ++m_curr_line;
    }
    else {
      m_eof = true;
      
    }
  }
  return c;
}

bool Source::iseof() {
  return m_eof;
}

void Source::printVector() {
  for(int i=0; i<m_vect.size(); ++i) {
    for(int j=0; j<m_vect[i].size(); ++j) {
      printf("%c ", m_vect[i][j]);
    }
    printf("\n");
  }
}

bool knownChar(char c) {
  //  c == '\n' diubah jadi unknown
  if(
    c == '[' || c == ']' || c == '>' || c == '<' ||
    c == '+' || c == '-' || c == ',' || c == '.' 
  ) {
    return true;
  }
  return false;
}

int getToken(Source& src) {
  char last_char = ' ';
  // semua karakter lain akan di ignore
  while(!src.iseof() && !knownChar(last_char)) {
    last_char = src.getChar();
  }
  
  switch(last_char) {
    case '[': return tok_opSquare;
    case ']': return tok_clSquare;
    case '>': return tok_gt;
    case '<': return tok_lt;
    case '+': return tok_plus;
    case '-': return tok_min;
    case ',': return tok_com;
    case '.': return tok_dot;
    case '\n': return tok_newline;
    default: return tok_eof;
  }
  
}
