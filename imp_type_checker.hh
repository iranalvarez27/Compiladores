#ifndef IMP_TYPECHECKER
#define IMP_TYPECHECKER

#include <unordered_map>
#include <vector>

#include "exp.h"
#include "type_visitor.h"
#include "environment.hh"

using namespace std;


class FEntry {
public:
  string fname;
  ImpType ftype;
  int mem_locals, max_stack;
};

class ImpTypeChecker : public TypeVisitor {
public:
  ImpTypeChecker();
  Environment<FEntry> ftable;  
private:
  Environment<ImpType> env;
  bool has_main;
  ImpType booltype;
  ImpType inttype;
  ImpType voidtype;
  ImpType maintype;
  ImpType longtype;

  vector<string> fnames; 
  int sp, max_sp;
  int dir, max_dir;
  void sp_decr(int n);
  void sp_incr(int n);

public:
  void typecheck(Program*);
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
  void visit(WhileStmt*);
  void visit(FuncCallStmt*);
  void visit(StepCondition*);
  void visit(Operaciones*);
  void visit(Type*);
  void visit(ArgList*);
  
  ImpType visit(BinaryExp* e);
  ImpType visit(NumberExp* e);
  ImpType visit(IdentifierExp* e);
  ImpType visit(RelationalExp* e);
  ImpType visit(FuncCallExp* e);
private:
  void add_fundec(FuncDecl* fd);
  ImpType process_fcall(FuncCallExp*);
};


#endif
