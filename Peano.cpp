#include <iostream>
#include "File.h"
#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"
using namespace std;

int main(int argc, char* argv[]) {
    File file;
    try { 
        cout << "Retrieving...\n";
        file.load(argc, argv); 
    }
    catch (File_Error e) {
        cout << e.what << endl;
        return -1;
    }

    Lexer lexer;
    try {
        cout << "Lexing...\n";
        lexer.load(file);
    }
    catch (Lexer_Error e) {
        cout << e.what << endl;
        return -1;
    }

    Parser parser;
    try {
        cout << "Parsing...\n";
        parser.load(lexer);
    }
    catch (Parser_Error e) {
        cout << e.what << endl;
        return -1;
    }

    Interpreter interpreter;
    try {
        cout << "Interpreting...\n";
        interpreter.load(parser);
    }
    catch (Interpreter_Error e) {
        cout << e.what << endl;
        return -1;
    }

    Object object;
    try {
        cout << "Writing...\n";
        object.load(interpreter.file.get(), argc, argv);
    }
    catch (File_Error e) {
        cout << e.what << endl;
        return -1;
    }

    cout << "Compilation successful!\n";
    return 0;
}








