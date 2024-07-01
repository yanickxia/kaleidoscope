//
// Created by yanick on 22-12-14.
//

#include "binary.h"

#include "function.h"
#include "../kaleidoscope/kaleidoscope.h"
#include "../log/log.h"

llvm::Value* BinaryExprAST::codegen() {
    llvm::Value* L = LHS->codegen();
    llvm::Value* R = RHS->codegen();
    if (!L || !R)
        return nullptr;

    switch (Op) {
        case '+':
            return Builder->CreateFAdd(L, R, "addtmp");
        case '-':
            return Builder->CreateFSub(L, R, "subtmp");
        case '*':
            return Builder->CreateFMul(L, R, "multmp");
        case '<':
            L = Builder->CreateFCmpULT(L, R, "cmptmp");
        // Convert bool 0/1 to double 0.0 or 1.0
            return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext), "booltmp");
        default:
            break;
    }

    // If it wasn't a builtin binary operator, it must be a user defined one. Emit
    // a call to it.
    llvm::Function* F = getFunction(std::string("binary") + Op);
    assert(F && "binary operator not found!");

    llvm::Value* Ops[2] = {L, R};
    return Builder->CreateCall(F, Ops, "binop");
}