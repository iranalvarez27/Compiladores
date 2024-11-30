#ifndef EXP_H
#define EXP_H


#include "imp_value.hh"
#include "imp_type.hh"
#include <string>
#include <unordered_map>
#include <list>
#include "visitor.h"
#include "type_visitor.h"

using namespace std;

class ImpInterpreter;

// Enumeraciones para operadores relacionales y binarios
enum RelOp { LT_OP, LE_OP, EQ_OP, GT_OP, GE_OP, NE_OP };
enum BinaryOp { PLUS_OP, MINUS_OP, MUL_OP, DIV_OP };
class ImpValueVisitor;

// Representa cualquier expresión del lenguaje
class CExp {
public:
    virtual int accept(Visitor* visitor) = 0;
    virtual ImpValue accept(ImpValueVisitor* v) = 0;
    virtual ImpType accept(TypeVisitor* v) = 0;
    virtual ~CExp() = 0;
    static string binopToChar(BinaryOp op){
        switch (op) {
            case BinaryOp::PLUS_OP: return "+";
            case BinaryOp::MINUS_OP: return "-";
            case BinaryOp::MUL_OP: return "*";
            case BinaryOp::DIV_OP: return "/";
        }
        return "";
    }
    static string relopToChar(RelOp op){
        switch (op) {
            case RelOp::LT_OP: return "<";
            case RelOp::LE_OP: return "<=";
            case RelOp::EQ_OP: return "==";
            case RelOp::GT_OP: return ">";
            case RelOp::GE_OP: return ">=";
            case RelOp::NE_OP: return "!=";
        }
        return "";
    }
};

// Expresión binaria
class BinaryExp : public CExp {
public:
    CExp* left;
    CExp* right;
    BinaryOp op;
    BinaryExp(CExp* left, CExp* right, BinaryOp op);
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ImpType accept(TypeVisitor* v);
    ~BinaryExp();
};

// Expresión relacional
class RelationalExp : public CExp {
public:
    CExp* left;
    CExp* right;
    RelOp op;
    RelationalExp(CExp* left, CExp* right, RelOp op);
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ImpType accept(TypeVisitor* v);
    ~RelationalExp();
};

// Factor: id, número o expresión entre paréntesis
class Factor : public CExp {
public:
    virtual ~Factor() = 0;
};

// Identificador
class IdentifierExp : public Factor {
public:
    string name;
    IdentifierExp(const string& name);
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ImpType accept(TypeVisitor* v);
    ~IdentifierExp();
};

// Número
class NumberExp : public Factor {
public:
    int value;
    NumberExp(int value);
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ImpType accept(TypeVisitor* v);
    ~NumberExp();
};

// Expresión de llamada a función: FuncCallExp ::= id '(' ArgList ')'
class FuncCallExp : public Factor {
public:
    string name;
    ArgList* args;
    FuncCallExp(string name, ArgList* args);
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ImpType accept(TypeVisitor* v);
    ~FuncCallExp();
};

// Declaración de variable
class VarDec {
public:
    string type;
    string varName;
    VarDec(string type, string varName);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~VarDec();
};

// Lista de declaraciones de variables
class VarDecList {
public:
    list<VarDec*> vardecs;
    VarDecList();
    void add(VarDec* vardec);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~VarDecList();
};

// Declaración de parámetro
class Param {
public:
    string type;
    string name;
    Param(string type, string name);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~Param();
};

// Lista de parámetros
class ParamList {
public:
    list<Param*> params;
    ParamList();
    void add(Param* param);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~ParamList();
};

// Declaración de función
class FuncDecl {
public:
    string type;
    string name;
    ParamList* params;
    VarDecList* varDecs;
    StatementList* stmts;
    ReturnStatement* returnStmt;
    FuncDecl(string type, string name, ParamList* params, VarDecList* varDecs, StatementList* stmts, ReturnStatement* returnStmt);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~FuncDecl();
};

// Lista de declaraciones de funciones
class FuncList {
public:
    list<FuncDecl*> functions;
    FuncList();
    void add(FuncDecl* funcDecl);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~FuncList();
};

// Representa cualquier sentencia del lenguaje
class Stmt {
public:
    virtual int accept(Visitor* visitor) = 0;
    virtual void accept(ImpValueVisitor* v) = 0;
    virtual void accept(TypeVisitor* v) = 0;
    virtual ~Stmt() = 0;
};

// Asignación
class Assignment : public Stmt {
public:
    string id;
    CExp* rhs;
    Assignment(string id, CExp* rhs);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~Assignment();
};

// Sentencia de impresión
class PrintStmt : public Stmt {
public:
    string format;
    CExp* exp;    
    PrintStmt(string format, CExp* exp);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    
    ~PrintStmt();
};

// Sentencia de retorno
class ReturnStatement : public Stmt {
public:
    CExp* exp;
    ReturnStatement(CExp* exp);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~ReturnStatement();
};

// Sentencia if-else
class IfStmt : public Stmt {
public:
    CExp* condition;
    StatementList* thenBody;
    StatementList* elseBody;
    IfStmt(CExp* condition, StatementList* thenBody, StatementList* elseBody);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~IfStmt();
};

// Step condicion: StepCondition ::= id '++' | id '--' 
class StepCondition : public Stmt {
public:
    string id;
    StepCondition(string id);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~StepCondition();
};

// Sentencia while
class WhileStmt : public Stmt {
public:
    CExp* condition;
    StatementList* body;
    WhileStmt(CExp* condition, StatementList* body);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~WhileStmt();
};

// Sentencia for
class ForStmt : public Stmt {
public:
    Assignment* init;
    CExp* condition;
    StepCondition* step;
    StatementList* body;
    ForStmt(Assignment* init, CExp* condition, StepCondition* step, StatementList* body);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~ForStmt();
};

// Lista de sentencias
class StatementList {
public:
    list<Stmt*> statements;
    StatementList();
    void add(Stmt* statement);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~StatementList();
};

//FuncCallStmt ::= FuncCall ';'
class FuncCallStmt : public Stmt {
public:
    FuncCallExp* funcCall;
    FuncCallStmt(FuncCallExp* funcCall);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~FuncCallStmt();
};

//Operaciones ::= id '=' CExp ';'
class Operaciones : public Stmt {
public:
    string id;
    CExp* rhs;
    Operaciones(string id, CExp* rhs);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~Operaciones();
};

// Clase tipo
class Type {
public:
    string type;
    Type(string type);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~Type();
};

// Lista de argumentos: ArgList ::= CExp (',' CExp)* | ε
class ArgList {
public:
    list<CExp*> args;
    ArgList();
    void add(CExp* arg);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~ArgList();

};

// Programa completo
class Program {
public:
    FuncList* functions;
    Program(FuncList* functions);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~Program();
};

#endif // EXP_H