#include <iostream>
#include "token.h"

using namespace std;

Token::Token(Type type):type(type) { text = ""; }

Token::Token(Type type, char c):type(type) { text = string(1, c); }

Token::Token(Type type, const string& source, int first, int last):type(type) {
    text = source.substr(first, last);
}


std::ostream& operator << ( std::ostream& outs, const Token & tok )
{
    switch (tok.type) {
        case Token::ID: outs << "TOKEN(ID)"; break;
        case Token::NUM: outs << "TOKEN(NUM)"; break;
        case Token::ASSIGN: outs << "TOKEN(ASSIGN)"; break;
        case Token::RETURN: outs << "TOKEN(RETURN)"; break;
        
        case Token::LT: outs << "TOKEN(LT)"; break;
        case Token::LE: outs << "TOKEN(LE)"; break;
        case Token::EQ: outs << "TOKEN(EQ)"; break;
        case Token::GE: outs << "TOKEN(GE)"; break;
        case Token::GT: outs << "TOKEN(GT)"; break;
        case Token::NE: outs << "TOKEN(NE)"; break;

        case Token::INT: outs << "TOKEN(INT)"; break;
        case Token::LONG: outs << "TOKEN(LONG)"; break;
        case Token::FLOAT: outs << "TOKEN(FLOAT)"; break;
        case Token::DOUBLE: outs << "TOKEN(DOUBLE)"; break;
        case Token::CHAR: outs << "TOKEN(CHAR)"; break;
        case Token::VOID: outs << "TOKEN(VOID)"; break;
        case Token::STRING: outs << "TOKEN(STRING)"; break; //Para la gramática, printf acepta una cadena (string) como primer argumento. 
        case Token::END: outs << "TOKEN(END)"; break;
        case Token::IF: outs << "TOKEN(IF)"; break;
        case Token::ELSE: outs << "TOKEN(ELSE)"; break;
        case Token::FOR: outs << "TOKEN(FOR)"; break;
        case Token::PRINTF: outs << "TOKEN(PRINTF)"; break;
        case Token::PTJ: outs << " TOKEN(PTJ)"; break;
        case Token::WHILE: outs << "TOKEN(WHILE)"; break;

        case Token::PLUS: outs << "TOKEN(PLUS)"; break;
        case Token::MINUS: outs << "TOKEN(MINUS)"; break;
        case Token::MUL: outs << "TOKEN(MUL)"; break;
        case Token::DIV: outs << "TOKEN(DIV)"; break;
        case Token::ERR: outs << "TOKEN(ERR)"; break;

        case Token::PD: outs << "TOKEN(PD)"; break;
        case Token::PI: outs << "TOKEN(PI)"; break;
        case Token::LLI: outs << "TOKEN(LLI)"; break;
        case Token::LLD: outs << "TOKEN(LLD)"; break;
        case Token::PLUS_PLUS: outs <<"TOKEN(PLUS_PLUS)"; break;
        case Token::MINUS_MINUS: outs <<"TOKEN(MINUS_MINUS)"; break;

        case Token::SEMICOLON: outs << "TOKEN(SEMICOLON)"; break;
        case Token::COMMA: outs << "TOKEN(COMMA)"; break;
        case Token::COMILLA: outs << "TOKEN(COMILLA)"; break;
        default: outs << "TOKEN(UNKNOWN)"; break;
    }
    return outs;
}

std::ostream& operator << ( std::ostream& outs, const Token* tok ) {
    return outs << *tok;
}

std::string Token::typeToString(Token::Type type) {
    switch (type) {
        case INT: return "INT";
        case ID: return "ID";
        case ASSIGN: return "ASSIGN";
        case SEMICOLON: return "SEMICOLON";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case MUL: return "MUL";
        case DIV: return "DIV";
        case PI: return "PI";
        case PD: return "PD";
        case PRINTF: return "PRINTF";
        case RETURN: return "RETURN";
        case IF: return "IF";
        case ELSE: return "ELSE";
        case FOR: return "FOR";
        case LLI: return "LLI";
        case LLD: return "LLD";
        case ERR: return "ERR";
        case NUM: return "NUM";
        case STRING: return "STRING";
        case END: return "END";
        default: return "ERR";
    }
}