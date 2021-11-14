#pragma once

#include "scanner.hpp"
#include <vector>
#include <stdio.h>
using Tok = int;
using TokVect = std::vector<Tok>;

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
    void start_parsing();
    void printStack();
    Tok peek();
    void pop();
    void push(char);

};

