

#include "ReadCode.h"

void ReadCode::readFromFile(string fileName) {
    Interpreter interpreter;
    ifstream infile;

    // open file
    infile.open (fileName);

    // put content in string
    string input(dynamic_cast<stringstream const&>(stringstream() << infile.rdbuf()).str());

    // close file
    infile.close();

    // use lexer and interpreter
    interpreter.parser(interpreter.lexer(input));
}

void ReadCode::readFromConsole() {
    Interpreter interpreter;
    string input;
    string line;

    cout << "type your code (to finish type 'exit')" << endl;

    while (true) {
        // get line each time
        getline(cin, line);

        // read until type 'quit'
        if (line == "exit") { break; }

        // add '\n' after each line
        input += line + "\n";
    }

    // use lexer and interpreter
    interpreter.parser(interpreter.lexer(input));
}
