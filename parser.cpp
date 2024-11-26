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
            exit(1);
        }
        return true;
    }
    return false;
}


bool Parser::isAtEnd() {
    return (current->type == Token::END);
}


/*----------------------------------------------------------------------------------------------------------------------------------------*/


//Factor ::= id | Num | '(' CExp ')'
CExp* Parser::parseFactor() {
    if (check(Token::ID)) {
        advance();
        if(match(Token::PLUS) || match(Token::MINUS)){
            CExp* exp = parseCExp();
            return exp;
        }
        else if(check(Token::PI)){
            current = previous;
            FuncCallExp* exp = parseFuncCallExp();
            return exp;
        } 
        else if(check(Token::COMMA)){
            current = previous;
            ArgList* args = parseArgList();
            
        }
        return new IdentifierExp(current->text);
    } else if (match(Token::NUM)) {
        return new NumberExp(stoi(previous->text));
    } else if (check(Token::PI) || check(Token::PD) ) {
        cout << "Factor es una expresión entre paréntesis: (" << endl;
        CExp* exp = parseExp();
        if (!match(Token::PD)) {
            cout << "Error: Se esperaba un ')' en la línea " << previous->text << endl;
            exit(1);
        }
        return exp;
    }
        else{
        cout << "Error: Se esperaba un factor en la línea " << current->text << endl;
        exit(1);
    }
    return nullptr;
}

// Parseando CExp ::= Exp [ RelOp Exp ] | id '+' id ';'

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


// Parseando un término: Factor ::= id | Num | '(' CExp ')'

CExp* Parser::parseTerm() {
    CExp* left = parseFactor();
    while (match(Token::MUL) || match(Token::DIV)) {
        Token* op = previous;
        CExp* right = parseFactor();
        left = new BinaryExp(left, right, (op->type == Token::MUL) ? BinaryOp::MUL_OP : BinaryOp::DIV_OP);
    }
    return left;
}

// Parseando una expresión: Exp ::= Term (( '+' | '-' ) Term)*

CExp* Parser::parseExp() {
    CExp* left = parseTerm();
    while (check(Token::PLUS) || check(Token::MINUS)) {
        Token* op = previous;
        CExp* right = parseTerm();
        left = new BinaryExp(left, right, (op->type == Token::PLUS) ? BinaryOp::PLUS_OP : BinaryOp::MINUS_OP);
    }
    return left;
}

// Parseando CExp ::= Exp [ RelOp Exp ]
RelationalExp* Parser::parseRelationalExp() {
    CExp* left = parseExp();    
    if (check(Token::LT) || check(Token::LE) || check(Token::EQ) || check(Token::GT) || check(Token::GE) || check(Token::NE)) {
        Token* op = previous;
        advance();
        CExp* right = parseExp();
        return new RelationalExp(left, right, (op->type == Token::LT) ? RelOp::LT_OP : (op->type == Token::LE) ? RelOp::LE_OP : (op->type == Token::EQ) ? RelOp::EQ_OP : (op->type == Token::GT) ? RelOp::GT_OP : (op->type == Token::GE) ? RelOp::GE_OP : RelOp::NE_OP);
    }
    return nullptr;
}

// Parseando una asignación: Assignment ::= id '=' CExp ';' Assignment ::= id '=' CExp ';' | id '+' id ';' | id '-' id ';'


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

// Parseando una sentencia de impresión: PrintStmt ::= 'printf' '(' STRING ',' CExp ')' ';'

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
    current = scanner->nextToken();
    // cout << "Siguiente " << current->text << endl;
    // CExp* exp = parseExp();
    if(check(Token::ID))
        if(match(Token::PI)){
            current = previous;
            FuncCallExp* exp = parseFuncCallExp();
            if(!match(Token::PD)){
                cout << "Error: Se esperaba un ')' en la línea " << previous->text << endl;
                exit(1);
            }
            if(!match(Token::SEMICOLON)){
                cout << "Error: Se esperaba un ';' en la línea " << previous->text << endl;
                exit(1); 
            }
        else{
            CExp* exp = parseExp();
            if(!match(Token::PD)){
                cout << "Error: Se esperaba un ')' en la línea " << previous->text << endl;
                exit(1);
            }
            if(!match(Token::SEMICOLON)){
                cout << "Error: Se esperaba un ';' en la línea " << previous->text << endl;
                exit(1); 
                }        
            }
        return new PrintStmt(format, exp);
}
return nullptr;
}
// Parseando una sentencia de retorno: ReturnStmt ::= 'return' CExp ';'
ReturnStatement* Parser::parseReturnStmt() {
    if (!check(Token::RETURN)) {
        if(check(Token::LLD)){
            //Terminar la ejecución
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

// Parseando una sentencia if-else: IfStmt ::= 'if' '(' CExp ')' '{' StmtList '}' [ 'else' '{' StmtList '}' ]

IfStmt* Parser::parseIfStmt(){
    //Reemplacar current por previous
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
    cout << "Current--: " << current->text << endl;
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

//Parseando un StepCondition: StepCondition ::= id '++' | id '--'
StepCondition* Parser::parseStepCondition(){
    if (!match(Token::ID)) {
        cout << "Error: Se esperaba un identificador en la línea " << previous->text << endl;
        exit(1);
    }
    string id = previous->text;
    if (!check(Token::PLUS_PLUS) && !check(Token::MINUS_MINUS)) {
        exit(1);
    }
    return new StepCondition(id);
}

// Parseando una sentencia for: ForStmt ::= 'for' '(' Assignment CExp ';' Assignment ')' '{' StmtList '}'

ForStmt* Parser::parseForStmt(){
    if (!match(Token::FOR)) {
        cout << "Error: Se esperaba un 'for' en la línea " << previous->text << endl;
        exit(1);
    }
    if (!check(Token::PI)) {
        cout << "Error: Se esperaba un '(' en la línea " << previous->text << endl;
        exit(1);
    }
    cout << "Current: " << current->text << endl;
    current = scanner->nextToken();
    if(!match(Token::INT) && !match(Token::LONG) && !match(Token::FLOAT) && !match(Token::DOUBLE) && !match(Token::CHAR)){
        cout << "Error: Se esperaba un tipo de dato en la línea " << previous->text << endl;
        exit(1);
    }
    Assignment* init = parseAssignment();
    //Parsear una expresión relacional
    
    CExp* condition = parseRelationalExp();
    if (!match(Token::SEMICOLON)) {
        cout << "Error: Se esperaba un ';' en la línea " << previous->text << endl;
        exit(1);
    }
    //current = scanner->nextToken();
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

// Parseando una lista de sentencias: StmtList ::= Stmt StmtList | ε

StatementList* Parser::parseStatementList() {
    StatementList* stmtList = new StatementList();
    while (!check(Token::LLD) && !isAtEnd()) {
        stmtList->add(parseStatement());
    }
    return stmtList;
}

// Parseando una sentencia: Stmt ::= Assignment | PrintStmt | ReturnStmt | IfStmt | ForStmt

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


// Parseando una declaración de variable: VarDec ::= Type id ';'
VarDec* Parser::parseVarDec() {
    // cout << "Parseando una declaración de variable..." << endl;
    // cout << "Current2: " << current->text << endl; //x
    // cout << "Previous: " << previous->text << endl; //int}

    // Verificar si el token actual es un tipo de dato
    if (!(match(Token::INT) || match(Token::LONG))) {
        if(check(Token::LLD)){
            cout <<"Parsing exitoso" << endl;
        }
        cout << "Error: Se esperaba un tipo de dato en la línea " << previous->text << endl;
        exit(1);
    }
    // cout << "Antes: " << current->text << endl; //int
    //Swappear current por previous sin perder los valores 

    // cout << "Después: " << current->text << endl; //int
    string type = previous->text;  // Guardamos el tipo de dato
    // cout << "Type: " << type << endl;

    if (!match(Token::ID)) {
        // cout <<current->text << endl;
        cout << "Error: Se esperaba un identificador en la línea " << current->text << endl;
    }
    // cout <<"Hola";
    string id = previous->text;  // Guardamos el identificador
    // cout <<"ID: " << id << endl;
    // cout <<"Type " << type << endl;
    if (!match(Token::SEMICOLON)) {
        cout << "Error: Se esperaba un ';' en la línea " << previous->text << endl;
        exit(1);
    }
    return new VarDec(type, id);  // Crear y devolver el nodo de la declaración de variable
}


// Parseando una lista de declaraciones de variables: VarDecList ::= VarDec VarDecList | ε
VarDecList* Parser::parseVarDecList() {
    VarDecList* varDecList = new VarDecList();
    while (check(Token::INT) || check(Token::LONG)) {
        varDecList->add(parseVarDec());
    }
    return varDecList;
}

// Parseando un parámetro: Param ::= Type id

Param* Parser::parseParam() {
    
    if (match(Token::INT) || match(Token::LONG) || match(Token::FLOAT) || match(Token::DOUBLE) || match(Token::CHAR)) {
        cout << "Parseando un parámetro..." << endl;
    }
    string type = previous->text;
    if (!match(Token::ID)) {
        cout << "Error: Se esperaba un identificador en la línea " << previous->text << endl;
        exit(1);
    }
    string id = previous->text;
    return new Param(type, id);
}

// Parseando una lista de parámetros: ParamList ::= Param (',' Param)* | ε

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

// Parseando una declaración de función: FuncDecl ::= Type id '(' ParamList ')' '{' VarDecList StmtList ReturnStmt '}'

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
        cout << "Tipo: " << p->type << ", ID: " << p->name << endl;
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

// Parseando una lista de funciones con mensajes: FuncList ::= FuncDecl FuncList | ε

FuncList* Parser::parseFuncList() {

    FuncList* funcList = new FuncList();
    while (!isAtEnd()) {
        funcList->add(parseFuncDecl());
    }
    return funcList;
}

//Quiero parsear algo asi: suma(x, y) donde ArgsList es x, y 
// Parseando una lista de argumentos: ArgList ::= CExp (',' CExp)* | ε | 

ArgList* Parser::parseArgList() {
    ArgList* argList = new ArgList();
    while (check(Token::ID) || check(Token::NUM) || check(Token::PI)) {
        CExp* arg = parseExp();
        argList->add(arg);
        if (!match(Token::COMMA)) {
            break;
        }
    }
    return argList;
}

// Parseando una  FuncCallStmt ::= FuncCallExp ';'
FuncCallStmt* Parser::parseFuncCallStmt() {
    FuncCallExp* exp = parseFuncCallExp();
    if (!match(Token::SEMICOLON)) {
        cout << "Error: Se esperaba un ';' en la línea " << previous->text << endl;
        exit(1);
    }
    return new FuncCallStmt(exp);
}

// Parseando una FuncCallExp ::= ID '(' ArgList ')'
FuncCallExp* Parser::parseFuncCallExp() {
    //Cambiar los valores de current con el token anterior

    // if (!match(Token::ID)) {
    //     cout << "Error: Se esperaba un identificador en la línea " << previous->text << endl;
    //     exit(1);
    // }
    string id = previous->text;
    if (!match(Token::PI)) {
        cout << "Error: Se esperaba un '(' en la línea " << previous->text << endl;
        exit(1);
    }
    ArgList* args = parseArgList();
    if (!match(Token::PD)) {
        cout << "Error: Se esperaba un ')' en la línea " << previous->text << endl;
        exit(1);
    }
    return new FuncCallExp(id, args);
}

// Parseando un programa: Program ::= FuncList

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
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        cout << "Error en el primer token: " << current->text << endl;
        exit(1);
    }
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/
