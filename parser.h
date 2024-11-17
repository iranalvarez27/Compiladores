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
    Program* parseProgram();
    StmList* parseStmtList();
    Stmt* parseStatement(); 
    VarDecl* parseVarDecl();
    AssignStmt* parseAssignStmt();
    IfStmt* parseIfStmt();
    ForStmt* parseForStmt();
    FuncCall* parseFuncCall();
    FuncDecl* parseFuncDecl();
    PrintStmt* parsePrintStmt();
    //FormatString* parseFormatString();
    Exp* parseExpression();
    Term* parseTerm();
    Factor* parseFactor();
    CExp* parseCExp();
    AExp* parseAExp();
    Type parseType();
public:
    Parser(Scanner* scanner);
    Program* parse(){
        return parseProgram();
    }
};

#endif // PARSER_H