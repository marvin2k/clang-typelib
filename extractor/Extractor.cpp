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
#include "clang/AST/RecordLayout.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/Signals.h"

#include <iostream>
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

            std::cout << "got "
                << "'" << clang::TypeWithKeyword::getTagTypeKindName(D->getTagKind()) << "'"
                << " named "
                << "'" << D->getQualifiedNameAsString() << "'"
                << " in "
                << "'" << D->getLocation().printToString(sm) << "'"
                << "\n";

            const clang::ASTRecordLayout *layout = 0;
            layout = &(D->getASTContext().getASTRecordLayout(D));
//             std::cout << "Alignment " << layout-> getAlignment().getQuantity() << std::endl;
            std::cout << "Type " << D->getQualifiedNameAsString() <<std::endl;
            std::cout << "  Size " << layout->getSize().getQuantity() << std::endl;

            if(D->field_begin() != D->field_end())
            {
                std::cout << "  Members :" << std::endl;
            }
            
            for(RecordDecl::field_iterator fit = D->field_begin(); fit != D->field_end(); fit++)
            {
                std::cout << "    Field " << fit->getQualifiedNameAsString() << std::endl;
                std::cout << "    Offset " << layout->getFieldOffset(fit->getFieldIndex()) << std::endl;
                std::cout << "    Decl Name " << fit->getNameAsString() << std::endl;
                
                SplitQualType T_split = fit->getType().split();
                std::cout << "    Qual Type " << QualType::getAsString(T_split) << std::endl;
                std::cout << std::endl;
            }
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
