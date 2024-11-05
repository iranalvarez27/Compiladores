#include <iostream>
#include <cstring>
#include "token.h"
#include "scanner.h"

using namespace std;

Scanner::Scanner(const char* s):input(s), first(0), current(0) { }

bool is_white_space(char c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

Token* Scanner::nextToken() {
    Token* token;
    while (current < input.length() && is_white_space(input[current])) current++;
    // Fin de la cadena, se cambió el END por nullptr porque C no tiene ese tipo de dato
    if (current >= input.length()) return nullptr; 
    
    char c = input[current];
    first = current;

    //Esto ignora las liberias que se incluyan en el archivo de entrada como #include<stdio.h>
    if (c == '#') {
    while (current < input.length() && input[current] != '\n') {
        current++;
    }
    return nextToken(); 
    }
    
    // Esto ignora comentarios de línea que comienzan con //
    if (c == '/' && current + 1 < input.length() && input[current + 1] == '/') {
        current += 2; // Avanzar dos caracteres para omitir //
        while (current < input.length() && input[current] != '\n') {
            current++;
        }
        return nextToken(); 
    }

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
    else if (c == '"') {  // Maneja cadenas de texto de tipo printf("%d")
        current++;
        while (current < input.length() && input[current] != '"')
            current++;
        if (current < input.length()) { 
            current++; // Para incluir la comilla de cierre
            token = new Token(Token::STRING, input, first + 1, current - first - 2); // -2 para excluir las comillas
        } else {
            token = new Token(Token::ERR, c); // Error si no se encuentra el cierre
        }
    }

    else if (strchr("+-*/()=;,<>!{}!", c)) {
        switch(c) {
            case '+': token = new Token(Token::PLUS, c); break;
            case '-': token = new Token(Token::MINUS, c); break;
            case '*': token = new Token(Token::MUL, c); break;
            case '/': token = new Token(Token::DIV, c); break;
            case ',': token = new Token(Token::COMMA, c); break;
            case '(': token = new Token(Token::PI, c); break;
            case ')': token = new Token(Token::PD, c); break;
            case '{': token = new Token(Token::LLI, c); break;
            case '}': token = new Token(Token::LLD, c); break;
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
                    token = new Token(Token::ERR, c); // '!' solo no es un token válido
                }
                break;
            case ';': token = new Token(Token::SEMICOLON, c); break;
            default:
                cout << "No debería llegar acá" << endl;
                token = new Token(Token::ERR, c);
        }
        current++;
    }
    else {
        token = new Token(Token::ERR, c);
        current++;
    }
    return token;
}

void Scanner::reset() {
    first = 0;
    current = 0;
}

Scanner::~Scanner() { }


void test_scanner(Scanner* scanner) {
    Token* current;
    cout << "Iniciando Scanner:" << endl << endl;

    while ((current = scanner->nextToken()) != nullptr) {  // Cambiar a nullptr porque C no tiene END
        if (current->type == Token::ERR) {
            cout << "Error en scanner - carácter inválido: " << current->text << endl;
            delete current;
            break;
        } else {
            cout << *current << endl;
        }
        delete current;
    }
    // Mensaje de finalización de entrada, se cambió el END por nullptr porque C no tiene ese tipo de dato
    cout << "Fin de la entrada" << endl;  
}