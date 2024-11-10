#include <iostream>
#include <stdexcept>
#include "token.h"
#include "scanner.h"
#include "parser.h"

using namespace std;

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERR)) {
            cout << "Error de análisis, carácter no reconocido: " << current->text << endl;
            exit(1);
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}

Parser::Parser(Scanner* sc):scanner(sc) {
    previous = NULL;
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        cout << "Error en el primer token: " << current->text << endl;
        exit(1);
    }
}

Program* Parser::parseProgram() {
    StmList* s = parseStmtList();
    if (s == NULL) {
        throw runtime_error("Error: se esperaba una lista de declaraciones.");
    }
    Program* p = new Program();
    p->add(s);
    return p;
}

//StmtList      ::= Stmt (';' Stmt)*
StmList* Parser::parseStmtList() {
    StmList* s = new StmList();
    try {
        while (!isAtEnd()) {
            s->add(parseStatement());
            if (!isAtEnd() && !match(Token::SEMICOLON)) {
                throw runtime_error("Error: se esperaba ';' al final de la declaración.");
            }
        }
    } catch (const exception& e) {
        cout << "Error durante el parsing: " << e.what() << endl;
        delete s;
        exit(1);
    }
    return s;
}

//Stmt::= VarDecl|AssignStmt|IfStmt|ForStmt|FuncCall|FuncDecl|PrintStmt
Stmt* Parser::parseStatement() {
    Stmt* s = NULL;
    Exp* e;
    
    if (current == NULL) {
        cout << "Error: Token actual es NULL" << endl;
        exit(1);
    }
}

VarDecl* Parser::parseVarDecl(){
    Type t = parseType();
    if (!match(Token::ID)) {
        cout << "Error: se esperaba un identificador después del tipo." << endl;
        exit(1);
    }
    string id = previous->text;
    return new VarDecl(t, id);
}

AssignStmt* Parser::parseAssignStmt(){
    if (!match(Token::ID)) {
        cout << "Error: se esperaba un identificador después del tipo." << endl;
        exit(1);
    }
    string id = previous->text;
    if (!match(Token::ASSIGN)) {
        cout << "Error: se esperaba un '=' después del identificador." << endl;
        exit(1);
    }
    AExp* e = parseAExp();
    return new AssignStmt(id, e);
}

IfStmt* Parser::parseIfStmt(){
    if (!match(Token::IF)) {
        cout << "Error: se esperaba un 'if'." << endl;
        exit(1);
    }
    if (!match(Token::PI)) {
        cout << "Error: se esperaba un '(' después del 'if'." << endl;
        exit(1);
    }
    CExp* c = parseCExp();
    if (!match(Token::PD)) {
        cout << "Error: se esperaba un ')' después de la condición." << endl;
        exit(1);
    }
    if(!match(Token::LLI)){
        cout << "Error: se esperaba un '{' después de la condición." << endl;
        exit(1);
    }
    StmList* t = parseStmtList();
    if(!match(Token::LLD)){
        cout << "Error: se esperaba un '}' después de la condición." << endl;
        exit(1);
    }
    StmList* e = NULL;
    if (match(Token::ELSE)) {
        if(!match(Token::LLI)){
            cout << "Error: se esperaba un '{' después del 'else'." << endl;
            exit(1);
        }
        e = parseStmtList();
        if(!match(Token::LLD)){
            cout << "Error: se esperaba un '}' después del 'else'." << endl;
            exit(1);
        }
    }
    return new IfStmt(c, t, e);
}


ForStmt* Parser::parseForStmt(){
    if (!match(Token::FOR)) {
        cout << "Error: se esperaba un 'for'." << endl;
        exit(1);
    }
    if (!match(Token::PI)) {
        cout << "Error: se esperaba un '(' después del 'for'." << endl;
        exit(1);
    }
    VarDecl* i = parseVarDecl();
    if (!match(Token::SEMICOLON)) {
        cout << "Error: se esperaba un ';' después de la inicialización." << endl;
        exit(1);
    }
    CExp* c = parseCExp();
    if (!match(Token::SEMICOLON)) {
        cout << "Error: se esperaba un ';' después de la condición." << endl;
        exit(1);
    }
    AssignStmt* u = parseAssignStmt();
    if (!match(Token::PD)) {
        cout << "Error: se esperaba un ')' después de la actualización." << endl;
        exit(1);
    }
    if(!match(Token::LLI)){
        cout << "Error: se esperaba un '{' después de la actualización." << endl;
        exit(1);
    }
    StmList* b = parseStmtList();
    if(!match(Token::LLD)){
        cout << "Error: se esperaba un '}' después de la actualización." << endl;
        exit(1);
    }
    return new ForStmt(i, c, u, b);
}

//FuncCall      ::= id '(' [AExp (',' AExp)*] ')'
FuncCall* Parser::parseFuncCall(){
    if (!match(Token::ID)) {
        cout << "Error: se esperaba un identificador después del tipo." << endl;
        exit(1);
    }
    string id = previous->text;
    if (!match(Token::PI)) {
        cout << "Error: se esperaba un '(' después del identificador." << endl;
        exit(1);
    }
    FuncCall* f = new FuncCall(id);
    if(!check(Token::PD)){
        f->add(parseAExp());
        while(match(Token::COMMA)){
            f->add(parseAExp());
        }
    }

    if(!match(Token::PD)){
        cout << "Error: se esperaba un ')' después de la lista de argumentos." << endl;
        exit(1);
    }
    return f;
}

//FuncDecl      ::= Type id '(' [VarDecl (',' VarDecl)*] ')' '{' StmtList '}'
FuncDecl* Parser::parseFuncDecl(){
    Type t = parseType();
    if (!match(Token::ID)) {
        cout << "Error: se esperaba un identificador después del tipo." << endl;
        exit(1);
    }
    string id = previous->text;
    if (!match(Token::PI)) {
        cout << "Error: se esperaba un '(' después del identificador." << endl;
        exit(1);
    }
    FuncDecl* f = new FuncDecl(t, id);
    if(!check(Token::PD)){
        f->add(parseVarDecl());
        while(match(Token::COMMA)){
            f->add(parseVarDecl());
        }
    }
    if(!match(Token::PD)){
        cout << "Error: se esperaba un ')' después de la lista de argumentos." << endl;
        exit(1);
    }
    if(!match(Token::LLI)){
        cout << "Error: se esperaba un '{' después de la lista de argumentos." << endl;
        exit(1);
    }
    f->body = parseStmtList();
    if(!match(Token::LLD)){
        cout << "Error: se esperaba un '}' después de la lista de argumentos." << endl;
        exit(1);
    }
    return f;
}

//Print         ::= 'printf' '(' FormatString ',' AExp ')'

PrintStmt* Parser::parsePrintStmt(){
    if (!match(Token::PRINTF)) {
        cout << "Error: se esperaba un 'printf'." << endl;
        exit(1);
    }
    if (!match(Token::PI)) {
        cout << "Error: se esperaba un '(' después del 'printf'." << endl;
        exit(1);
    }
    FormatString* f = parseFormatString();
    if (!match(Token::COMMA)) {
        cout << "Error: se esperaba una ',' después de la cadena de formato." << endl;
        exit(1);
    }
    AExp* e = parseAExp();
    if (!match(Token::PD)) {
        cout << "Error: se esperaba un ')' después de la expresión." << endl;
        exit(1);
    }
    return new PrintStmt(e);
}

Type Parser::parseType(){
    if (match(Token::INT)) return INT_T;
    if (match(Token::LONG)) return LONG_T;
    if (match(Token::FLOAT)) return FLOAT_T;
    if (match(Token::DOUBLE)) return DOUBLE_T;
    if (match(Token::CHAR)) return CHAR_T;
    if (match(Token::VOID)) return VOID_T;
    if (match(Token::STRING)) return STRING_T;
    cout << "Error: se esperaba un tipo de dato." << endl;
    exit(1);
}

CExp* Parser::parseCExp(){
    AExp* l = parseAExp();
    CompOp o;
    if (match(Token::LT)) o = LT_OP;
    else if (match(Token::LE)) o = LE_OP;
    else if (match(Token::EQ)) o = EQ_OP;
    else if (match(Token::GE)) o = GE_OP;
    else if (match(Token::GT)) o = GT_OP;
    else if (match(Token::NE)) o = NE_OP;
    else {
        cout << "Error: se esperaba un operador de comparación." << endl;
        exit(1);
    }
    Exp* r = parseAExp();
    return new CExp(l, r, o);
}

AExp* Parser::parseAExp(){
    AExp* a = new AExp(parseTerm());
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp o;
        if (previous->type == Token::PLUS) o = PLUS_OP;
        else o = MINUS_OP;
        a->add(parseTerm(), o);
    }
    return a;
}

Term* Parser::parseTerm(){
    Term* t = new Term(parseFactor());
    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp o;
        if (previous->type == Token::MUL) o = MUL_OP;
        else o = DIV_OP;
        t->add(parseFactor(), o);
    }
    return t;
}

Factor* Parser::parseFactor(){
    if (match(Token::PI)) {
        Exp* e = parseAExp();
        if (!match(Token::PD)) {
            cout << "Error: se esperaba un ')' después de la expresión." << endl;
            exit(1);
        }
        return new Factor(e);
    }
    if (match(Token::NUM)) {
        return new Factor(previous->text);
    }
    if (match(Token::ID)) {
        return new Factor(previous->text);
    }
    cout << "Error: se esperaba un número o un identificador." << endl;
    exit(1);
}

//FormatString  ::= " (%d | %ld | %f | %c | %s )"

FormatString* Parser::parseFormatString(){
    if (!match(Token::COMILLA)) {
        cout << "Error: se esperaba una comilla después de 'printf'." << endl;
        exit(1);
    }
    if (!match(Token::COMILLA)) {
        cout << "Error: se esperaba una comilla después de la primera comilla." << endl;
        exit(1);
    }
    if (match(Token::PTJ)) {
        return new FormatString();
    }
    if (match(Token::INT)) return new FormatString(INT_T);
    if (match(Token::LONG)) return new FormatString(LONG_T);
    if (match(Token::FLOAT)) return new FormatString(FLOAT_T);
    if (match(Token::CHAR)) return new FormatString(CHAR_T);
    if (match(Token::STRING)) return new FormatString(STRING_T);
    cout << "Error: se esperaba un tipo de dato." << endl;
    exit(1);
}

