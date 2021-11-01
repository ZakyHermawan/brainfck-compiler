#pragma once

#include <string>
#include <vector>

enum Token {
  tok_eof,
  tok_newline,
  tok_opSquare,
  tok_clSquare,
  tok_gt,
  tok_lt,
  tok_plus,
  tok_min,
  tok_com,
  tok_dot
};

class Source {
private:
  int m_curr_pos;
  int m_curr_line;
  int m_last_pos;
  int m_last_line;
  bool m_eof;
  std::vector<std::string> m_vect;

public:
  Source() = default;
  Source(std::string& fileName) {
    readFile(fileName);
  }

  void readFile(std::string& fileName);
  char getChar();
  bool iseof();

  int get_curr_pos();
  int get_curr_line();

  int get_last_pos();
  int get_last_line();

  void printVector();
};


bool knownChar(char c);
int getToken(Source& src);

// void readToVector(std::vector<std::string>& lines,const std::string& fileName);
