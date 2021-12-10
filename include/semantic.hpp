#pragma once

#include "scanner.hpp"
#include "parser.hpp"
#include <vector>
#include <stdio.h>
using ll = long long int;

void context_check(ll symbol_table[30000], TokVect& tokens);

