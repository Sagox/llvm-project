#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/Expr.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include <string>

using namespace clang;

class FindNamedClassVisitor
  : public RecursiveASTVisitor<FindNamedClassVisitor> {
public:
  explicit FindNamedClassVisitor(ASTContext *Context)
    : Context(Context) {
      SrcManager = &(Context->getSourceManager());
    }

  bool VisitStmt(Stmt* S) {
    unsigned int lineNum, coloumnNum;
    // llvm::outs() << S->getStmtClassName() <<"\n";
    std::string a = S->getStmtClassName();
    if(a == "BinaryOperator") {
      if(dyn_cast<BinaryOperator>(S)->isEqualityOp()) {
        for(auto i: S->children()) {
          std::string b = i->getStmtClassName();
          auto sl = i->getBeginLoc();
          // SourceLocation::dump(S->Context->getSourceManager());
          if(b == "FloatingLiteral") {
            llvm::outs() << "Potential error" << "\n";
            // sl.dump(*SrcManager);
            lineNum = SrcManager->getExpansionLineNumber(sl);
            coloumnNum = SrcManager->getExpansionColumnNumber(sl);
            llvm::outs() << lineNum << ":" << coloumnNum << "\n";
            break;
          }
          else if(b == "ImplicitCastExpr") {
            if(std::string(dyn_cast<CastExpr>(i)->getCastKindName()) == "FloatingCast") {
                llvm::outs() << "Potential error" << "\n";
                // sl.dump(*SrcManager);
                lineNum = SrcManager->getExpansionLineNumber(sl);
                coloumnNum = SrcManager->getExpansionColumnNumber(sl);
                llvm::outs() << lineNum << ":" << coloumnNum << "\n";
                break;              
            }
          }
        }
      }
      // S->dump();
    }
    // S->dump();
    // RecursiveASTVisitor<FindNamedClassVisitor>::TraverseStmt(S);
    return true;
  }

private:
  SourceManager* SrcManager;
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
  virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
    clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
    return std::unique_ptr<clang::ASTConsumer>(
        new FindNamedClassConsumer(&Compiler.getASTContext()));
  }
};

int main(int argc, char **argv) {
  if (argc > 1) {
    clang::tooling::runToolOnCode(new FindNamedClassAction(), argv[1]);
  }
}
