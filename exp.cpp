#include <iostream>
#include "exp.h"
using namespace std;

Program::Program() {}
Program::~Program() {
    for (StmList* s : slist) {
        delete s;
    }
}
void Program::add(StmList* s) {
    slist.push_back(s);
}


StmList::StmList(Stmt* s) {
    slist.push_back(s);
}
void StmList::add(Stmt* s) {
    slist.push_back(s);
}
StmList::~StmList() {
    for (Stmt* s : slist) {
        delete s;
    }
}


Stmt::~Stmt() {}


VarDecl::VarDecl(Type t, string i):type(t),id(i) {}
VarDecl::~VarDecl() {}


AssignStmt::AssignStmt(string id, Exp* e): id(id), rhs(e) {}
AssignStmt::~AssignStmt() {
    delete rhs;
}


IfStmt::IfStmt(Exp* c, StmList* t, StmList* e):cond(c),then(t),els(e) {}
IfStmt::~IfStmt() {
    delete cond;
    delete then;
    delete els;
}


ForStmt::ForStmt(Stmt* i, Exp* c, Stmt* u, StmList* b):init(i),cond(c),update(u),body(b) {}
ForStmt::~ForStmt() {
    delete init;
    delete cond;
    delete update;
    delete body;
}


FuncCall::FuncCall(string i):id(i) {}
FuncCall::~FuncCall() {
    for (Exp* e : args) {
        delete e;
    }
}


FuncDecl::FuncDecl(Type t, string i):type(t),id(i),body(new StmList(nullptr)) {}
FuncDecl::~FuncDecl() {
    for (VarDecl* v : params) {
        delete v;
    }
    delete body;
}


PrintStmt::PrintStmt(Exp* e):e(e) {}
PrintStmt::~PrintStmt() {
    delete e;
}


CExp::CExp(Exp* l, Exp* r, CompOp o):left(l),right(r),op(o) {}
CExp::~CExp() {
    delete left;
    delete right;
}
