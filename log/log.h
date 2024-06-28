//
// Created by yanick on 22-12-14.
//

#ifndef KALEIDOSCOPE_LOG_H
#define KALEIDOSCOPE_LOG_H

#include <cstdio>
#include <memory>

#include "../ast/ast.h"
#include "../ast/prototype.h"
#include "../llvm/llvm.h"

/// LogError* - These are little helper functions for error handling.
std::unique_ptr<ExprAST> LogError (const char *Str);

std::unique_ptr<PrototypeAST> LogErrorP (const char *Str);

llvm::Value *LogErrorV (const char *Str);

#endif // KALEIDOSCOPE_LOG_H
