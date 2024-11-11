#ifndef EXP_H
#define EXP_H

#include <string>
#include <unordered_map>
#include <list>

enum BinaryOp { PLUS_OP, MINUS_OP, MUL_OP, DIV_OP };
enum CompOp { LT_OP, LE_OP, EQ_OP, GE_OP, GT_OP, NE_OP };
enum Type { INT_T, LONG_T, FLOAT_T, DOUBLE_T, CHAR_T, VOID_T, STRING_T };

// Clase base Stmt
class Stmt {
    public:
        virtual ~Stmt() = 0;
};

// VarDecl ::= Type id
class VarDecl : public Stmt {
    public:
        Type type;
        std::string id;
        VarDecl(Type t, std::string i);
        ~VarDecl();
};

// StmList ::= Stmt (';' Stmt)*
class StmList {
    public:
        StmList();
        StmList(Stmt* s);
        void add(Stmt* s);
        ~StmList();
    
    private:
        std::list<Stmt*> slist;
};

// Clase base Exp
class Exp {
    public:
        virtual ~Exp() = 0;
        static char binopToChar(BinaryOp op);
};

// CExp ::= AExp ( '<' | '<=' | '==' | '>=' | '>' | '!=' ) AExp
class CExp {
    public:
        Exp* left;
        Exp* right;
        CompOp op;
        CExp(Exp* l, Exp* r, CompOp o);
        ~CExp();
};

// AExp ::= Term (('+' | '-') Term)*
class AExp : public Exp {
    public:
        std::list<Exp*> terms;
        std::list<BinaryOp> ops;
        AExp(Exp* t);
        void add(Exp* t, BinaryOp o);
        ~AExp();
};

// Term ::= Factor (('*' | '/') Factor)*
class Term : public Exp {
    public:
        std::list<Exp*> factors;
        std::list<BinaryOp> ops;
        Term(Exp* f);
        void add(Exp* f, BinaryOp o);
        ~Term();
};

// Factor ::= '(' AExp ')' | num | id
class Factor : public Exp {
    public:
        Exp* e;
        Factor(Exp* e);
        Factor(const std::string& s);
        ~Factor();
};

// FormatString para la impresión
class FormatString {
    public:
        std::string format;
        std::list<Exp*> args;
        FormatString();
        FormatString(int type);
        void add(Exp* e);
        ~FormatString();
};

// AssignStmt ::= id '=' Exp
class AssignStmt : public Stmt {
    public:
        std::string id;
        Exp* rhs;
        AssignStmt(std::string id, Exp* e);
        ~AssignStmt();
};

// IfStmt ::= 'if' '(' CExp ')' StmList ('else' StmList)?
class IfStmt : public Stmt {
    public:
        CExp* cond;
        StmList* then;
        StmList* els;
        IfStmt(CExp* c, StmList* t, StmList* e);
        ~IfStmt();
};

// ForStmt ::= 'for' '(' Stmt CExp Stmt ')' StmList
class ForStmt : public Stmt {
    public:
        Stmt* init;
        CExp* cond;
        Stmt* update;
        StmList* body;
        ForStmt(Stmt* i, CExp* c, Stmt* u, StmList* b);
        ~ForStmt();
};

// FuncCall ::= id '(' (Exp (',' Exp)*)? ')'
class FuncCall : public Stmt {
    public:
        std::string id;
        std::list<Exp*> args;
        FuncCall(std::string i);
        void add(Exp* e);
        ~FuncCall();
};

// FuncDecl ::= Type id '(' (VarDecl (',' VarDecl)*)? ')' StmList
class FuncDecl : public Stmt {
    public:
        Type type;
        std::string id;
        std::list<VarDecl*> params;
        StmList* body;
        FuncDecl(Type t, std::string i);
        void add(VarDecl* v);
        ~FuncDecl();
};

// PrintStmt ::= 'print' '(' Exp ')'
class PrintStmt : public Stmt {
    public:
        Exp* e;
        PrintStmt(Exp* e);
        ~PrintStmt();
};

// Program ::= StmList*
class Program {
    public:
        Program();
        void add(StmList* s);
        ~Program();
    
    private:
        std::list<StmList*> slist;
};

#endif // EXP_H
