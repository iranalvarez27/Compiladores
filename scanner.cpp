#include <iostream>
#include <cstring>
#include "token.h"
#include "scanner.h"

using namespace std;

// Constructor del Scanner
Scanner::Scanner(const char* s) : input(s), first(0), current(0) {}

// Función para identificar espacios en blanco
bool is_white_space(char c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

void Scanner::ignore() {
    while (current < input.length()) {
        // Ignorar espacios en blanco
        if (is_white_space(input[current])) {
            current++;
            continue;
        }

        // Ignorar comentarios con "//"
        if (input[current] == '/' && current + 1 < input.length() && input[current + 1] == '/') {
            current += 2;
            while (current < input.length() && input[current] != '\n') {
                current++;
            }
            if (current < input.length() && input[current] == '\n') {
                current++;
            }
            continue;
        }

        // Ignorar comentarios con "#"
        if (input[current] == '#') {
            while (current < input.length() && input[current] != '\n') {
                current++; 
            }
            if (current < input.length() && input[current] == '\n') {
                current++;
            }
            continue;
        }

        break;
    }
}


// Generación del siguiente token
Token* Scanner::nextToken() {
    Token* token;
    ignore();

    // Ignorar espacios en blanco
    while (current < input.length() && is_white_space(input[current])) {
        current++;
    }

    if (current >= input.length()) {
        token = new Token(Token::END);
        return token;
    }

    char c = input[current];
    first = current;

    
    if (isdigit(c)) {
        current++;
        while (current < input.length() && isdigit(input[current]))
            current++;
        token = new Token(Token::NUM, input, first, current - first);
    }
    else if (isalpha(c)) {
        current++;
        while (current < input.length() && isalnum(input[current]))
            current++;
        string word = input.substr(first, current - first);

        if (word == "printf") token = new Token(Token::PRINTF, word, 0, word.length());
        else if (word == "if") token = new Token(Token::IF, word, 0, word.length());
        else if (word == "else") token = new Token(Token::ELSE, word, 0, word.length());
        else if (word == "for") token = new Token(Token::FOR, word, 0, word.length());
        else if (word == "int") token = new Token(Token::INT, word, 0, word.length());
        else if (word == "long") token = new Token(Token::LONG, word, 0, word.length());
        else if (word == "float") token = new Token(Token::FLOAT, word, 0, word.length());
        else if (word == "double") token = new Token(Token::DOUBLE, word, 0, word.length());
        else if (word == "char") token = new Token(Token::CHAR, word, 0, word.length());
        else if (word == "return") token = new Token(Token::RETURN, word, 0, word.length());
        else if (word == "void") token = new Token(Token::VOID, word, 0, word.length());
        else token = new Token(Token::ID, word, 0, word.length());
    }
    else if (c == '"') { 
        current++;
        while (current < input.length() && input[current] != '"')
            current++;
        if (current >= input.length()) {
            token = new Token(Token::ERR, c);
        } else {
            token = new Token(Token::STRING, input, first + 1, current - first - 1);
            current++;
        }
    }
    else if (strchr("+-*/()=;,<>!{}%", c)) {
        switch (c) {
            case '*': token = new Token(Token::MUL, c); break;
            case '/': token = new Token(Token::DIV, c); break;
            case ',': token = new Token(Token::COMMA, c); break;
            case '(': token = new Token(Token::PI, c); break;
            case ')': token = new Token(Token::PD, c); break;
            case '{': token = new Token(Token::LLI, c); break;
            case '}': token = new Token(Token::LLD, c); break;
            case '%': token = new Token(Token::PTJ, c); break;
            case '+': 
                if (current + 1 < input.length() && input[current + 1] == '+') {
                    token = new Token(Token::PLUS_PLUS, "++", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::PLUS, c);
                }
                break;
            case '-': 
                if (current + 1 < input.length() && input[current + 1] == '-') {
                    token = new Token(Token::MINUS_MINUS, "--", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::MINUS, c);
                }
                break;
            case '=':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::EQ, "==", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::ASSIGN, c);
                }
                break;
            case '<':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::LE, "<=", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::LT, c);
                }
                break;
            case '>':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::GE, ">=", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::GT, c);
                }
                break;
            case '!':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::NE, "!=", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::ERR, c); 
                }
                break;
            case ';': token = new Token(Token::SEMICOLON, c); break;
            default:
                cout <<"Error: Carácter inválido: " << c << endl;
                token = new Token(Token::ERR, c); 
        }
        current++;
    }
    else {
        token = new Token(Token::ERR, c); // Error
        current++;
    }
    return token;
}

void Scanner::reset() {
    first = 0;
    current = 0;
}

Scanner::~Scanner() {}

void test_scanner(Scanner* scanner) {
    Token* current;

    cout << "-------------------" << endl;
    cout << "Iniciando Scanner:" << endl;
    do {
        scanner->ignore();
        current = scanner->nextToken();
        cout << *current << endl;
    } while (current->type != Token::END);
    cout << "Fin del escaneo." << endl;
    cout << "----------------" << endl;

}
