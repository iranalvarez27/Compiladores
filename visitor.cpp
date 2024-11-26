#include <iostream>
#include "exp.h"
#include "visitor.h"
#include <unordered_map>

using namespace std;

unordered_map<std::string, int> memoria;

///////////////////////////////////////////////////////////////////////////////////
// Implementación de las clases accept para las expresiones y sentencias

// ==== CExp ====
int CExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

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

int PrintVisitor::visit(CExp* exp) {
    printIndent();
    cout << "CExp" << endl;
    return 0;
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

///////////////////////////////////////////////////////////////////////////////////
// Implementación de EvalVisitor para evaluar expresiones

void EVALVisitor::ejecutar(Program* program){
    for (FuncDecl* func : program->functions->functions) {
        func->accept(this);
    }   
}

int EVALVisitor::visit(CExp* exp) {
    return exp->accept(this);
}

int EVALVisitor::visit(BinaryExp* exp) {
    int left = exp->left->accept(this);
    int right = exp->right->accept(this);
    switch (exp->op) {
        case BinaryOp::PLUS_OP: return left + right;
        case BinaryOp::MINUS_OP: return left - right;
        case BinaryOp::MUL_OP: return left * right;
        case BinaryOp::DIV_OP: return left / right;
    }
    return 0;
}

int EVALVisitor::visit(RelationalExp* exp) {
    int left = exp->left->accept(this);
    int right = exp->right->accept(this);
    switch (exp->op) {
        case RelOp::LT_OP: return left < right;
        case RelOp::LE_OP: return left <= right;
        case RelOp::GT_OP: return left > right;
        case RelOp::GE_OP: return left >= right;
        case RelOp::EQ_OP: return left == right;
        case RelOp::NE_OP: return left != right;
    }
    return 0;
}

int EVALVisitor::visit(IdentifierExp* exp) {
    if (memoria.find(exp->name) == memoria.end()) {
        throw runtime_error("Variable no inicializada: " + exp->name);
    }
    return memoria[exp->name];
}


int EVALVisitor::visit(NumberExp* exp) {
    return exp->value;
}

int EVALVisitor::visit(FuncCallExp* exp) {
    cout << "FuncCallExp" << endl;
    //Falta implementar
    return 0;
}

void EVALVisitor::visit(IfStmt* ifStmt) {
    if(ifStmt->condition->accept(this)){
        for (Stmt* s : ifStmt->thenBody->statements) {
            s->accept(this);
        }
    }
    else{
        for (Stmt* s : ifStmt->elseBody->statements) {
            s->accept(this);
        }
    }
}

void EVALVisitor::visit(StepCondition* stepCondition) {
    memoria[stepCondition->id]++;
}

void EVALVisitor::visit(ForStmt* forStmt) {
    forStmt->init->accept(this);
    while(forStmt->condition->accept(this)){
        for (Stmt* s : forStmt->body->statements) {
            s->accept(this);
        }
        //forStmt->step->accept(this);
        //operation
        cout << " " << CExp::binopToChar(BinaryOp::PLUS_OP) << " ";
    }
}

void EVALVisitor::visit(ReturnStatement* returnStmt) {
    returnStmt->exp->accept(this);
}

void EVALVisitor::visit(PrintStmt* printStmt) {
    string format = printStmt->format;
    size_t pos = format.find("%");
    if (pos != string::npos) {
        cout << format.substr(0, pos); 
        cout << printStmt->exp->accept(this); 
        cout << format.substr(pos + 2); 
    } else {
        cout << format;
    }
    cout << endl;
}

void EVALVisitor::visit(Assignment* assignment) {
    memoria[assignment->id] = assignment->rhs->accept(this);
}

void EVALVisitor::visit(VarDec* varDec) {
    memoria[varDec->varName] = 0;
}

void EVALVisitor::visit(VarDecList* varDecList) {
    for (VarDec* varDec : varDecList->vardecs) {
        varDec->accept(this);
    }
}

void EVALVisitor::visit(Param* param) {
    memoria[param->name] = 0;
}

void EVALVisitor::visit(ParamList* paramList) {
    for (Param* param : paramList->params) {
        param->accept(this);
    }
}

void EVALVisitor::visit(FuncDecl* funcDecl) {
    funcDecl->varDecs->accept(this);
    funcDecl->stmts->accept(this);
    funcDecl->returnStmt->accept(this);
}

void EVALVisitor::visit(FuncList* funcList) {
    for (FuncDecl* func : funcList->functions) {
        func->accept(this);
    }
}

void EVALVisitor::visit(StatementList* stmtList) {
    for (Stmt* stmt : stmtList->statements) {
        stmt->accept(this);
    }
}

void EVALVisitor::visit(FuncCallStmt* funcCallStmt) {
    cout << "FuncCallStmt" << endl;
}

void EVALVisitor::visit(Type* type) {
    cout << type->type;
}

void EVALVisitor::visit(ArgList* argList) {
    for (CExp* arg : argList->args) {
        arg->accept(this);
    }
}

void EVALVisitor::visit(Program* program) {
    program->functions->accept(this);
}

