//
// Created by yanick on 22-12-14.
//

#ifndef KALEIDOSCOPE_VARIABLE_H
#define KALEIDOSCOPE_VARIABLE_H

#include <string>
#include "ast.h"
#include "../kaleidoscope/kaleidoscope.h"
#include "../log/log.h"
/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
    std::string Name;

public:
    VariableExprAST(const std::string &Name) : Name(Name) {}
    llvm::Value *codegen() override;
};

#endif //KALEIDOSCOPE_VARIABLE_H
