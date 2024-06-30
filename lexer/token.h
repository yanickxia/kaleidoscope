//
// Created by yanick on 2022/12/13.
//

#ifndef KALEIDOSCOPE_TOKEN_H
#define KALEIDOSCOPE_TOKEN_H
// The lexer returns tokens [0-255] if it's an unknown character
// otherwise it returns one of these for known things
enum Token {
    // End Of File
    tok_eof = -1,

    // Commands
    tok_def    = -2,
    tok_extern = -3,

    // Primary
    tok_identifier = -4,
    tok_number     = -5,

    // control flow
    tok_if   = -6,
    tok_then = -7,
    tok_else = -8,
    tok_for  = -9,
    tok_in   = -10
};
#endif // KALEIDOSCOPE_TOKEN_H
