//  vim: set foldmethod=marker:
//  Usage:
//  tool-template <cmake-output-dir> <file1> <file2> ...
//

// include {{{1
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecordLayout.h"
#include "clang/Lex/Lexer.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/ADT/OwningPtr.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/Signals.h"

#include <iostream>
// }}}

// using namespace {{{1
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;
// }}}

namespace {

class ToolTemplateCallback : public MatchFinder::MatchCallback {
 public:

  virtual void run(const MatchFinder::MatchResult &Result) {
//  TODO: This routine will get called for each thing that the matchers find.
//  At this point, you can examine the match, and do whatever you want,
//  including replacing the matched text with other text

    const CXXRecordDecl *D = Result.Nodes.getNodeAs<CXXRecordDecl>("match");
    if (D) {
        clang::SourceManager *sm(Result.SourceManager);

        // we just don't care about any file not given in the commandline
        if (!sm->isInMainFile(D->getLocation())) {
            return;
        }

        bool hasLayout = !D->isDependentType() && !D->isInvalidDecl();

//         std::cout << "Definition " << D->getDefinition() << std::endl;

        if (hasLayout && D->getDefinition())
        {
            std::cout << "got "
                << "'" << clang::TypeWithKeyword::getTagTypeKindName(D->getTagKind()) << "'"
                << " named "
                << "'" << D->getQualifiedNameAsString() << "'"
                << " in "
                << "'" << D->getLocation().printToString(*Result.SourceManager) << "'"
                << "\n";

//             std::cout << "Has ASTContext " << &(D->getASTContext()) << std::endl;
//             
            const clang::ASTRecordLayout *layout = 0;
            layout = &(D->getASTContext().getASTRecordLayout(D));
//             std::cout << "Has layout " << layout << std::endl;
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
        }
    
    }
  }

};
} // end anonymous namespace

int main(int argc, const char **argv) {

  // optparsing {{{1
  llvm::sys::PrintStackTraceOnErrorSignal();
  CommonOptionsParser OptionsParser(argc, argv);
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());
  // }}}

  ast_matchers::MatchFinder Finder;
  ToolTemplateCallback Callback;

  // AST matching ftw...
  //
  // the big table: http://clang.llvm.org/docs/LibASTMatchersReference.html

  // the "bind" will make the match referencable by the given string in the "run()" mathod of the
  // callback

  // the "isDefinition()" is needed to reject "Class Name Injection" and forward
  // declarations. see https://stackoverflow.com/questions/24761684 and
  // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/1994/N0444.pdf
  DeclarationMatcher matcher = recordDecl(isDefinition()).bind("match");

  Finder.addMatcher(matcher, &Callback);

  return Tool.run(newFrontendActionFactory(&Finder));
}
