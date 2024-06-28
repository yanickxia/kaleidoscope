//
// Created by yanick on 22-12-14.
//

#include "prototype.h"

llvm::Function* PrototypeAST::codegen () {
    // Make the function type:  double(double,double) etc.
    std::vector<llvm::Type*> Doubles (Args.size (), llvm::Type::getDoubleTy (*TheContext));
    llvm::FunctionType* FT =
    llvm::FunctionType::get (llvm::Type::getDoubleTy (*TheContext), Doubles, false);

    llvm::Function* F = llvm::Function::Create (
    FT, llvm::Function::ExternalLinkage, Name, TheModule.get ());
    // Set names for all arguments.
    unsigned Idx = 0;
    for (auto& Arg : F->args ()) {
        Arg.setName (Args[Idx++]);
    }

    return F;
}