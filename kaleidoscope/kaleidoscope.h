//
// Created by yanick on 2022/12/15.
//

#ifndef KALEIDOSCOPE_KALEIDOSCOPE_H
#define KALEIDOSCOPE_KALEIDOSCOPE_H

#include <map>

// #include "../ast/prototype.h"
#include "../llvm/llvm.h"
#include <map>

// This is an object that owns LLVM core data structures
extern std::unique_ptr<llvm::LLVMContext> TheContext;

// This is a helper object that makes easy to generate LLVM instructions
extern std::unique_ptr<llvm::IRBuilder<>> Builder;

// This is an LLVM construct that contains functions and global variables
extern std::unique_ptr<llvm::Module> TheModule;

// This map keeps track of which values are defined in the current scope
extern std::map<std::string, llvm::Value *> NamedValues;

void InitializeModuleAndManagers();

// pass and analysis managers.
extern std::unique_ptr<llvm::FunctionPassManager> TheFPM;
extern std::unique_ptr<llvm::LoopAnalysisManager> TheLAM;
extern std::unique_ptr<llvm::FunctionAnalysisManager> TheFAM;
extern std::unique_ptr<llvm::CGSCCAnalysisManager> TheCGAM;
extern std::unique_ptr<llvm::ModuleAnalysisManager> TheMAM;
extern std::unique_ptr<llvm::PassInstrumentationCallbacks> ThePIC;
extern std::unique_ptr<llvm::StandardInstrumentations> TheSI;
// extern std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;

#endif // KALEIDOSCOPE_KALEIDOSCOPE_H
