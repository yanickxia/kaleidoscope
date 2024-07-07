//
// Created by yanick on 2022/12/15.
//

#include "kaleidoscope.h"

#include "../extern/extern.h"

// This is an object that owns LLVM core data structures
std::unique_ptr<llvm::LLVMContext> TheContext;

// This is a helper object that makes easy to generate LLVM instructions
std::unique_ptr<llvm::IRBuilder<>> Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);

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

std::unique_ptr<llvm::DIBuilder> DBuilder;


void InitJIT() {
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
#ifndef EXPORT_TO_OBJECT
    TheJIT = ExitOnErr(llvm::orc::KaleidoscopeJIT::Create());
    auto& jd = TheJIT->getMainJITDylib();
    auto mangle = llvm::orc::MangleAndInterner(jd.getExecutionSession(), TheJIT->getDataLayout());

    auto s = [](llvm::orc::MangleAndInterner interner) {
        llvm::orc::SymbolMap symbolMap;
        symbolMap[interner("putchard")] = {
            llvm::pointerToJITTargetAddress(&putchard),
            llvm::JITSymbolFlags(),
        };
        symbolMap[interner("printd")] = {
            llvm::pointerToJITTargetAddress(&printd),
            llvm::JITSymbolFlags(),
        };
        return llvm::orc::absoluteSymbols(symbolMap);
    }(mangle);

    ExitOnErr(jd.define(s));
#endif
}

void InitializeModuleAndPassManager() {
    // Open a new context and module.
    TheContext = std::make_unique<llvm::LLVMContext>();
    TheModule = std::make_unique<llvm::Module>("My awesome JIT", *TheContext);
    // Create a new builder for the module.
    Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);

    DBuilder = std::make_unique<llvm::DIBuilder>(*TheModule);
    KSDbgInfo.TheCU = DBuilder->createCompileUnit(
        llvm::dwarf::DW_LANG_C,
        DBuilder->createFile("fib.ks", "."),
        "Kaleidoscope Compiler",
        0,
        "",
        0);

    // Add the current debug info version into the module.
    TheModule->addModuleFlag(
        llvm::Module::Warning,
        "Debug Info Version",
        llvm::DEBUG_METADATA_VERSION);

    // Darwin only supports dwarf2.
    if (llvm::Triple(llvm::sys::getProcessTriple()).isOSDarwin())
        TheModule->addModuleFlag(llvm::Module::Warning, "Dwarf Version", 2);

#ifndef EXPORT_TO_OBJECT
    TheModule->setDataLayout(TheJIT->getDataLayout());

    TheFPM = std::make_unique<llvm::legacy::FunctionPassManager>(TheModule.get());
    // Promote allocas to registers.
    TheFPM->add(llvm::createPromoteMemoryToRegisterPass());
    // Do simple "peephole" optimizations and bit-twiddling optzns.
    TheFPM->add(llvm::createInstructionCombiningPass());
    // Reassociate expressions.
    TheFPM->add(llvm::createReassociatePass());
    // Eliminate Common SubExpressions.
    TheFPM->add(llvm::createGVNPass());
    // Simplify the control flow graph (deleting unreachable blocks, etc).
    TheFPM->add(llvm::createCFGSimplificationPass());

    TheFPM->doInitialization();
#endif
}

llvm::DIType* DebugInfo::getDoubleTy() {
    if (DblTy)
        return DblTy;

    DblTy = DBuilder->createBasicType("double", 64, llvm::dwarf::DW_ATE_float);
    return DblTy;
}

llvm::DISubroutineType* CreateFunctionType(unsigned NumArgs, llvm::DIFile* Unit) {
    llvm::SmallVector<llvm::Metadata*, 8> EltTys;
    llvm::DIType* DblTy = KSDbgInfo.getDoubleTy();

    // Add the result type.
    EltTys.push_back(DblTy);

    for (unsigned i = 0, e = NumArgs; i != e; ++i)
        EltTys.push_back(DblTy);

    return DBuilder->createSubroutineType(DBuilder->getOrCreateTypeArray(EltTys));
}


void DebugInfo::emitLocation(ExprAST* AST) {
    if (!AST)
        return Builder->SetCurrentDebugLocation(llvm::DebugLoc());
    llvm::DIScope* Scope;
    if (LexicalBlocks.empty())
        Scope = TheCU;
    else
        Scope = LexicalBlocks.back();
    Builder->SetCurrentDebugLocation(
        llvm::DILocation::get(
            Scope->getContext(),
            AST->getLine(),
            AST->getCol(),
            Scope));
}