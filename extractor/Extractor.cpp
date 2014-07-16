//  vim: set foldmethod=marker:
//  Usage:
//  tool-template <cmake-output-dir> <file1> <file2> ...
//
//  Where <cmake-output-dir> is a CMake build directory in which a file named
//  compile_commands.json exists (enable -DCMAKE_EXPORT_COMPILE_COMMANDS in
//  CMake to get this output).
//
//  <file1> ... specify the paths of files in the CMake source tree. This path
//  is looked up in the compile command database. If the path of a file is
//  absolute, it needs to point into CMake's source tree. If the path is
//  relative, the current working directory needs to be in the CMake source
//  tree and the file must be in a subdirectory of the current working
//  directory. "./" prefixes in the relative files will be automatically
//  removed, but the rest of a relative path must be a suffix of a path in
//  the compile command line database.
//
//  For example, to use tool-template on all files in a subtree of the
//  source tree, use:
//
//    /path/in/subtree $ find . -name '*.cpp'|
//        xargs tool-template /path/to/build

// include {{{1
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
  ToolTemplateCallback(Replacements *Replace) : Replace(Replace) {}

  virtual void run(const MatchFinder::MatchResult &Result) {
//  TODO: This routine will get called for each thing that the matchers find.
//  At this point, you can examine the match, and do whatever you want,
//  including replacing the matched text with other text
    (void) Replace; // This to prevent an "unused member variable" warning;

    const CXXRecordDecl *D = Result.Nodes.getNodeAs<CXXRecordDecl>("match");
    if (D) {

        // do some initial filtering
        if (Result.SourceManager->isInSystemHeader(D->getLocation())) {
            /* std::cout << "skipping system header " */
            /*     << "'" << D->getLocation().printToString(*Result.SourceManager) << "'" */
            /*     << "\n"; */
            return;
        }

        bool hasLayout = !D->isDependentType() && !D->isInvalidDecl();

//         std::cout << "Definition " << D->getDefinition() << std::endl;

        if (hasLayout && D->getDefinition())
        {
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
    
      std::cout << "got "
          << D->getTagKind() << " named "
          << D->getQualifiedNameAsString() << " in "
          << D->getLocation().printToString(*Result.SourceManager) << "\n";
        
      std::cout << "got "
          << "'" << clang::TypeWithKeyword::getTagTypeKindName(D->getTagKind()) << "'"
          << " named "
          << "'" << D->getQualifiedNameAsString() << "'"
          << " in "
          << "'" << D->getLocation().printToString(*Result.SourceManager) << "'"
          << "\n";

      // prints alotta stuff...
      //D->dump();
    }
  }

 private:
  Replacements *Replace;
};
} // end anonymous namespace

// options {{{
// Set up the command line options
cl::opt<std::string> BuildPath(
  cl::Positional,
  cl::desc("<build-path>"));

cl::list<std::string> SourcePaths(
  cl::Positional,
  cl::desc("<source0> [... <sourceN>]"),
  cl::OneOrMore);
// }}}

int main(int argc, const char **argv) {

  // optparsing {{{1
  llvm::sys::PrintStackTraceOnErrorSignal();
  llvm::OwningPtr<CompilationDatabase> Compilations(
        FixedCompilationDatabase::loadFromCommandLine(argc, argv));
  cl::ParseCommandLineOptions(argc, argv);
  if (!Compilations) {  // Couldn't find a compilation DB from the command line
    std::string ErrorMessage;
    Compilations.reset(
      !BuildPath.empty() ?
        CompilationDatabase::autoDetectFromDirectory(BuildPath, ErrorMessage) :
        CompilationDatabase::autoDetectFromSource(SourcePaths[0], ErrorMessage)
      );

//  Still no compilation DB? - bail.
    if (!Compilations)
      llvm::report_fatal_error(ErrorMessage);
    }
  // }}}

  RefactoringTool Tool(*Compilations, SourcePaths);
  ast_matchers::MatchFinder Finder;
  ToolTemplateCallback Callback(&Tool.getReplacements());

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
