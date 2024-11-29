#include <iostream>
#include <fstream>
#include <string>
#include "scanner.h"
#include "parser.h"
#include "visitor.h"
#include "imp_interpreter.hh"
#include "imp_type.hh"
#include "imp_type_checker.hh"
#include "imp_codegen.hh"

using namespace std;

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        cout << "Numero incorrecto de argumentos. Uso: " << argv[0] << " <archivo_de_entrada>" << endl;
        exit(1);
    }

    ifstream infile(argv[1]);
    if (!infile.is_open()) {
        cout << "No se pudo abrir el archivo: " << argv[1] << endl;
        exit(1);
    }

    string input;
    string line;
    while (getline(infile, line)) {
        input += line + '\n';
    }
    infile.close();

    // Crear y probar el scanner
    Scanner scanner(input.c_str());
    string input_copy = input;
    Scanner scanner_test(input_copy.c_str());
    test_scanner(&scanner_test);  // Esto es para verificar los tokens generados por el scanner
    cout << "Scanner exitoso" << endl;
    cout << endl;

    // Iniciar el parsing
    cout << "Iniciando parsing:" << endl;
    Parser parser(&scanner); 
    try {
        Program* program = parser.parseProgram();  // Parseo del programa
        cout << "Parsing exitoso" << endl << endl;
        ImpInterpreter interpreter;
        ImpTypeChecker type_checker;
        ImpCodeGen codegen(&type_checker);

        cout << "TypeChecker:" << endl;
        type_checker.typecheck(program);
        
        cout<<endl<<"generar codigo:"<<endl;
        string filename = argv[1];
        codegen.codegen(program, filename + ".sm");
        cout << "End of program execution" << endl;
        
        delete program;
    }
    catch (const exception& e) {
        cout << "Error durante el parsing: " << e.what() << endl;
        return 1;
    }

    return 0;
}
