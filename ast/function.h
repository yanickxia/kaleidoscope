//
// Created by yanick on 22-12-14.
//

#ifndef KALEIDOSCOPE_FUNCTION_H
#define KALEIDOSCOPE_FUNCTION_H

#include <memory>

#include "ast.h"
#include "prototype.h"

extern std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST> Body)
        : Proto(std::move(Proto)), Body(std::move(Body)) {}

    llvm::Function* codegen();

    llvm::raw_ostream& dump(llvm::raw_ostream& out, int ind) {
        indent(out, ind) << "FunctionAST\n";
        ++ind;
        indent(out, ind) << "Body:";
        return Body ? Body->dump(out, ind) : out << "null\n";
    }
};

llvm::Function* getFunction(std::string Name);

#endif // KALEIDOSCOPE_FUNCTION_H