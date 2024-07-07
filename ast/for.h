//
// Created by yanick on 24-6-30.
//

#ifndef KALEIDOSCOPE_FOR_H
#define KALEIDOSCOPE_FOR_H

#include "ast.h"

/// ForExprAST - Expression class for for/in.
class ForExprAST : public ExprAST {
    std::string VarName;
    std::unique_ptr<ExprAST> Start, End, Step, Body;

    public:
    ForExprAST (const std::string &VarName,
    std::unique_ptr<ExprAST> Start,
    std::unique_ptr<ExprAST> End,
    std::unique_ptr<ExprAST> Step,
    std::unique_ptr<ExprAST> Body)
    : VarName (VarName), Start (std::move (Start)), End (std::move (End)),
      Step (std::move (Step)), Body (std::move (Body)) {
    }

    llvm::Value *codegen () override;
    llvm::raw_ostream &dump(llvm::raw_ostream &out, int ind) override {
        ExprAST::dump(out << "for", ind);
        Start->dump(indent(out, ind) << "Cond:", ind + 1);
        End->dump(indent(out, ind) << "End:", ind + 1);
        Step->dump(indent(out, ind) << "Step:", ind + 1);
        Body->dump(indent(out, ind) << "Body:", ind + 1);
        return out;
    }
};

#endif // KALEIDOSCOPE_FOR_H
