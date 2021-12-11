#pragma once

#include "scanner.hpp"
#include "parser.hpp"
#include <vector>
#include <stdio.h>
#define SYM_SIZE 30000
using ll = long long int;

void print_memory(ll sym[SYM_SIZE]);
void context_check(ll symbol_table[SYM_SIZE], TokVect& tokens);

