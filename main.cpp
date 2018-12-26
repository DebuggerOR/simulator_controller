#include <iostream>
#include <cstring>
#include <list>
#include <fstream>
#include <sstream>
#include "Interpreter.h"
#include "ReadCode.h"

using namespace std;


int main(int argc, char** argv) {
    try {
        ReadCode readCode;

        // if there is arg, read from file
        if (argc > 1) {
            readCode.readFromFile(argv[1]);
            // else, read from console
        } else {
            readCode.readFromConsole();
        }
        cout << "\nbye bye :)\n" << endl;

    } catch (...) {
        cout << "\nbe careful!!\n" << endl;
    }

    return 0;
}
