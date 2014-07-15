#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include <clang/AST/ASTContext.h>
#include <iostream>

using namespace clang;

class FindNamedClassVisitor
  : public RecursiveASTVisitor<FindNamedClassVisitor> {
public:
  explicit FindNamedClassVisitor(ASTContext *Context)
    : Context(Context) {
    
    }

#if 0 
  bool VisitCXXRecordDecl(CXXRecordDecl *Declaration) {
    FullSourceLoc FullLocation = Context->getFullLoc(Declaration->getLocStart());
    std::cerr << "########################" << std::endl;
    std::cerr << Declaration->getQualifiedNameAsString() << std::endl;
    for(auto it = attr_begin(); it != attr_end(); it++){
        it->
    }

    //Declaration->getCaptureFields();
//   Declaration->dump();



 //   std::cerr << "in" << std::endl;
 //   FullLocation.dump();
    
    //std::cerr << "Found: " << Declaration->getCannoncialDecl() << std::endl;

      /*
    if (Declaration->getQualifiedNameAsString() == "n::m::C") {
      FullSourceLoc FullLocation = Context->getFullLoc(Declaration->getLocStart());
      if (FullLocation.isValid())
        llvm::outs() << "Found declaration at "
                     << FullLocation.getSpellingLineNumber() << ":"
                     << FullLocation.getSpellingColumnNumber() << "\n";
    }
    */
    return true;
  }
#endif
#if 1 
  bool VisitFieldDecl(FieldDecl *F){
    std::cerr << "------------Field-----------------------------" << std::endl;
    F->dump();
    //std::cerr << F->getQualifiedNameAsString()  << " " << F->getBitWidthValue(*Context) << " " << std::endl;
    std::cerr << F->getQualifiedNameAsString()  << std::endl;
    return true;
  }
#endif
#if 0 
  bool VisitType(Type *T){
    auto a = Context->getTypeInfo(T);
    auto s = a.first;
    auto align = a.second;
    if(T->getAsCXXRecordDecl()){
        T->getAsCXXRecordDecl()->dump();//  T->getTypeClassName() <<std::endl;
        std::cerr << "--------- Type ------------------------" << std::endl;
        std::cerr << T->getTypeClassName() <<std::endl;
        std::cerr << "Size: " << s << " alignment: " << align << std::endl;
    }

//    T->dump();
    return true;
  }
#endif
#if 0 
  bool VisitAttr(Attr *A){
      std::cerr << A->getSpelling() << std::endl;
    return true;
  }
#endif

private:
  ASTContext *Context;
};

class FindNamedClassConsumer : public clang::ASTConsumer {
public:
  explicit FindNamedClassConsumer(ASTContext *Context)
    : Visitor(Context) {}

  virtual void HandleTranslationUnit(clang::ASTContext &Context) {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
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

int main(int argc, char **argv) {
  if (argc > 1) {
    clang::tooling::runToolOnCode(new FindNamedClassAction, argv[1]);
  }
}
