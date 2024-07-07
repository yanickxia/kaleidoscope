//
// Created by yanick on 22-12-14.
//

#include "call.h"
#include "../log/log.h"
#include "function.h"

llvm::Value* CallExprAST::codegen() {
    KSDbgInfo.emitLocation(this);
    // Look up the name in the global module table.
    llvm::Function* CalleeF = getFunction(Callee);
    if (!CalleeF)
        return LogErrorV("Unknown function referenced");

    // If argument mismatch error.
    if (CalleeF->arg_size() != Args.size())
        return LogErrorV("Incorrect # arguments passed");

    std::vector<llvm::Value*> ArgsV;
    for (unsigned i = 0, e = Args.size(); i != e; ++i) {
        ArgsV.push_back(Args[i]->codegen());
        if (!ArgsV.back())
            return nullptr;
    }

    return Builder->CreateCall(CalleeF, ArgsV, "calltmp");
}