//
// Created by yanick on 22-12-14.
//

#ifndef KALEIDOSCOPE_PROTOTYPE_H
#define KALEIDOSCOPE_PROTOTYPE_H

#include <string>
#include <vector>

#include "../kaleidoscope/kaleidoscope.h"

/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;
    bool IsOperator;
    unsigned Precedence; // Precedence if a binary op.

public:
    PrototypeAST(
        const std::string& name,
        std::vector<std::string> Args,
        bool IsOperator = false,
        unsigned Prec = 0)
        : Name(name), Args(std::move(Args)), IsOperator(IsOperator),
          Precedence(Prec) {}

    llvm::Function* codegen();

    const std::string& getName() const {
        return Name;
    }

    bool isUnaryOp() const {
        return IsOperator && Args.size() == 1;
    }

    bool isBinaryOp() const {
        return IsOperator && Args.size() == 2;
    }

    char getOperatorName() const {
        assert(isUnaryOp() || isBinaryOp());
        return Name[Name.size() - 1];
    }

    unsigned getBinaryPrecedence() const {
        return Precedence;
    }
};
#endif // KALEIDOSCOPE_PROTOTYPE_H