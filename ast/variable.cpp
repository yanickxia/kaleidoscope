//
// Created by yanick on 22-12-14.
//

#include "variable.h"

llvm::Value* VariableExprAST::codegen () {
    llvm::Value* V = NamedValues[Name];
    if (!V)
        LogErrorV ("Unknown variable name");
    return V;
}
