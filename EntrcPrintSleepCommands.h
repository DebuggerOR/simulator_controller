

#ifndef DEFINEPRINTSLEEPCMDS_H
#define DEFINEPRINTSLEEPCMDS_H

#include <string>
#include <list>
#include <map>
#include <vector>
#include <unistd.h>
#include "Command.h"
#include "VarData.h"
#include "Utils.h"

using namespace std;


class EntrcCommand : public Command {
public:
    void doCommand() override;
};


class PrintCommand : public Command {
    list <string> strings;
    map<string, VarData *> *symTbl;

public:
    PrintCommand(vector<string> &code, map<string, VarData *> *symTbl);

    void doCommand() override;
};


class SleepCommand : public Command {
    int sleepTime;

public:
    void doCommand() override;

    SleepCommand(vector <string> &code, map<string, VarData *> *symTbl);
};


#endif
