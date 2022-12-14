//
// Created by yanick on 22-12-14.
//

#ifndef KALEIDOSCOPE_NUM_H
#define KALEIDOSCOPE_NUM_H

#include "ast.h"

/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
    double Val;

public:
    NumberExprAST(double Val) : Val(Val) {}
};

#endif //KALEIDOSCOPE_NUM_H
