//
// Created by yanick on 22-12-14.
//

#include "function.h"

#include "../log/log.h"
#include "variable.h"
#include "../kaleidoscope/kaleidoscope.h"

std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;

llvm::Function* getFunction(std::string Name) {
    // First, see if the function has already been added to the current module.
    if (auto* F = TheModule->getFunction(Name))
        return F;

    // If not, check whether we can codegen the declaration from some existing
    // prototype.
    auto FI = FunctionProtos.find(Name);
    if (FI != FunctionProtos.end())
        return FI->second->codegen();

    // If no existing prototype exists, return null.
    return nullptr;
}

llvm::Function* FunctionAST::codegen() {
    // Transfer ownership of the prototype to the FunctionProtos map, but keep a
    // reference to it for use below.
    auto& P = *Proto;
    FunctionProtos[Proto->getName()] = std::move(Proto);
    llvm::Function* TheFunction = getFunction(P.getName());
    if (!TheFunction)
        return nullptr;

    // If this is an operator, install it.
    if (P.isBinaryOp())
        BinopPrecedence[P.getOperatorName()] = P.getBinaryPrecedence();

    // Create a new basic block to start insertion into.
    llvm::BasicBlock* BB = llvm::BasicBlock::Create(*TheContext, "entry", TheFunction);
    Builder->SetInsertPoint(BB);

    // Create a subprogram DIE for this function.
    llvm::DIFile* Unit = DBuilder->createFile(
        KSDbgInfo.TheCU->getFilename(),
        KSDbgInfo.TheCU->getDirectory());
    llvm::DIScope* FContext = Unit;
    unsigned LineNo = P.getLine();
    unsigned ScopeLine = LineNo;
    llvm::DISubprogram* SP = DBuilder->createFunction(
        FContext,
        P.getName(),
        llvm::StringRef(),
        Unit,
        LineNo,
        CreateFunctionType(TheFunction->arg_size(), Unit),
        ScopeLine,
        llvm::DINode::FlagPrototyped,
        llvm::DISubprogram::SPFlagDefinition);
    TheFunction->setSubprogram(SP);

    // Push the current scope.
    KSDbgInfo.LexicalBlocks.push_back(SP);

    // Unset the location for the prologue emission (leading instructions with no
    // location in a function are considered part of the prologue and the debugger
    // will run past them when breaking on a function)
    KSDbgInfo.emitLocation(nullptr);

    // Record the function arguments in the NamedValues map.
    NamedValues.clear();
    unsigned ArgIdx = 0;
    for (auto& Arg : TheFunction->args()) {
        // Create an alloca for this variable.
        llvm::AllocaInst* Alloca = CreateEntryBlockAlloca(TheFunction, Arg.getName());
        // Create a debug descriptor for the variable.
        llvm::DILocalVariable* D = DBuilder->createParameterVariable(
            SP,
            Arg.getName(),
            ++ArgIdx,
            Unit,
            LineNo,
            KSDbgInfo.getDoubleTy(),
            true);

        DBuilder->insertDeclare(
            Alloca,
            D,
            DBuilder->createExpression(),
            llvm::DILocation::get(SP->getContext(), LineNo, 0, SP),
            Builder->GetInsertBlock());
        // Store the initial value into the alloca.
        Builder->CreateStore(&Arg, Alloca);

        // Add arguments to variable symbol table.
        NamedValues[std::string(Arg.getName())] = Alloca;
    }
    KSDbgInfo.emitLocation(Body.get());

    if (llvm::Value* RetVal = Body->codegen()) {
        // Finish off the function.
        Builder->CreateRet(RetVal);
        // Pop off the lexical block for the function.
        KSDbgInfo.LexicalBlocks.pop_back();
        // Validate the generated code, checking for consistency.
        verifyFunction(*TheFunction);

        // Run the optimizer on the function.
        //        TheFPM->run(*TheFunction);

        return TheFunction;
    }
    // Error reading body, remove function.
    TheFunction->eraseFromParent();

    if (P.isBinaryOp())
        BinopPrecedence.erase(P.getOperatorName());
    // Pop off the lexical block for the function since we added it
    // unconditionally.
    KSDbgInfo.LexicalBlocks.pop_back();

    return nullptr;
}