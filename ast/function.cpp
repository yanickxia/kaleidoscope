//
// Created by yanick on 22-12-14.
//

#include "function.h"

llvm::Function* FunctionAST::codegen()
{
  // First, check for an existing function from a previous 'extern' declaration.

  llvm::Function* TheFunction = nullptr;

  if (!Proto->getName().empty())
  {
    TheFunction = TheModule->getFunction(Proto->getName());
  }

  if (!TheFunction)
  {
    TheFunction = Proto->codegen();
  }

  if (!TheFunction)
    return nullptr;

  if (!TheFunction->empty())
  {
    return (llvm::Function*)LogErrorV("Function cannot be redefined.");
  }
  // Create a new basic block to start insertion into.
  llvm::BasicBlock* BB =
    llvm::BasicBlock::Create(*TheContext, "entry", TheFunction);
  Builder->SetInsertPoint(BB);

  // Record the function arguments in the NamedValues map.
  NamedValues.clear();
  for (auto& Arg : TheFunction->args())
  {
    NamedValues[Arg.getName().str()] = &Arg;
  }
  if (llvm::Value* RetVal = Body->codegen())
  {
    // Finish off the function.
    Builder->CreateRet(RetVal);

    // Validate the generated code, checking for consistency.
    llvm::verifyFunction(*TheFunction);

    return TheFunction;
  }
  // Error reading body, remove function.
  TheFunction->eraseFromParent();
  return nullptr;
}
