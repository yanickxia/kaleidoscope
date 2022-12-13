//
// Created by yanick on 2022/12/13.
//

#ifndef KALEIDOSCOPE_LEXER_H
#define KALEIDOSCOPE_LEXER_H

#include <string>
extern int CurTok;
int gettok();
int getNextToken();

// If the current token is an identifier
// IdentifierStr will hold the name of the identifier
extern std::string IdentifierStr;

// If the current token is a numeric literal
// NumVal holds its value
extern double NumVal;
#endif //KALEIDOSCOPE_LEXER_H
