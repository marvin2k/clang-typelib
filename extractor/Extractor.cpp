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
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/FrontendActions.h"
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

//const DirectoryEntry* theDir;

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

        /* FileID id = sm->getFileID(D->getLocation()); */
        /* const FileEntry* entry = sm->getFileEntryForID(id); */
        /* if (entry) { */
        /*     const DirectoryEntry* dir = entry->getDir(); */
        /*     llvm::outs() << dir->getName() << " " << theDir->getName() << "\n"; */
        /* } */

        std::cout << "got "
            << "'" << clang::TypeWithKeyword::getTagTypeKindName(D->getTagKind()) << "'"
            << " named "
            << "'" << D->getQualifiedNameAsString() << "'"
            << " in "
            << "'" << D->getLocation().printToString(*sm) << "'"
            << "\n";

      // prints alotta stuff...
      //D->dump();
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

  llvm::outs() << OptionsParser.getSourcePathList();

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

  // save us which file we are working on
  //theDir = Tool.getFiles().getFile(argv[1])->getDir();

  return Tool.run(newFrontendActionFactory(&Finder));
}
