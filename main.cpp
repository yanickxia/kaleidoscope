#include <iostream>

#include "kaleidoscope/kaleidoscope.h"
#include "parser/parser.h"

static void HandleDefinition() {
    if (auto FnAST = ParseDefinition()) {
#ifndef EXPORT_TO_OBJECT
        if (auto* FnIR = FnAST->codegen()) {
            ExitOnErr(TheJIT->addModule(
                llvm::orc::ThreadSafeModule(std::move(TheModule), std::move(TheContext))));
            InitializeModuleAndPassManager();
        }
#else
        if (!FnAST->codegen())
            fprintf(stderr, "Error reading function definition:");
#endif
    } else {
        // Skip token for error recovery.
        getNextToken();
    }
}

static void HandleExtern() {
    if (auto ProtoAST = ParseExtern()) {
        if (auto* FnIR = ProtoAST->codegen()) {
            fprintf(stderr, "Read extern: ");
            FnIR->print(llvm::errs());
            fprintf(stderr, "\n");
            FunctionProtos[ProtoAST->getName()] = std::move(ProtoAST);
        }
    } else {
        // Skip token for error recovery.
        getNextToken();
    }
}

static void HandleTopLevelExpression() {
    // Evaluate a top-level expression into an anonymous function.
    if (auto FnAST = ParseTopLevelExpr()) {
        if (auto* FnIR = FnAST->codegen()) {
#ifndef EXPORT_TO_OBJECT
            // Create a ResourceTracker to track JIT'd memory allocated to our
            // anonymous expression -- that way we can free it after executing.
            auto RT = TheJIT->getMainJITDylib().createResourceTracker();

            auto TSM = llvm::orc::ThreadSafeModule(std::move(TheModule), std::move(TheContext));
            ExitOnErr(TheJIT->addModule(std::move(TSM), RT));
            InitializeModuleAndPassManager();

            // Search the JIT for the __anon_expr symbol.
            // auto ExprSymbol = TheJIT->lookup ("__anon_expr");
            // 找到 入口函数，这里和  ParseTopLevelExpr 中保持一致
            auto ExprSymbol = TheJIT->lookup("__anon_expr");
            assert(ExprSymbol && "Function not found");

            // Get the symbol's address and cast it to the right type (takes no
            // arguments, returns a double) so we can call it as a native function.
            double (*FP)() = (double (*)())(intptr_t)ExprSymbol->getAddress();
            fprintf(stderr, "Evaluated to %f\n", FP());
            ExitOnErr(RT->remove());
#endif
            if (!FnIR) {
                fprintf(stderr, "Error generating code for top level expr");
            }
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
        getNextToken();
    }
}

/// top ::= definition | external | expression | ';'
static void MainLoop() {
    while (true) {
#ifndef DISABLE_INTERACTIVE
        fprintf(stderr, "ready> ");
#endif
        switch (CurTok) {
            case tok_eof:
                return;
            case ';': // ignore top-level semicolons.
                getNextToken();
                break;
            case tok_def:
                HandleDefinition();
                break;
            case tok_extern:
                HandleExtern();
                break;
            default:
                HandleTopLevelExpression();
                break;
        }
    }
}

int main() {
    try {
        // Install standard binary operators.
        // 1 is lowest precedence.
        BinopPrecedence['='] = 2;
        BinopPrecedence['<'] = 10;
        BinopPrecedence['+'] = 20;
        BinopPrecedence['-'] = 20;
        BinopPrecedence['*'] = 40; // highest.

#ifndef DISABLE_INTERACTIVE
        // Prime the first token.
        fprintf(stderr, "ready> ");
#endif
        getNextToken();

        InitJIT();
        InitializeModuleAndPassManager();

        // Create the compile unit for the module.
        // Currently down as "fib.ks" as a filename since we're redirecting stdin
        // but we'd like actual source locations.
        KSDbgInfo.TheCU = DBuilder->createCompileUnit(
            llvm::dwarf::DW_LANG_C,
            DBuilder->createFile("fib.ks", "."),
            "Kaleidoscope Compiler",
            false,
            "",
            0);

        // Run the main "interpreter loop" now.
        MainLoop();

        // Print out all of the generated code.
        TheModule->print(llvm::errs(), nullptr);

        DBuilder->finalize();

#ifdef EXPORT_TO_OBJECT
        auto TargetTriple = llvm::sys::getDefaultTargetTriple();
        TheModule->setTargetTriple(TargetTriple);

        std::string Error;
        auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

        // Print an error and exit if we couldn't find the requested target.
        // This generally occurs if we've forgotten to initialise the
        // TargetRegistry or we have a bogus target triple.
        if (!Target) {
            llvm::errs() << Error;
            return 1;
        }

        auto CPU = "generic";
        auto Features = "";

        llvm::TargetOptions opt;
        auto RM = llvm::Optional<llvm::Reloc::Model>();
        auto TheTargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

        TheModule->setDataLayout(TheTargetMachine->createDataLayout());

        auto Filename = "output.o";
        std::error_code EC;
        llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::OF_None);

        if (EC) {
            llvm::errs() << "Could not open file: " << EC.message();
            return 1;
        }

        llvm::legacy::PassManager pass;
        auto FileType = llvm::CGFT_ObjectFile;

        if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
            llvm::errs() << "TheTargetMachine can't emit a file of this type";
            return 1;
        }

        pass.run(*TheModule);
        dest.flush();

        llvm::outs() << "Wrote " << Filename << "\n";
#endif
    } catch (const std::exception& e) {
        fprintf(stderr, "%s", e.what());
    }

    return 0;
}