#include <iostream>
#include <fstream>
#include <string>
#include "scanner.h"
#include "parser.h" 
#include "exp.h"

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

    delete parser;
    delete scanner;

    return 0;
}
