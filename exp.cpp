#include "exp.h"
#include "visitor.h"

using namespace std;

// Implementación de destructores y constructores
// ==== Stm ====
Stmt::~Stmt() {}

// ==== CExp ====
CExp::~CExp() {}

// ==== BinaryExp ====
BinaryExp::BinaryExp(CExp* left, CExp* right, BinaryOp op) 
    : left(left), right(right), op(op) {}

BinaryExp::~BinaryExp() {
    delete left;
    delete right;
}

// ==== RelationalExp ====
RelationalExp::RelationalExp(CExp* left, CExp* right, RelOp op) 
    : left(left), right(right), op(op) {}

RelationalExp::~RelationalExp() {
    delete left;
    delete right;
}

// ==== Factor ====
Factor::~Factor() {}

// ==== IdentifierExp ====
IdentifierExp::IdentifierExp(const string& name) : name(name) {}

IdentifierExp::~IdentifierExp() {}

// ==== NumberExp ====
NumberExp::NumberExp(int value) : value(value) {}

NumberExp::~NumberExp() {}

// ==== FuncCallExp ====
FuncCallExp::FuncCallExp(string name, ArgList* args) 
    : name(name), args(args) {}

FuncCallExp::~FuncCallExp() {
    delete args;
}

// ==== FuncCallStmt ====
FuncCallStmt::FuncCallStmt(FuncCallExp* funcCall) 
    : funcCall(funcCall) {}

FuncCallStmt::~FuncCallStmt() {
    delete funcCall;
}

// ==== Type ====
Type::Type(string type) : type(type) {}
Type::~Type() {}

// ==== ArgList ====
ArgList::ArgList() {}
ArgList::~ArgList() {
    for (CExp* arg : args) {
        delete arg;
    }
}
void ArgList::add(CExp* arg) {
    args.push_back(arg);
}

// ==== VarDec ====
VarDec::VarDec(string type, string varName) : type(type), varName(varName) {}

VarDec::~VarDec() {}

// ==== VarDecList ====
VarDecList::VarDecList() {}

void VarDecList::add(VarDec* vardec) {
    vardecs.push_back(vardec);
}

VarDecList::~VarDecList() {
    for (VarDec* vardec : vardecs) {
        delete vardec;
    }
}

// ==== Param ====
Param::Param(string type, string name) : type(type), name(name) {}

Param::~Param() {}

// ==== ParamList ====
ParamList::ParamList() {}

void ParamList::add(Param* param) {
    params.push_back(param);
}

ParamList::~ParamList() {
    for (Param* param : params) {
        delete param;
    }
}

// ==== FuncDecl ====
FuncDecl::FuncDecl(string type, string name, ParamList* params, VarDecList* varDecs, StatementList* stmts, ReturnStatement* returnStmt)
    : type(type), name(name), params(params), varDecs(varDecs), stmts(stmts), returnStmt(returnStmt) {}

FuncDecl::~FuncDecl() {
    delete params;
    delete varDecs;
    delete stmts;
    delete returnStmt;
}


// ==== FuncList ====
FuncList::FuncList() {}

void FuncList::add(FuncDecl* funcDecl) {
    functions.push_back(funcDecl);
}

FuncList::~FuncList() {
    for (FuncDecl* func : functions) {
        delete func;
    }
}

// ==== Assignment ====
Assignment::Assignment(string id, CExp* rhs) 
    : id(id), rhs(rhs) {}

Assignment::~Assignment() {
    delete rhs;
}

// ==== PrintStmt ====
PrintStmt::PrintStmt(string format, CExp* exp) 
    : format(format), exp(exp) {}

PrintStmt::~PrintStmt() {
    delete exp;
}

// ==== ReturnStatement ====
ReturnStatement::ReturnStatement(CExp* exp) 
    : exp(exp) {}

ReturnStatement::~ReturnStatement() {
    delete exp;
}

// ==== IfStmt ====
IfStmt::IfStmt(CExp* condition, StatementList* thenBody, StatementList* elseBody) 
    : condition(condition), thenBody(thenBody), elseBody(elseBody) {}

IfStmt::~IfStmt() {
    delete condition;
    delete thenBody;
    delete elseBody;
}

// ==== StepCondition ====
StepCondition::StepCondition(string id) : id(id) {}

StepCondition::~StepCondition() {
    
}

// ==== ForStmt ====
ForStmt::ForStmt(Assignment* init, CExp* condition, StepCondition* step, StatementList* body) 
    : init(init), condition(condition), step(step), body(body) {}
ForStmt::~ForStmt() {
    delete init;
    delete condition;
    delete step;
    delete body;
}

// ==== StatementList ====
StatementList::StatementList() {}

void StatementList::add(Stmt* statement) {
    statements.push_back(statement);
}

StatementList::~StatementList() {
    for (Stmt* stmt : statements) {
        delete stmt;
    }
}

// ==== Program ====
Program::Program(FuncList* functions) 
    : functions(functions) {}

Program::~Program() {
    delete functions;
}

// ==== Operaciones ====
Operaciones::Operaciones(string id, CExp* rhs) 
    : id(id), rhs(rhs) {}

Operaciones::~Operaciones() {
    delete rhs;
}
