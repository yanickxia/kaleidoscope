//
// Created by yanick on 22-12-14.
//

#include "variable.h"

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
llvm::AllocaInst* CreateEntryBlockAlloca(llvm::Function* TheFunction, llvm::StringRef VarName) {
    llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
    return TmpB.CreateAlloca(llvm::Type::getDoubleTy(*TheContext), nullptr, VarName);
}

llvm::Value* VariableExprAST::codegen() {
    llvm::AllocaInst *A = NamedValues[Name];
    if (!A)
        LogErrorV("Unknown variable name");
    return Builder->CreateLoad(A->getAllocatedType(), A, Name.c_str());
}
