#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token {
public:
    enum Type {
        ID, NUM, ASSIGN, 
        LT, LE,EQ, GE, GT, NE,
        INT, LONG, FLOAT, DOUBLE, CHAR, VOID, STRING,
        IF, ELSE, FOR, WHILE, PRINTF,
        PLUS, MINUS, MUL, DIV, ERR, 
        PD, PI, LLI, LLD,
        SEMICOLON, COMMA
    };
    Type type;
    std::string text;
    Token(Type);
    Token(Type, char c);
    Token(Type, const std::string& source, int first, int last);

    //Added for testing purposes
    friend std::ostream& operator<<(std::ostream& outs, const Token& tok);
    friend std::ostream& operator<<(std::ostream& outs, const Token* tok);
};

#endif // TOKEN_H

