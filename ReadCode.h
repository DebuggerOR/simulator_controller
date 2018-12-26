

#ifndef READCODE_H
#define READCODE_H

#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>

#include "Interpreter.h"

using namespace std;


class ReadCode {

public:
    void readFromFile(string fileName);

    void readFromConsole();
};

#endif
