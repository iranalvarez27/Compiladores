#include "exp.h"
#include <iostream>

// Implementación de Stmt (destructor virtual)
Stmt::~Stmt() {}

// Implementación de Exp (destructor virtual)
Exp::~Exp() {}

char Exp::binopToChar(BinaryOp op) {
    switch (op) {
        case PLUS_OP: return '+';
        case MINUS_OP: return '-';
        case MUL_OP: return '*';
        case DIV_OP: return '/';
    }
    return '?';
}

// Implementación de Program
Program::Program() {}
Program::~Program() {
    for (StmList* s : slist) {
        delete s;
    }
}
void Program::add(StmList* s) {
    slist.push_back(s);
}

// Implementación de StmList
StmList::StmList() {}
StmList::StmList(Stmt* s) {
    add(s);
}
void StmList::add(Stmt* s) {
    slist.push_back(s);
}
StmList::~StmList() {
    for (Stmt* s : slist) {
        delete s;
    }
}

// Implementación de VarDecl
VarDecl::VarDecl(Type t, std::string i) : type(t), id(i) {}
VarDecl::~VarDecl() {}

// Implementación de AssignStmt
AssignStmt::AssignStmt(std::string id, Exp* e) : id(id), rhs(e) {}
AssignStmt::~AssignStmt() {
    delete rhs;
}

// Implementación de IfStmt
IfStmt::IfStmt(CExp* c, StmList* t, StmList* e) : cond(c), then(t), els(e) {}
IfStmt::~IfStmt() {
    delete cond;
    delete then;
    delete els;
}

// Implementación de ForStmt
ForStmt::ForStmt(Stmt* i, CExp* c, Stmt* u, StmList* b) : init(i), cond(c), update(u), body(b) {}
ForStmt::~ForStmt() {
    delete init;
    delete cond;
    delete update;
    delete body;
}

// Implementación de FuncCall
FuncCall::FuncCall(std::string i) : id(i) {}
FuncCall::~FuncCall() {
    for (Exp* e : args) {
        delete e;
    }
}
void FuncCall::add(Exp* e) {
    args.push_back(e);
}

// Implementación de FuncDecl
FuncDecl::FuncDecl(Type t, std::string i) : type(t), id(i), body(new StmList()) {}
FuncDecl::~FuncDecl() {
    for (VarDecl* v : params) {
        delete v;
    }
    delete body;
}
void FuncDecl::add(VarDecl* v) {
    params.push_back(v);
}

// Implementación de PrintStmt
PrintStmt::PrintStmt(Exp* e) : e(e) {}
PrintStmt::~PrintStmt() {
    delete e;
}

// Implementación de CExp
CExp::CExp(Exp* l, Exp* r, CompOp o) : left(l), right(r), op(o) {}
CExp::~CExp() {
    delete left;
    delete right;
}

// Implementación de AExp
AExp::AExp(Exp* t) {
    terms.push_back(t);
}
AExp::~AExp() {
    for (Exp* term : terms) {
        delete term;
    }
}
void AExp::add(Exp* t, BinaryOp o) {
    terms.push_back(t);
    ops.push_back(o);
}

// Implementación de Term
Term::Term(Exp* f) {
    factors.push_back(f);
}
Term::~Term() {
    for (Exp* factor : factors) {
        delete factor;
    }
}
void Term::add(Exp* f, BinaryOp o) {
    factors.push_back(f);
    ops.push_back(o);
}

// Implementación de Factor
Factor::Factor(Exp* e) : e(e) {}
Factor::Factor(const std::string& s) : e(nullptr) {}  // Implementación para cadenas
Factor::~Factor() {
    delete e;
}

// Implementación de FormatString
FormatString::FormatString() {}
FormatString::FormatString(int type) : format(std::to_string(type)) {}
void FormatString::add(Exp* e) {
    args.push_back(e);
}
FormatString::~FormatString() {
    for (Exp* e : args) {
        delete e;
    }
}
