#ifndef TYPE_VISITOR
#define TYPE_VISITOR

#include "exp.h"
#include "imp_type.hh"

class TypeVisitor {
public:
  virtual void visit(Program* p) = 0;
  virtual void visit(FuncList* b) = 0; 
  virtual void visit(VarDecList* e) = 0;
  virtual void visit(VarDec* e) = 0;
  virtual void visit(FuncDecl* e) = 0;
  virtual void visit(ParamList* e) = 0;
  virtual void visit(Param* e) = 0;
  virtual void visit(StatementList* e) = 0;
  virtual void visit(Assignment* e) = 0;
  virtual void visit(PrintStmt* e) = 0;
  virtual void visit(IfStmt* e) = 0;
  virtual void visit(ReturnStatement* e) = 0;
  virtual void visit(ForStmt* e) = 0;
  virtual void visit(WhileStmt* e) = 0;
  virtual void visit(FuncCallStmt* e) = 0;
  virtual void visit(StepCondition* e) = 0;
  virtual void visit(Operaciones* e) = 0;
  virtual void visit(Type* e) = 0;
  virtual void visit(ArgList* e) = 0;

  
  virtual ImpType visit(BinaryExp* e) = 0;
  virtual ImpType visit(NumberExp* e) = 0;
  virtual ImpType visit(IdentifierExp* e) = 0;
  virtual ImpType visit(RelationalExp* e) = 0;
  virtual ImpType visit(FuncCallExp* e) = 0;
};


#endif