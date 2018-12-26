

#ifndef COMMANDFACTORY_H
#define COMMANDFACTORY_H

#include <map>
#include <list>
#include <iostream>
#include "VarData.h"
#include "ExpsCollection.h"
#include "Threads.h"
#include "Command.h"

class CommandFactory {
    map<string, VarData *> *symTbl;
    map<string, string> cmdMap;
    Threads *threads;

public:
    CommandFactory(map<string, VarData *> *symTbl, map<string, string> &cmpMap, Threads *threads);

    Command *createCommand(vector<string> &code);
};


#endif
