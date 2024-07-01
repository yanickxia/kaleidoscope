//
// Created by yanick on 2022/12/13.
//

#ifndef KALEIDOSCOPE_PARSER_H
#define KALEIDOSCOPE_PARSER_H

#include <memory>
#include <map>
#include "../ast/ast.h"
#include "../ast/num.h"
#include "../ast/variable.h"
#include "../lexer/lexer.h"
#include "../log/log.h"
#include "../lexer/token.h"
#include "../ast/call.h"
#include "../ast/binary.h"
#include "../ast/function.h"
#include "../ast/if.h"
#include "../ast/for.h"
#include "../ast/unary.h"
#include "../kaleidoscope/kaleidoscope.h"

std::unique_ptr<ExprAST> ParseNumberExpr();

std::unique_ptr<ExprAST> ParseParenExpr();

std::unique_ptr<ExprAST> ParseIdentifierExpr();

std::unique_ptr<ExprAST> ParsePrimary();

std::unique_ptr<ExprAST> ParseExpression();

std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS);

std::unique_ptr<PrototypeAST> ParsePrototype();

std::unique_ptr<FunctionAST> ParseDefinition();

std::unique_ptr<PrototypeAST> ParseExtern();

std::unique_ptr<FunctionAST> ParseTopLevelExpr();

std::unique_ptr<ExprAST> ParseUnary();

#endif //KALEIDOSCOPE_PARSER_H
