#include <iostream>
#include <fstream>
#include "scanner.h"

using namespace std;

int main() {
    ifstream inputFile("input1.txt");
    if (!inputFile) {
        cerr << "Error al abrir el archivo inputs.txt" << endl;
        return 1; 
    }

    string inputContent((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());

    inputFile.close();

    Scanner* scanner = new Scanner(inputContent.c_str());

    test_scanner(scanner);

    delete scanner;

    return 0;
}
