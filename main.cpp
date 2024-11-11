#include <iostream>
#include <fstream>
#include <string>
#include "scanner.h"
#include "parser.h"


using namespace std;


int main() {
    ifstream inputFile("input1.txt"); 
    if (!inputFile) {
        cerr << "Error al abrir el archivo input1.txt" << endl;
        return 1;  
    }

    string inputContent((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close(); 

    Scanner* scanner = new Scanner(inputContent.c_str());
    test_scanner(scanner);

    Parser* parser = new Parser(scanner);
    if(parser->parse()) {
        cout << "Análisis sintáctico exitoso" << endl;
    } else {
        cout << "Error en el análisis sintáctico" << endl;
    }


    delete scanner;
    delete parser;

    return 0;
}
