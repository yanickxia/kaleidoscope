//
// Created by yanick on 22-12-14.
//

#ifndef KALEIDOSCOPE_VARIABLE_H
#define KALEIDOSCOPE_VARIABLE_H

#include <string>

#include "../kaleidoscope/kaleidoscope.h"
#include "../log/log.h"
#include "ast.h"

llvm::AllocaInst* CreateEntryBlockAlloca(llvm::Function* TheFunction, llvm::StringRef VarName);

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
    std::string Name;

public:
    VariableExprAST(const std::string& Name) : Name(Name) {}

    llvm::Value* codegen() override;

    const std::string& getName() const {
        return Name;
    }
};

#endif // KALEIDOSCOPE_VARIABLE_H
