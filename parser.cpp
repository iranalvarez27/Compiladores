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

CExp* Parser::parseFactor() {
    if (check(Token::ID)) {
        string id = current->text;
        advance();
        if (check(Token::PI)) {
            return parseFuncCallExp();
        }
        else { // Variable case
            return new IdentifierExp(id);
        }
    }
    else if (match(Token::NUM)) {
        return new NumberExp(stoi(previous->text));
    }
    else if (match(Token::PI)) {
        CExp* exp = parseExp();
        if (!match(Token::PD)) {
            cout << "Error: Se esperaba un ')' en la línea " << previous->text << endl;
            exit(1);
        }
        return exp;
    }
    else if (check(Token::PLUS)) {
        advance();
        return parseFactor();
    } else if (check(Token::MINUS)) {
        advance();
        return new BinaryExp(new NumberExp(0), parseFactor(), BinaryOp::MINUS_OP);
    }
    else {
        cout << "Error: Se esperaba un factor en la línea " << current->text << endl;
        exit(1);
    }
    return nullptr;
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/



CExp* Parser::parseCExp() {
    CExp* left = parseExp();
    if (check(Token::PLUS) || check(Token::MINUS)) {
        Token* op = current;
        advance();
        CExp* right = parseExp();
        return new BinaryExp(left, right, (op->type == Token::PLUS) ? BinaryOp::PLUS_OP : BinaryOp::MINUS_OP);
    } else if (check(Token::ID)) {
        string id = current->text;
        advance();
        if (match(Token::PLUS)) {
            if (!match(Token::ID)) {
                cout << "Error: Se esperaba un identificador en la línea " << current->text << endl;
                exit(1);
            }
            string id2 = previous->text;
            if (!match(Token::SEMICOLON)) {
                cout << "Error: Se esperaba un ';' en la línea " << current->text << endl;
                exit(1);
            }
            return new BinaryExp(new IdentifierExp(id), new IdentifierExp(id2), BinaryOp::PLUS_OP);
        }
    }
    return nullptr;
}



CExp* Parser::parseTerm() {
    CExp* left = parseFactor();
    while (match(Token::MUL) || match(Token::DIV)) {
        Token* op = previous;
        CExp* right = parseFactor();
        left = new BinaryExp(left, right, (op->type == Token::MUL) ? BinaryOp::MUL_OP : BinaryOp::DIV_OP);
    }
    return left;
}


CExp* Parser::parseExp() {
    CExp* left = parseTerm();
    while (check(Token::PLUS) || check(Token::MINUS)) {
        Token* op = current;
        CExp* right = parseTerm();
        left = new BinaryExp(left, right, (op->type == Token::PLUS) ? BinaryOp::PLUS_OP : BinaryOp::MINUS_OP);
    }
    if (check(Token::ID)) {
        Token* currentToken = previous; 
        advance();

        if (check(Token::PI)) { 
            return parseFuncCallExp();
        }

        if (currentToken != previous) {
        }
    }
    
    return left;
}

RelationalExp* Parser::parseRelationalExp() {
    CExp* left = parseExp();
    if (check(Token::LT) || check(Token::LE) || check(Token::EQ) || check(Token::GT) || check(Token::GE) || check(Token::NE)) {
        Token* op = current;
        advance();
        CExp* right = parseExp();
        return new RelationalExp(left, right, (op->type == Token::LT) ? RelOp::LT_OP : (op->type == Token::LE) ? RelOp::LE_OP : (op->type == Token::EQ) ? RelOp::EQ_OP : (op->type == Token::GT) ? RelOp::GT_OP : (op->type == Token::GE) ? RelOp::GE_OP : RelOp::NE_OP);
    }
    return nullptr;
}



Assignment* Parser::parseAssignment() {
    if (!match(Token::ID)) {
        cout << "Error: Se esperaba un identificador en la línea " << previous->text << endl;
        exit(1);
    }
    string id = previous->text;
    if (!match(Token::ASSIGN)) {
        cout << "Error: Se esperaba un '=' en la línea " << previous->text << endl;
        exit(1);
    }
    CExp* rhs = parseExp();
    if (!match(Token::SEMICOLON)) {
        cout << "Error: Se esperaba un ';' en la línea " << previous->text << endl;
        exit(1);
    }
    return new Assignment(id, rhs);
}


PrintStmt* Parser::parsePrintStmt() {
    if (!match(Token::PRINTF)) {
        cout << "Error: Se esperaba un 'printf' en la línea " << previous->text << endl;
        exit(1);
    }
    if(!match(Token::PI)){
        cout << "Error: Se esperaba un '(' en la línea " << previous->text << endl;
        exit(1);
    }
    if(!match(Token::STRING)){
        cout << "Error: Se esperaba una cadena de texto en la línea " << previous->text << endl;
        exit(1);
    }

    string format = previous->text;
    if(!match(Token::COMMA)){
        cout << "Error: Se esperaba una ',' en la línea " << previous->text << endl;
        exit(1);
    }
    CExp* exp = parseExp();
    if(!match(Token::PD)){
        cout << "Error: Se esperaba un ')' en la línea " << previous->text << endl;
        exit(1);
    }
    if(!match(Token::SEMICOLON)){
        cout << "Error: Se esperaba un ';' en la línea " << previous->text << endl;
        exit(1);
    }
    return new PrintStmt(format, exp);
}

ReturnStatement* Parser::parseReturnStmt() {
    if (!check(Token::RETURN)) {
        if(check(Token::LLD)){
            return nullptr;
        }
        cout << "Error: Se esperaba un 'return' en la línea " << previous->text << endl;
        exit(1);
    }
    advance();
    CExp* exp = parseExp();
    if (!check(Token::SEMICOLON)) {
        cout << "Error: Se esperaba un ';' en la línea " << previous->text << endl;
        exit(1);
    }
    return new ReturnStatement(exp);
}


IfStmt* Parser::parseIfStmt(){
    if(!match(Token::IF)){
        cout << "Error: Se esperaba un 'if' en la línea " << previous->text << endl;
        exit(1);
    }
    if (!match(Token::PI)) {
        cout << "Error: Se esperaba un '(' en la línea " << previous->text << endl;
        exit(1);
    }

    CExp* condition = parseRelationalExp();

    if (!match(Token::PD)) {
        cout << "Error: Se esperaba un ')' en la línea " << previous->text << endl;
        exit(1);
    }

    if (!check(Token::LLI)) {
        cout << "Error: Se esperaba un '{' en la línea " << previous->text << endl;
        exit(1);
    }
    StatementList* ifBody = parseStatementList();
    if (!match(Token::LLD)) {
        cout << "Error: Se esperaba un '}' en la línea " << previous->text << endl;
        exit(1);
    }
    StatementList* elseBody = nullptr;
    if (match(Token::ELSE)) {
        if (!match(Token::LLI)) {
            cout << "Error: Se esperaba un '{' en la línea " << previous->text << endl;
            exit(1);
        }
        elseBody = parseStatementList();
        if (!match(Token::LLD)) {
            cout << "Error: Se esperaba un '}' en la línea " << previous->text << endl;
            exit(1);
        }
    }
    return new IfStmt(condition, ifBody, elseBody);
}

StepCondition* Parser::parseStepCondition(){
    if (!match(Token::ID)) {
        cout << "Error: Se esperaba un identificador en la línea " << previous->text << endl;
        exit(1);
    }
    string id = previous->text;
    if (!check(Token::PLUS_PLUS) && !check(Token::MINUS_MINUS)) {
        cout << "Error: Se esperaba un '++' o '--' en la línea " << previous->text << endl;
        exit(1);
    }
    return new StepCondition(id);
}



ForStmt* Parser::parseForStmt(){
    if (!match(Token::FOR)) {
        cout << "Error: Se esperaba un 'for' en la línea " << previous->text << endl;
        exit(1);
    }
    if (!check(Token::PI)) {
        cout << "Error: Se esperaba un '(' en la línea " << previous->text << endl;
        exit(1);
    }
    current = scanner->nextToken();
    if(!match(Token::INT) && !match(Token::LONG) && !match(Token::FLOAT) && !match(Token::DOUBLE) && !match(Token::CHAR)){
        cout << "Error: Se esperaba un tipo de dato en la línea " << previous->text << endl;
        exit(1);
    }
    Assignment* init = parseAssignment();
    
    CExp* condition = parseRelationalExp();
    if (!match(Token::SEMICOLON)) {
        cout << "Error: Se esperaba un ';' en la línea " << previous->text << endl;
        exit(1);
    }

    StepCondition* step = parseStepCondition();
    current = scanner->nextToken();
    if (!match(Token::PD)) {
        cout << "Error: Se esperaba un ')' en la línea " << previous->text << endl;
        exit(1);
    }
    if (!match(Token::LLI)) {
        cout << "Error: Se esperaba un '{' en la línea " << previous->text << endl;
        exit(1);
    }
    StatementList* body = parseStatementList();
    if (!match(Token::LLD)) {
        cout << "Error: Se esperaba un '}' en la línea " << previous->text << endl;
        exit(1);
    }
    return new ForStmt(init, condition, step, body);
}


StatementList* Parser::parseStatementList() {
    StatementList* stmtList = new StatementList();
    while (!check(Token::LLD) && !isAtEnd()) {
        stmtList->add(parseStatement());
    }
    return stmtList;
}


Stmt* Parser::parseStatement() {
    if (check(Token::IF)) {
        return parseIfStmt();
    } else if (check(Token::ID)) {
        return parseAssignment();
    } else if (check(Token::PRINTF)) {
        return parsePrintStmt();
    } else if (check(Token::RETURN)) {
        return parseReturnStmt();
    } else if (check(Token::FOR)) {
        return parseForStmt();
    } else {
        advance();
    }    return nullptr;
}


VarDec* Parser::parseVarDec() {

    if (!(match(Token::INT) || match(Token::LONG))) {
        if(check(Token::LLD)){
            cout <<"Parsing exitoso" << endl;
        }
        cout << "Error: Se esperaba un tipo de dato en la línea " << previous->text << endl;
        exit(1);
    }
    string type = previous->text;  

    if (!match(Token::ID)) {
        cout << "Error: Se esperaba un identificador en la línea " << current->text << endl;
    }
    string id = previous->text; 
    if (!match(Token::SEMICOLON)) {
        cout << "Error: Se esperaba un ';' en la línea " << previous->text << endl;
        exit(1);
    }
    return new VarDec(type, id); 
}


VarDecList* Parser::parseVarDecList() {
    VarDecList* varDecList = new VarDecList();
    while (check(Token::INT) || check(Token::LONG)) {
        varDecList->add(parseVarDec());
    }
    return varDecList;
}


Param* Parser::parseParam() {
    
    if (match(Token::INT) || match(Token::LONG) || match(Token::FLOAT) || match(Token::DOUBLE) || match(Token::CHAR)) {
    }
    string type = previous->text;
    if (!match(Token::ID)) {
        cout << "Error: Se esperaba un identificador en la línea " << previous->text << endl;
        exit(1);
    }
    string id = previous->text;
    return new Param(type, id);
}


ParamList* Parser::parseParamList() {
    ParamList* paramList = new ParamList();
    while (check(Token::INT) || check(Token::LONG) || check(Token::FLOAT) || check(Token::DOUBLE) || check(Token::CHAR)) {
        paramList->add(parseParam());
        if (!match(Token::COMMA)) {
            break;
        }
    }
    return paramList;
}


FuncDecl* Parser::parseFuncDecl() {

    if (!match(Token::INT) && !match(Token::LONG) && !match(Token::FLOAT) && !match(Token::DOUBLE) && !match(Token::CHAR) && !match(Token::VOID)) {
        cout << "Error: Se esperaba un tipo de dato en la línea " << previous->text << endl;
        exit(1);
    }
    string type = previous->text;

    if (!match(Token::ID)) {
        cout << "Error: Se esperaba un identificador para la función en la línea " << previous->text << endl;
        exit(1);
    }
    string name = previous->text;

    if (!match(Token::PI)) {
        cout << "Error: Se esperaba un '(' en la línea " << previous->text << endl;
        exit(1);
    }
    ParamList* params = parseParamList();
    for (Param* p : params->params) {
    }
    if(!match(Token::PD)){
        cout << "Error: Se esperaba un ')' en la línea " << previous->text << endl;
        exit(1);
    }

    if (!match(Token::LLI)) {
        cout << "Error: Se esperaba un '{' en la línea " << previous->text << endl;
        exit(1);
    }
    VarDecList* varDecs = parseVarDecList();
    StatementList* stmts = parseStatementList();
    ReturnStatement* ret = parseReturnStmt();
    if (!match(Token::LLD )) {
        cout << "Error: Se esperaba un '}' en la línea " << previous->text << endl;
        exit(1);
    }
    return new FuncDecl(type, name, params, varDecs, stmts, ret);
}


FuncList* Parser::parseFuncList() {

    FuncList* funcList = new FuncList();
    while (!isAtEnd()) {
        funcList->add(parseFuncDecl());
    }
    return funcList;
}

ArgList* Parser::parseArgList() {
    ArgList* argList = new ArgList();
    while (!check(Token::PD)) {
        CExp* arg = parseExp();
        argList->add(arg);
        if (!match(Token::COMMA)) {
            break;
        }
    }
    return argList;
}

FuncCallStmt* Parser::parseFuncCallStmt() {
    FuncCallExp* exp = parseFuncCallExp();
    if (!match(Token::SEMICOLON)) {
        cout << "Error: Se esperaba un ';' en la línea " << previous->text << endl;
        exit(1);
    }
    return new FuncCallStmt(exp);
}

FuncCallExp* Parser::parseFuncCallExp() {
    string id = previous->text;
    if (!match(Token::PI)) {
        cout << "Error: Se esperaba un '(' después de la función " << id << " en la línea " << previous->text << endl;
        exit(1);
    }

    ArgList* args = parseArgList();

    if (!match(Token::PD)) {
        cout << "Error: Se esperaba un ')' al final de la llamada a la función " << id << " en la línea " << previous->text << endl;
        exit(1);
    }

    return new FuncCallExp(id, args);
}

Program* Parser::parseProgram() {
    FuncList* funcs = parseFuncList();
    if (!isAtEnd()) {
        cout << "Error: Tokens adicionales después del final del programa" << endl;
        exit(1);
    }

    return new Program(funcs);
}


// Constructor de la clase Parser
Parser::Parser(Scanner* sc):scanner(sc) {
    previous = NULL;
    scanner->ignore();
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        cout << "Error en el primer token: " << current->text << endl;
        exit(1);
    }
}
