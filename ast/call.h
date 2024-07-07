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
    CallExprAST(
        SourceLocation loc,
        const std::string& Callee,
        std::vector<std::unique_ptr<ExprAST>> Args)
        : ExprAST(loc), Callee(Callee), Args(std::move(Args)) {}

    llvm::Value* codegen() override;
    llvm::raw_ostream &dump(llvm::raw_ostream &out, int ind) override {
        ExprAST::dump(out << "call " << Callee, ind);
        for (const auto &Arg : Args)
            Arg->dump(indent(out, ind + 1), ind + 1);
        return out;
    }
};

#endif //KALEIDOSCOPE_CALL_H