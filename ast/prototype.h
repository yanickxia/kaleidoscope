//
// Created by yanick on 22-12-14.
//

#ifndef KALEIDOSCOPE_PROTOTYPE_H
#define KALEIDOSCOPE_PROTOTYPE_H

#include "../kaleidoscope/kaleidoscope.h"
#include <string>
#include <vector>

/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;

    public:
    PrototypeAST (const std::string& name, std::vector<std::string> Args)
    : Name (name), Args (std::move (Args)) {
    }

    const std::string& getName () const {
        return Name;
    }
    llvm::Function* codegen ();
};

#endif // KALEIDOSCOPE_PROTOTYPE_H