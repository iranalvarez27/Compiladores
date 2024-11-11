#include "parser.h"

using namespace std;

Parser::Parser(Scanner* scanner) : scanner(scanner), current(nullptr), previous(nullptr) {}

// Avanza al siguiente token
bool Parser::advance() {
    previous = current;
    current = scanner->nextToken();
    return current != nullptr;
}

// Verifica si el token actual es del tipo especificado
bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

// Comprueba el tipo del token actual
bool Parser::check(Token::Type ttype) {
    return current && current->type == ttype;
}

// Verifica si se ha alcanzado el final de la entrada
bool Parser::isAtEnd() {
    return current == nullptr;
}

// Métodos del parser para cada regla de la gramática

void Parser::parse() {
    advance();
    parseProgram();
}

void Parser::parseProgram() {
    parseStmtList();
}

void Parser::parseStmtList() {
    while (!isAtEnd()) {
        parseStmt();
        if (match(Token::SEMICOLON)) {
            continue;
        } else {
            cout << "Error: se esperaba ';'." << endl;
            exit(1);
        }
    }
}

void Parser::parseStmt() {
    if (check(Token::INT) || check(Token::FLOAT) || check(Token::CHAR) || check(Token::VOID)) {
        parseVarDecl();
    } else if (check(Token::ID)) {
        if (match(Token::PI)) {
            parseFuncCall();
        } else if (match(Token::ASSIGN)) {
            parseAssignment();
        }
    } else if (match(Token::IF)) {
        parseIfStmt();
    } else if (match(Token::FOR)) {
        parseForStmt();
    } else if (match(Token::PRINTF)) {
        parsePrint();
    } else {
        cout << "Error: declaración inválida." << endl;
        exit(1);
    }
}

void Parser::parseVarDecl() {
    parseType();
    if (match(Token::ID)) {
        return;
    } else {
        cout << "Error: se esperaba un identificador." << endl;
        exit(1);
    }
}

void Parser::parseAssignment() {
    if (match(Token::ID)) {
        if (match(Token::ASSIGN)) {
            parseAExp();
        } else {
            cout << "Error: se esperaba '='." << endl;
            exit(1);
        }
    }
}

void Parser::parseIfStmt() {
    if (match(Token::PI)) {
        parseCExp();
        if (match(Token::PD)) {
            if (match(Token::LLI)) {
                parseStmtList();
                if (!match(Token::LLD)) {
                    cout << "Error: se esperaba '}'." << endl;
                    exit(1);
                }
                if (match(Token::ELSE)) {
                    if (match(Token::LLI)) {
                        parseStmtList();
                        if (!match(Token::LLD)) {
                            cout << "Error: se esperaba '}' en 'else'." << endl;
                            exit(1);
                        }
                    }
                }
            }
        }
    }
}

void Parser::parseForStmt() {
    if (match(Token::PI)) {
        parseVarDecl();
        if (match(Token::SEMICOLON)) {
            parseCExp();
            if (match(Token::SEMICOLON)) {
                parseAssignment();
                if (match(Token::PD)) {
                    if (match(Token::LLI)) {
                        parseStmtList();
                        if (!match(Token::LLD)) {
                            cout << "Error: se esperaba '}'." << endl;
                            exit(1);
                        }
                    }
                }
            }
        }
    }
}

void Parser::parseFuncCall() {
    if (match(Token::PI)) {
        if (!match(Token::PD)) {
            parseAExp();
            while (match(Token::COMMA)) {
                parseAExp();
            }
            if (!match(Token::PD)) {
                cout << "Error: se esperaba ')'." << endl;
                exit(1);
            }
        }
    }
}

void Parser::parseFuncDecl() {
    parseType();
    if (match(Token::ID)) {
        if (match(Token::PI)) {
            if (!match(Token::PD)) {
                parseVarDecl();
                while (match(Token::COMMA)) {
                    parseVarDecl();
                }
                if (!match(Token::PD)) {
                    cout << "Error: se esperaba ')'." << endl;
                    exit(1);
                }
            }
            if (match(Token::LLI)) {
                parseStmtList();
                if (!match(Token::LLD)) {
                    cout << "Error: se esperaba '}'." << endl;
                    exit(1);
                }
            }
        }
    }
}

void Parser::parsePrint() {
    if (match(Token::PI)) {
        if (match(Token::STRING)) {
            while (match(Token::COMMA)) {
                parseAExp();
            }
            if (!match(Token::PD)) {
                cout << "Error: se esperaba ')'" << endl;
                exit(1);
            }
        }
    }
}

void Parser::parseType() {
    if (match(Token::INT) || match(Token::FLOAT) || match(Token::CHAR) || match(Token::VOID)) {
        return;
    } else {
        cout << "Error: tipo inválido." << endl;
        exit(1);
    }
}

void Parser::parseCExp() {
    parseAExp();
    if (match(Token::LT) || match(Token::LE) || match(Token::EQ) || match(Token::GE) || match(Token::GT) || match(Token::NE)) {
        parseAExp();
    } else {
        cout << "Error: expresión condicional inválida." << endl;
        exit(1);
    }
}

void Parser::parseAExp() {
    parseTerm();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        parseTerm();
    }
}

void Parser::parseTerm() {
    parseFactor();
    while (match(Token::MUL) || match(Token::DIV)) {
        parseFactor();
    }
}

void Parser::parseFactor() {
    if (match(Token::ID) || match(Token::NUM)) {
        return;
    } else if (match(Token::PI)) {
        parseAExp();
        if (!match(Token::PD)) {
            cout << "Error: se esperaba ')'" << endl;
            exit(1);
        }
    } else {
        cout << "Error: factor inválido." << endl;
        exit(1);
    }
}
