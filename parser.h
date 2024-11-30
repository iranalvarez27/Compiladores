 #ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "exp.h"

class Parser {
private:
    Scanner* scanner;
    Token *current, *previous;

    bool match(Token::Type ttype);
    bool check(Token::Type ttype);
    bool advance();
    bool isAtEnd();
    CExp* parseExp();
    CExp* parseTerm();
    CExp* parseFactor();

public:
    Parser(Scanner* scanner);
    Program* parseProgram();
    FuncList* parseFuncList();
    FuncDecl* parseFuncDecl();
    VarDecList* parseVarDecList();
    VarDec* parseVarDec();
    ParamList* parseParamList();
    Param* parseParam();
    StatementList* parseStatementList();
    Stmt* parseStatement();
    Assignment* parseAssignment();
    PrintStmt* parsePrintStmt();
    ReturnStatement* parseReturnStmt();
    IfStmt* parseIfStmt();
    ForStmt* parseForStmt();
    WhileStmt* parseWhileStmt();
    FuncCallExp* parseFuncCallExp();
    FuncCallStmt* parseFuncCallStmt();
    ArgList* parseArgList();
    RelationalExp* parseRelationalExp();
    StepCondition* parseStepCondition();

    CExp* parseCExp();
    
};

#endif // PARSER_H