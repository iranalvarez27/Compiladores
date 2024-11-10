#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include "scanner.h"
#include "token.h"

class Parser {
private:
    Scanner* scanner;  
    Token* current;     
    Token* previous;    

    bool match(Token::Type ttype);    
    bool check(Token::Type ttype);    
    bool advance();                   
    bool isAtEnd();                

    void parseProgram();
    void parseStmtList();
    void parseStmt();
    void parseVarDecl();
    void parseAssignment();
    void parseIfStmt();
    void parseForStmt();
    void parseFuncCall();
    void parseFuncDecl();
    void parsePrint();

    void parseType();
    void parseCExp();
    void parseAExp();
    void parseTerm();
    void parseFactor();

public:
    Parser(Scanner* scanner);

    void parse();
};

#endif // PARSER_H
