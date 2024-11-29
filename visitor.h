#ifndef VISITOR_H
#define VISITOR_H

#include "exp.h"
#include <list>
#include <string>

class CExp;
class BinaryExp;
class RelationalExp;
class IdentifierExp;
class NumberExp;
class FuncCallExp;
class VarDec;
class VarDecList;
class Param;
class ParamList;
class FuncDecl;
class FuncList;
class Assignment;
class PrintStmt;
class ReturnStatement;
class IfStmt;
class ForStmt;
class StatementList;
class FuncCallStmt;
class FuncCallExp;
class Type;
class ArgList;
class StepCondition;
class Operaciones;
class Program;


class Visitor {
public:
    virtual int visit(BinaryExp* exp) = 0;
    virtual int visit(RelationalExp* exp) = 0;
    virtual int visit(IdentifierExp* exp) = 0;
    virtual int visit(NumberExp* exp) = 0;
    virtual int visit(FuncCallExp* exp) = 0;
    virtual void visit(Type* type) = 0;

    virtual void visit(VarDec* varDec) = 0;
    virtual void visit(VarDecList* varDecList) = 0;
    virtual void visit(Param* param) = 0;
    virtual void visit(ParamList* paramList) = 0;
    virtual void visit(FuncDecl* funcDecl) = 0;
    virtual void visit(FuncList* funcList) = 0;
    virtual void visit(ArgList* argList) = 0;
    virtual void visit(Operaciones* op) = 0;

    virtual void visit(Assignment* assignment) = 0;
    virtual void visit(PrintStmt* printStmt) = 0;
    virtual void visit(ReturnStatement* returnStmt) = 0;
    virtual void visit(IfStmt* ifStmt) = 0;
    virtual void visit(ForStmt* forStmt) = 0;
    virtual void visit(StatementList* stmtList) = 0;
    virtual void visit(FuncCallStmt* funcCallStmt) = 0;
    virtual void visit(StepCondition* stepCondition) = 0;

    virtual void visit(Program* program) = 0;
};

// Implementación de PrintVisitor para imprimir la estructura del programa
class PrintVisitor : public Visitor {
public:
    void imprimir(Program* program);
    int visit(BinaryExp* exp) override;
    int visit(RelationalExp* exp) override;
    int visit(IdentifierExp* exp) override;
    int visit(NumberExp* exp) override;

    void visit(VarDec* varDec) override;
    void visit(VarDecList* varDecList) override;
    void visit(Param* param) override;
    void visit(ParamList* paramList) override;
    void visit(FuncDecl* funcDecl) override;
    void visit(FuncList* funcList) override;
    void visit(ArgList* argList) override;

    void visit(Assignment* assignment) override;
    void visit(PrintStmt* printStmt) override;
    void visit(ReturnStatement* returnStmt) override;
    void visit(FuncCallStmt* funcCallStmt) override;
    void visit(IfStmt* ifStmt) override;
    void visit(ForStmt* forStmt) override;
    void visit(StatementList* stmtList) override;
    void visit(StepCondition* stepCondition) override;
    void visit(Operaciones* op) override;

    void visit(Type* type) override;
    int visit(FuncCallExp* exp) override;
    
    void visit(Program* program) override;

private:
    int indentLevel = 0;
    void increaseIndent() { indentLevel++; }
    void decreaseIndent() { indentLevel--; }
    void printIndent();
};

#endif // VISITOR_H