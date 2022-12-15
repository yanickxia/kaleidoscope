//
// Created by yanick on 22-12-14.
//

#ifndef KALEIDOSCOPE_FUNCTION_H
#define KALEIDOSCOPE_FUNCTION_H

#include <memory>
#include "prototype.h"
#include "ast.h"
#include "../kaleidoscope/kaleidoscope.h"
#include "../log/log.h"

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                std::unique_ptr<ExprAST> Body)
            : Proto(std::move(Proto)), Body(std::move(Body)) {}
    llvm::Function *codegen();
};

#endif //KALEIDOSCOPE_FUNCTION_H
