//
// Created by yanick on 22-12-14.
//

#include "num.h"

#include "../kaleidoscope/kaleidoscope.h"

llvm::Value* NumberExprAST::codegen() {
    KSDbgInfo.emitLocation(this);
    return llvm::ConstantFP::get(*TheContext, llvm::APFloat(Val));
}