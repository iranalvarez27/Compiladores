#ifndef EXP_H
#define EXP_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Clase base para Expresiones
class Expr {
public:
    virtual ~Expr() = default;
    virtual void print() const = 0; // Para depuración
};

// Clase base para Sentencias
class Stmt {
public:
    virtual ~Stmt() = default;
    virtual void print() const = 0;
};

// ----------------------------- AST para Expresiones -----------------------------

class AExp : public Expr {
public:
    virtual ~AExp() = default;
};

class Term : public AExp {
public:
    virtual ~Term() = default;
};

class Factor : public Term {
public:
    string value;
    
    Factor(const string& val) : value(val) {}

    void print() const override {
        cout << "Factor: " << value << endl;
    }
};

class BinOp : public AExp {
public:
    AExp* left;
    string op;
    AExp* right;

    BinOp(AExp* l, const string& operation, AExp* r) : left(l), op(operation), right(r) {}

    void print() const override {
        cout << "BinOp: " << op << endl;
        left->print();
        right->print();
    }
};

// ----------------------------- AST para Sentencias -----------------------------

class VarDecl : public Stmt {
public:
    string type;
    string id;

    VarDecl(const string& t, const string& id) : type(t), id(id) {}

    void print() const override {
        cout << "VarDecl: " << type << " " << id << endl;
    }
};

class Assignment : public Stmt {
public:
    string id;
    AExp* expr;

    Assignment(const string& id, AExp* e) : id(id), expr(e) {}

    void print() const override {
        cout << "Assignment: " << id << " = ";
        expr->print();
    }
};

class IfStmt : public Stmt {
public:
    Expr* condition;
    vector<Stmt*> thenStmtList;
    vector<Stmt*> elseStmtList;

    IfStmt(Expr* cond) : condition(cond) {}

    void addThenStmt(Stmt* stmt) {
        thenStmtList.push_back(stmt);
    }

    void addElseStmt(Stmt* stmt) {
        elseStmtList.push_back(stmt);
    }

    void print() const override {
        cout << "IfStmt: " << endl;
        cout << "Condition: ";
        condition->print();
        cout << "Then block:" << endl;
        for (const auto& stmt : thenStmtList) stmt->print();
        if (!elseStmtList.empty()) {
            cout << "Else block:" << endl;
            for (const auto& stmt : elseStmtList) stmt->print();
        }
    }
};

class ForStmt : public Stmt {
public:
    VarDecl* varDecl;
    Expr* condition;
    Assignment* assignment;
    vector<Stmt*> stmtList;

    ForStmt(VarDecl* v, Expr* c, Assignment* a) : varDecl(v), condition(c), assignment(a) {}

    void addStmt(Stmt* stmt) {
        stmtList.push_back(stmt);
    }

    void print() const override {
        cout << "ForStmt: " << endl;
        varDecl->print();
        cout << "Condition: ";
        condition->print();
        cout << "Assignment: ";
        assignment->print();
        cout << "Body:" << endl;
        for (const auto& stmt : stmtList) stmt->print();
    }
};

class FuncCall : public Stmt {
public:
    string id;
    vector<AExp*> args;

    FuncCall(const string& id) : id(id) {}

    void addArg(AExp* arg) {
        args.push_back(arg);
    }

    void print() const override {
        cout << "FuncCall: " << id << "(";
        for (size_t i = 0; i < args.size(); ++i) {
            args[i]->print();
            if (i < args.size() - 1) cout << ", ";
        }
        cout << ")" << endl;
    }
};

class Print : public Stmt {
public:
    string format;
    vector<AExp*> args;

    Print(const string& format) : format(format) {}

    void addArg(AExp* arg) {
        args.push_back(arg);
    }

    void print() const override {
        cout << "Print: " << format << " ";
        for (size_t i = 0; i < args.size(); ++i) {
            args[i]->print();
            if (i < args.size() - 1) cout << ", ";
        }
        cout << endl;
    }
};

// ----------------------------- AST para el Programa -----------------------------

class Program {
public:
    vector<Stmt*> stmtList;

    void addStmt(Stmt* stmt) {
        stmtList.push_back(stmt);
    }

    void print() const {
        for (const auto& stmt : stmtList) {
            stmt->print();
        }
    }
};

#endif // EXP_H
