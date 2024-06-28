//
// Created by yanick on 2022/12/15.
//

#include "kaleidoscope.h"

// This is an object that owns LLVM core data structures
std::unique_ptr<llvm::LLVMContext> TheContext;

// This is a helper object that makes easy to generate LLVM instructions
std::unique_ptr<llvm::IRBuilder<>> Builder;

// This is an LLVM construct that contains functions and global variables
std::unique_ptr<llvm::Module> TheModule;

// This map keeps track of which values are defined in the current scope
std::map<std::string, llvm::Value *> NamedValues;

// static std::unique_ptr<KaleidoscopeJIT> TheJIT;
std::unique_ptr<llvm::FunctionPassManager> TheFPM;
std::unique_ptr<llvm::LoopAnalysisManager> TheLAM;
std::unique_ptr<llvm::FunctionAnalysisManager> TheFAM;
std::unique_ptr<llvm::CGSCCAnalysisManager> TheCGAM;
std::unique_ptr<llvm::ModuleAnalysisManager> TheMAM;
std::unique_ptr<llvm::PassInstrumentationCallbacks> ThePIC;
std::unique_ptr<llvm::StandardInstrumentations> TheSI;
// std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;

void InitializeModuleAndManagers() {
  TheContext = std::make_unique<llvm::LLVMContext>();
  Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
  TheModule = std::make_unique<llvm::Module>("My awesome JIT", *TheContext);

  TheFPM = std::make_unique<llvm::FunctionPassManager>();
  TheLAM = std::make_unique<llvm::LoopAnalysisManager>();
  TheFAM = std::make_unique<llvm::FunctionAnalysisManager>();
  TheCGAM = std::make_unique<llvm::CGSCCAnalysisManager>();
  TheMAM = std::make_unique<llvm::ModuleAnalysisManager>();
  ThePIC = std::make_unique<llvm::PassInstrumentationCallbacks>();
  TheSI = std::make_unique<llvm::StandardInstrumentations>(
      /*DebugLogging*/ true);
}
