#include <iostream>

#include "kaleidoscope/kaleidoscope.h"
#include "parser/parser.h"

static void HandleDefinition () {
    if (auto FnAST = ParseDefinition ()) {
        if (auto* FnIR = FnAST->codegen ()) {
            fprintf (stderr, "Read function definition:");
            FnIR->print (llvm::errs ());
            fprintf (stderr, "\n");
            ExitOnErr (TheJIT->addModule (llvm::orc::ThreadSafeModule (
            std::move (TheModule), std::move (TheContext))));
            InitializeModuleAndPassManager ();
        }
    } else {
        // Skip token for error recovery.
        getNextToken ();
    }
}

static void HandleExtern () {
    if (auto ProtoAST = ParseExtern ()) {
        if (auto* FnIR = ProtoAST->codegen ()) {
            fprintf (stderr, "Read extern: ");
            FnIR->print (llvm::errs ());
            fprintf (stderr, "\n");
            FunctionProtos[ProtoAST->getName ()] = std::move (ProtoAST);
        }
    } else {
        // Skip token for error recovery.
        getNextToken ();
    }
}

static void HandleTopLevelExpression () {
    // Evaluate a top-level expression into an anonymous function.
    if (auto FnAST = ParseTopLevelExpr ()) {
        if (auto* FnIR = FnAST->codegen ()) {
            // Create a ResourceTracker to track JIT'd memory allocated to our
            // anonymous expression -- that way we can free it after executing.
            auto RT = TheJIT->getMainJITDylib ().createResourceTracker ();

            auto TSM = llvm::orc::ThreadSafeModule (
            std::move (TheModule), std::move (TheContext));
            ExitOnErr (TheJIT->addModule (std::move (TSM), RT));
            InitializeModuleAndPassManager ();


            // Search the JIT for the __anon_expr symbol.
            // auto ExprSymbol = TheJIT->lookup ("__anon_expr");
            auto ExprSymbol = TheJIT->lookup ("__anon_expr");
            assert (ExprSymbol && "Function not found");

            // Get the symbol's address and cast it to the right type (takes no
            // arguments, returns a double) so we can call it as a native function.
            double (*FP) () = (double (*) ()) (intptr_t)ExprSymbol->getAddress ();
            fprintf (stderr, "Evaluated to %f\n", FP ());
            ExitOnErr (RT->remove ());

            // ------------- Non JIT --------------------//
            // fprintf(stderr, "Read top-level expression:");
            // FnIR->print(llvm::errs());
            // fprintf(stderr, "\n");
            //
            // // Remove the anonymous expression.
            // FnIR->eraseFromParent();
        }
    } else {
        // Skip token for error recovery.
        getNextToken ();
    }
}

/// top ::= definition | external | expression | ';'
static void MainLoop () {
    while (true) {
        fprintf (stderr, "ready> ");
        switch (CurTok) {
        case tok_eof: return;
        case ';': // ignore top-level semicolons.
            getNextToken ();
            break;
        case tok_def: HandleDefinition (); break;
        case tok_extern: HandleExtern (); break;
        default: HandleTopLevelExpression (); break;
        }
    }
}

int main () {
    try {
        // Install standard binary operators.
        // 1 is lowest precedence.
        BinopPrecedence['<'] = 10;
        BinopPrecedence['+'] = 20;
        BinopPrecedence['-'] = 20;
        BinopPrecedence['*'] = 40; // highest.

        // Prime the first token.
        fprintf (stderr, "ready> ");
        getNextToken ();

        InitJIT ();
        InitializeModuleAndPassManager ();

        // Run the main "interpreter loop" now.
        MainLoop ();

        // Print out all of the generated code.
        TheModule->print (llvm::errs (), nullptr);
    } catch (const std::exception& e) {
        fprintf (stderr, "%s", e.what ());
    }

    return 0;
}