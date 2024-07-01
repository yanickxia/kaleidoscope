//
// Created by yanick on 24-7-1.
//

#include "unary.h"

#include "function.h"
#include "../log/log.h"

llvm::Value* UnaryExprAST::codegen() {
    llvm::Value* OperandV = Operand->codegen();
    if (!OperandV)
        return nullptr;

    llvm::Function* F = getFunction(std::string("unary") + Opcode);
    if (!F)
        return LogErrorV("Unknown unary operator");

    return Builder->CreateCall(F, OperandV, "unop");
}