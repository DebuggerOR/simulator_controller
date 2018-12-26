

#include "CommandFactory.h"
#include "ConditionCommands.h"
#include "DefClientPlaceServerCommands.h"
#include "EntrcPrintSleepCommands.h"

CommandFactory::CommandFactory(map<string, VarData *> *symTbl, map<string, string> &cmpMap, Threads* threads) {
    // init members
    this->symTbl = symTbl;
    this->cmdMap = cmpMap;
    this->threads = threads;
}

Command *CommandFactory::createCommand(vector<string> &code) {
    // index at code string arr
    int index = 0;

    // create open server command
    if (code[index] == "openDataServer") {
        return new OpenServerCommand(code, this->symTbl, this->threads);

        // create client connect command
    } else if (code[index] == "connect") {
        return new ConnectCommand(code, this->symTbl, this->threads);

        // create define var command
    } else if (code[index] == "var") {
        return new DefineVarCommand(code, this->symTbl);

        // create loop command
    } else if (code[index] == "while") {
        return new LoopCommand(code, this, this->symTbl, this->cmdMap);

        // create if command
    } else if (code[index] == "if") {
        return new IfCommand(code, this, this->symTbl, this->cmdMap);

        // create sleep command
    } else if (code[index] == "sleep") {
        return new SleepCommand(code, this->symTbl);

        // create print command
    } else if (code[index] == "print") {
        return new PrintCommand(code, this->symTbl);

        // create print command
    } else if (code[index] == "entrc") {
        return new EntrcCommand();

        // create placement command
    } else if (code[index+1] == "=") {
        return new PlacementCommand(code, this->symTbl);
    }

    return nullptr;
}

