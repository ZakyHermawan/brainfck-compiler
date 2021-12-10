#pragma once

#include "scanner.hpp"
#include <vector>
#include <stdio.h>
#include <utility>
#include <map>
using Tok = int;
using TokVect = std::vector<Tok>;
using Key = std::pair<char, Tok>;
using PTable = std::map<Key, std::string>;

class Stack {
private:
    std::vector<char> m_stack;
    TokVect m_token;
    int m_idx_top;
public:
    Stack() = default;
    // Stack(TokVect) = delete;
    // Stack(TokVect&) = delete;
    Stack(TokVect&);
    virtual ~Stack() = default;

    void init_parsing();
    void start_parsing(PTable&);
    void printStack();
    Tok peek();
    void pop();
    void push(char);

};

void construct_table(PTable&);

