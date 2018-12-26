

#ifndef CONDITIONCOMMANDS_H
#define CONDITIONCOMMANDS_H


#include "ConditionCommand.h"
#include "ExpsCollection.h"
#include "CommandFactory.h"
#include "Utils.h"
#include "LexerParser.h"

class IfCommand : public ConditionCommand {
    ExpsCollection *expressions;
    vector<string> code;
    CommandFactory *factory;
    map<string, VarData *> *symTbl;
    map<string, string> cmdMap;

public:
    void doCommand() override;

    IfCommand(vector<string> &code, CommandFactory *factory, map<string, VarData *> *symTbl,
              map<string, string> &cmdMap);

    ~IfCommand();
};


class LoopCommand : public ConditionCommand {
    ExpsCollection *expressions;
    vector<string> code;
    CommandFactory *factory;
    map<string, string> cmdMap;
    map<string, VarData *> *symTbl;

public:
    explicit LoopCommand(vector<string> &code, CommandFactory *factory,
                         map<string, VarData *> *symTbl, map<string, string> &cmdMap);

    void doCommand() override;

    ~LoopCommand();
};

#endif