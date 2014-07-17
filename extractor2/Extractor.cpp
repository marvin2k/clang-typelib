//  vim: set foldmethod=marker:
//
// toy-program to test the RecursiveASTVisitor for speed-up compared to ast_matchers
//
// based on this tutorial, with added option-parsing:
//  http://clang.llvm.org/docs/RAVFrontendAction.html
//

// include {{{1
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "llvm/Support/Signals.h"
// }}}

// using namespace {{{1
using namespace clang;
using namespace clang::tooling;
using namespace llvm;
// }}}

class FindNamedClassVisitor : public RecursiveASTVisitor<FindNamedClassVisitor> {
    public:
        explicit FindNamedClassVisitor(ASTContext *Context)
            : Context(Context) {}

        bool VisitCXXRecordDecl(CXXRecordDecl *D) {
            const clang::SourceManager& sm(Context->getSourceManager());

            if (!sm.isInMainFile(D->getLocation())) {
                /* llvm::outs() << "skipping " */
                /*     << "'" << D->getQualifiedNameAsString() << "'" */
                /*     << "\n"; */
                return true;
            }

            llvm::outs() << "got "
                << "'" << clang::TypeWithKeyword::getTagTypeKindName(D->getTagKind()) << "'"
                << " named "
                << "'" << D->getQualifiedNameAsString() << "'"
                << "\n";

            return true;
        }

    private:
        ASTContext *Context;
};

class FindNamedClassConsumer : public clang::ASTConsumer {
    public:
        explicit FindNamedClassConsumer(ASTContext *Context)
            : Visitor(Context) {}

        virtual void HandleTranslationUnit(clang::ASTContext &Context) {
            //Visitor.TraverseDecl(Context.getTranslationUnitDecl());
        }
    private:
        FindNamedClassVisitor Visitor;
};

class FindNamedClassAction : public clang::ASTFrontendAction {
    public:
        virtual clang::ASTConsumer *CreateASTConsumer(
                clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
            return new FindNamedClassConsumer(&Compiler.getASTContext());
        }
};

int main(int argc, const char **argv) {

    // optparsing {{{1
    llvm::sys::PrintStackTraceOnErrorSignal();
    clang::tooling::CommonOptionsParser OptionsParser(argc, argv);
    clang::tooling::ClangTool Tool(OptionsParser.getCompilations(),
            OptionsParser.getSourcePathList());
    // }}}

    FrontendActionFactory *FrontendFactory = newFrontendActionFactory<FindNamedClassAction>();

    return Tool.run(FrontendFactory);
}
