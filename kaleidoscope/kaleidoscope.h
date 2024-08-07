//
// Created by yanick on 2022/12/15.
//

#ifndef KALEIDOSCOPE_KALEIDOSCOPE_H
#define KALEIDOSCOPE_KALEIDOSCOPE_H

#include <map>

#include "../jit/jit.h"
#include "../llvm/llvm.h"
#include "../ast/ast.h"

// This is an object that owns LLVM core data structures
extern std::unique_ptr<llvm::LLVMContext> TheContext;

// This is a helper object that makes easy to generate LLVM instructions
extern std::unique_ptr<llvm::IRBuilder<>> Builder;

// This is an LLVM construct that contains functions and global variables
extern std::unique_ptr<llvm::Module> TheModule;

// This map keeps track of which values are defined in the current scope
extern std::map<std::string, llvm::AllocaInst*> NamedValues;

extern std::unique_ptr<llvm::orc::KaleidoscopeJIT> TheJIT;

extern llvm::ExitOnError ExitOnErr;

extern std::unique_ptr<llvm::legacy::FunctionPassManager> TheFPM;

extern std::map<char, int> BinopPrecedence;

extern std::unique_ptr<llvm::DIBuilder> DBuilder;


void InitializeModuleAndPassManager();
void InitJIT();
llvm::DISubroutineType* CreateFunctionType(unsigned NumArgs, llvm::DIFile* Unit);


inline struct DebugInfo {
    llvm::DICompileUnit* TheCU;
    llvm::DIType* DblTy;
    std::vector<llvm::DIScope*> LexicalBlocks;
    void emitLocation(ExprAST* AST);
    llvm::DIType* getDoubleTy();
} KSDbgInfo;

#endif // KALEIDOSCOPE_KALEIDOSCOPE_H