//
// Created by yanick on 22-12-14.
//

#ifndef KALEIDOSCOPE_LOG_H
#define KALEIDOSCOPE_LOG_H

#include <memory>
#include <cstdio>
#include "../ast/ast.h"
#include "../ast/prototype.h"

/// LogError* - These are little helper functions for error handling.
std::unique_ptr<ExprAST> LogError(const char *Str);

std::unique_ptr<PrototypeAST> LogErrorP(const char *Str);

#endif //KALEIDOSCOPE_LOG_H
