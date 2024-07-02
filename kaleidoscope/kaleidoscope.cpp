//
// Created by yanick on 2022/12/15.
//

#include "kaleidoscope.h"

#include "../extern/extern.h"

// This is an object that owns LLVM core data structures
std::unique_ptr<llvm::LLVMContext> TheContext;

// This is a helper object that makes easy to generate LLVM instructions
std::unique_ptr<llvm::IRBuilder<>> Builder =
std::make_unique<llvm::IRBuilder<>> (*TheContext);

// This is an LLVM construct that contains functions and global variables
std::unique_ptr<llvm::Module> TheModule;

// This map keeps track of which values are defined in the current scope
std::map<std::string, llvm::AllocaInst*> NamedValues;

std::unique_ptr<llvm::orc::KaleidoscopeJIT> TheJIT;

llvm::ExitOnError ExitOnErr;

// static std::unique_ptr<KaleidoscopeJIT> TheJIT;
std::unique_ptr<llvm::legacy::FunctionPassManager> TheFPM;
// std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;

std::map<char, int> BinopPrecedence;

void InitJIT () {
    llvm::InitializeNativeTarget ();
    llvm::InitializeNativeTargetAsmPrinter ();
    llvm::InitializeNativeTargetAsmParser ();

    TheJIT      = ExitOnErr (llvm::orc::KaleidoscopeJIT::Create ());
    auto& jd    = TheJIT->getMainJITDylib ();
    auto mangle = llvm::orc::MangleAndInterner (
    jd.getExecutionSession (), TheJIT->getDataLayout ());

    auto s = [] (llvm::orc::MangleAndInterner interner) {
        llvm::orc::SymbolMap symbolMap;
        symbolMap[interner ("putchard")] = {
            llvm::pointerToJITTargetAddress (&putchard),
            llvm::JITSymbolFlags (),
        };
        symbolMap[interner ("printd")] = {
            llvm::pointerToJITTargetAddress (&printd),
            llvm::JITSymbolFlags (),
        };
        return llvm::orc::absoluteSymbols (symbolMap);
    }(mangle);

    ExitOnErr (jd.define (s));
}


void InitializeModuleAndPassManager () {
    // Open a new context and module.
    TheContext = std::make_unique<llvm::LLVMContext> ();
    TheModule  = std::make_unique<llvm::Module> ("My awesome JIT", *TheContext);
    TheModule->setDataLayout (TheJIT->getDataLayout ());
    // Create a new builder for the module.
    Builder = std::make_unique<llvm::IRBuilder<>> (*TheContext);

    TheFPM = std::make_unique<llvm::legacy::FunctionPassManager> (TheModule.get ());
    // Promote allocas to registers.
    TheFPM->add(llvm::createPromoteMemoryToRegisterPass());
    // Do simple "peephole" optimizations and bit-twiddling optzns.
    TheFPM->add (llvm::createInstructionCombiningPass ());
    // Reassociate expressions.
    TheFPM->add (llvm::createReassociatePass ());
    // Eliminate Common SubExpressions.
    TheFPM->add (llvm::createGVNPass ());
    // Simplify the control flow graph (deleting unreachable blocks, etc).
    TheFPM->add (llvm::createCFGSimplificationPass ());

    TheFPM->doInitialization ();
}