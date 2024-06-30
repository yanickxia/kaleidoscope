//
// Created by yanick on 24-6-30.
//

#include "if.h"

#include "../kaleidoscope/kaleidoscope.h"
#include "../llvm/llvm.h"

llvm::Value *IfExprAST::codegen () {
    llvm::Value *CondV = Cond->codegen ();
    if (!CondV)
        return nullptr;

    // Convert condition to a bool by comparing non-equal to 0.0.
    CondV = Builder->CreateFCmpONE (
    CondV, llvm::ConstantFP::get (*TheContext, llvm::APFloat (0.0)), "ifcond");

    llvm::Function *TheFunction = Builder->GetInsertBlock ()->getParent ();

    // Create blocks for the then and else cases.  Insert the 'then' block at
    // the end of the function.
    llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create (*TheContext, "then", TheFunction);
    llvm::BasicBlock *ElseBB = llvm::BasicBlock::Create (*TheContext, "else");
    llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create (*TheContext, "ifcont");

    Builder->CreateCondBr (CondV, ThenBB, ElseBB);

    // Emit then value.
    Builder->SetInsertPoint (ThenBB);

    llvm::Value *ThenV = Then->codegen ();
    if (!ThenV)
        return nullptr;

    Builder->CreateBr (MergeBB);
    // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
    ThenBB = Builder->GetInsertBlock ();

    // Emit else block.
    TheFunction->getBasicBlockList ().push_back (ElseBB);
    Builder->SetInsertPoint (ElseBB);

    llvm::Value *ElseV = Else->codegen ();
    if (!ElseV)
        return nullptr;

    Builder->CreateBr (MergeBB);
    // codegen of 'Else' can change the current block, update ElseBB for the PHI.
    ElseBB = Builder->GetInsertBlock ();
    // Emit merge block.
    TheFunction->getBasicBlockList ().push_back (MergeBB);
    Builder->SetInsertPoint (MergeBB);
    llvm::PHINode *PN =
    Builder->CreatePHI (llvm::Type::getDoubleTy (*TheContext), 2, "iftmp");

    PN->addIncoming (ThenV, ThenBB);
    PN->addIncoming (ElseV, ElseBB);
    return PN;
}