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
    VariableExprAST(SourceLocation Loc, const std::string& Name) : ExprAST(Loc), Name(Name) {}

    llvm::Value* codegen() override;

    const std::string& getName() const {
        return Name;
    }

    llvm::raw_ostream &dump(llvm::raw_ostream &out, int ind) override {
        return ExprAST::dump(out << Name, ind);
    }
};

#endif // KALEIDOSCOPE_VARIABLE_H