//
// Created by yanick on 22-12-14.
//

#ifndef KALEIDOSCOPE_AST_H
#define KALEIDOSCOPE_AST_H

#include "../llvm/llvm.h"
#include "../lexer/lexer.h"

/// ExprAST - Base class for all expression nodes.
class ExprAST {
    SourceLocation Loc;

public:
    ExprAST(SourceLocation Loc = CurLoc) : Loc(Loc) {}
    virtual ~ExprAST() = default;
    virtual llvm::Value* codegen() = 0;

    int getLine() const {
        return Loc.Line;
    }

    int getCol() const {
        return Loc.Col;
    }

    virtual llvm::raw_ostream& dump(llvm::raw_ostream& out, int ind) {
        return out << ':' << getLine() << ':' << getCol() << '\n';
    }
};

inline llvm::raw_ostream& indent(llvm::raw_ostream& O, int size) {
    return O << std::string(size, ' ');
}

#endif // KALEIDOSCOPE_AST_H