//
// Created by yanick on 24-7-2.
//

#ifndef KALEIDOSCOPE_VAR_H
#define KALEIDOSCOPE_VAR_H

#include "ast.h"
/// VarExprAST - Expression class for var/in
class VarExprAST : public ExprAST {
    std::vector<std::pair<std::string, std::unique_ptr<ExprAST>>> VarNames;
    std::unique_ptr<ExprAST> Body;

public:
    VarExprAST(
        std::vector<std::pair<std::string, std::unique_ptr<ExprAST>>> VarNames,
        std::unique_ptr<ExprAST> Body)
    : VarNames(std::move(VarNames)), Body(std::move(Body)) {}

    llvm::Value* codegen() override;
};

#endif //KALEIDOSCOPE_VAR_H
