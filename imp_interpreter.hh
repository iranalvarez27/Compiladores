#ifndef IMP_INTERPRETER
#define IMP_INTERPRETER

#include <unordered_map>

#include "exp.h"
#include "imp_value_visitor.hh"
#include "environment.hh"

using namespace std;

class ImpInterpreter : public ImpValueVisitor {
private:
  Environment<ImpValue> env;
  Environment<FuncDecl*> fdecs;
  ImpValue retval;
  bool retcall;

public:
  void interpret(Program*);
  void visit(Program*);
  void visit(FuncList*);
  void visit(FuncDecl*);
  void visit(VarDecList*);
  void visit(VarDec*);
  void visit(ParamList*);
  void visit(Param*);  
  void visit(StatementList*);
  void visit(Assignment*);
  void visit(PrintStmt*);
  void visit(IfStmt*);
  void visit(ReturnStatement*);
  void visit(ForStmt*);
  void visit(FuncCallStmt*);
  void visit(StepCondition*);
  void visit(Operaciones*);
  void visit(Type*);
  void visit(ArgList*);


  ImpValue visit(BinaryExp* e);
  ImpValue visit(NumberExp* e);
  ImpValue visit(IdentifierExp* e);
  ImpValue visit(RelationalExp* e);
  ImpValue visit(FuncCallExp* e);
};


#endif

