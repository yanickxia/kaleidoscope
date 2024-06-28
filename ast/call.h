//
// Created by yanick on 22-12-14.
//

#ifndef KALEIDOSCOPE_CALL_H
#define KALEIDOSCOPE_CALL_H


#include <string>
#include <vector>
#include <memory>
#include "ast.h"
#include "../kaleidoscope/kaleidoscope.h"


/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;

public:
    CallExprAST(const std::string &Callee,
                std::vector<std::unique_ptr<ExprAST>> Args)
            : Callee(Callee), Args(std::move(Args)) {}
    llvm::Value *codegen() override;
};

#endif //KALEIDOSCOPE_CALL_H
