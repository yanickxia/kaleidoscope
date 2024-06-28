//
// Created by yanick on 22-12-14.
//

#ifndef KALEIDOSCOPE_VARIABLE_H
#define KALEIDOSCOPE_VARIABLE_H

#include "../kaleidoscope/kaleidoscope.h"
#include "../log/log.h"
#include "ast.h"
#include <string>
/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
  std::string Name;

public:
  VariableExprAST(const std::string &Name) : Name(Name) {}
  llvm::Value *codegen() override;
};

#endif // KALEIDOSCOPE_VARIABLE_H
