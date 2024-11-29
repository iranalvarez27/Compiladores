#include <iostream>
#include "exp.h"
#include "visitor.h"
#include <unordered_map>

using namespace std;

unordered_map<std::string, int> memoria;

///////////////////////////////////////////////////////////////////////////////////
// Implementación de las clases accept para las expresiones y sentencias

// ==== BinaryExp ====
int BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

// ==== RelationalExp ====
int RelationalExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

// ==== IdentifierExp ====
int IdentifierExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

// ==== NumberExp ====
int NumberExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

// ==== FuncCallExp ====
int FuncCallExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

// ==== VarDec ====
int VarDec::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

// ==== VarDecList ====
int VarDecList::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

// ==== Param ====
int Param::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

// ==== ParamList ====
int ParamList::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

// ==== FuncDecl ====
int FuncDecl::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

// ==== FuncList ====
int FuncList::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

// ==== Assignment ====
int Assignment::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}


int Operaciones::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

// ==== PrintStmt ====
int PrintStmt::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

// ==== ReturnStatement ====
int ReturnStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

// ==== IfStmt ====
int IfStmt::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

// ==== ForStmt ====
int ForStmt::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

// ==== StatementList ====
int StatementList::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

// ==== FuncCallStmt ====
int FuncCallStmt::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

// ==== ArgList ====
int ArgList::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

// ==== Type ====
int Type::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

// ==== StepCondition ====
int StepCondition::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

// ==== Program ====
int Program::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}


///////////////////////////////////////////////////////////////////////////////////
// Implementación de PrintVisitor para imprimir la estructura del programa

void PrintVisitor::imprimir(Program* program) {
    if (program == nullptr) {
        cout << "El programa es vacío." << endl;
        return;
    }
    program->accept(this); 
}

int PrintVisitor::visit(BinaryExp* exp){
    printIndent();
    exp->left->accept(this);
    cout << " " << CExp::binopToChar(exp->op) << " ";
    exp->right->accept(this);
    cout << endl;
    return 0;
}

int PrintVisitor::visit(NumberExp* exp) {
    printIndent();
    cout << exp->value;
    return 0;
}

int PrintVisitor::visit(IdentifierExp* exp) {
    printIndent();
    cout << exp->name;
    return 0;
}

int PrintVisitor::visit(RelationalExp* exp) {
    printIndent();
    exp->left->accept(this);
    cout << " " << CExp::relopToChar(exp->op) << " ";
    exp->right->accept(this);
    cout << endl;
    return 0;
}

int PrintVisitor::visit(FuncCallExp* exp) {
    printIndent();
    cout << "FuncCallExp" << endl;
    return 0;
}


void PrintVisitor::visit(VarDec* varDec) {
    printIndent();
    cout << varDec->type << " " << varDec->varName << ";" << endl;
}

void PrintVisitor::visit(VarDecList* varDecList) {
    for (VarDec* varDec : varDecList->vardecs) {
        varDec->accept(this);
    }
}

void PrintVisitor::visit(Param* param) {
    printIndent();
    cout << param->type << " " << param->name;
}

void PrintVisitor::visit(ParamList* paramList) {
    for (Param* param : paramList->params) {
        param->accept(this);
        cout << ", ";
    }
}

void PrintVisitor::visit(FuncDecl* funcDecl) {
    printIndent();
    cout << funcDecl->type << " " << funcDecl->name << "(";
    funcDecl->params->accept(this);
    cout << ") {" << endl;
    funcDecl->varDecs->accept(this);
    funcDecl->stmts->accept(this);
    funcDecl->returnStmt->accept(this);
    printIndent();
    cout << "}" << endl;
}

void PrintVisitor::visit(FuncList* funcList) {
    for (FuncDecl* func : funcList->functions) {
        func->accept(this);
    }
}

void PrintVisitor::visit(Assignment* assignment) {
    printIndent();
    cout << assignment->id << " = ";
    assignment->rhs->accept(this);
    cout << ";" << endl;
}

void PrintVisitor::visit(PrintStmt* printStmt) {
    printIndent();
    cout << "printf(" << printStmt->format << ", ";
    printStmt->exp->accept(this);
    cout << ");" << endl;
}

void PrintVisitor::visit(ReturnStatement* returnStmt) {
    printIndent();
    cout << "return ";
    returnStmt->exp->accept(this);
    cout << ";" << endl;
}

void PrintVisitor::visit(IfStmt* ifStmt) {
    printIndent();
    cout << "if (";
    ifStmt->condition->accept(this);
    cout << ") {" << endl;
    ifStmt->thenBody->accept(this);
    cout << "}" << endl;
    if (ifStmt->elseBody) {
        printIndent();
        cout << "else {" << endl;
        ifStmt->elseBody->accept(this);
        cout << "}" << endl;
    }
}

void PrintVisitor::visit(StepCondition* stepCondition) {
    printIndent();
    cout << stepCondition->id << "++;" << endl;
}

void PrintVisitor::visit(ForStmt* forStmt) {
    printIndent();
    cout << "for (";
    forStmt->init->accept(this);
    cout << "; ";
    forStmt->condition->accept(this);
    cout << "; ";
    //forStmt->step->accept(this);
    cout << ") {" << endl;
    forStmt->body->accept(this);
    printIndent();
    cout << "}" << endl;
}

void PrintVisitor::visit(StatementList* stmtList) {
    for (Stmt* stmt : stmtList->statements) {
        stmt->accept(this);
    }
}

void PrintVisitor::visit(Operaciones* op) {
    printIndent();
    cout << op->id << " = ";
    op->rhs->accept(this);
    cout << ";" << endl;
}

void PrintVisitor::visit(FuncCallStmt* funcCallStmt) {
    printIndent();
    cout << ";" << endl;
}

void PrintVisitor::visit(Type* type) {
    cout << type->type;
}

void PrintVisitor::visit(ArgList* argList) {
    for (CExp* arg : argList->args) {
        arg->accept(this);
    }
}

void PrintVisitor::visit(Program* program) {
    program->functions->accept(this);
}

void PrintVisitor::printIndent() {
    for (int i = 0; i < indentLevel * 2; i++) {
        cout << ' ';
    }
}
