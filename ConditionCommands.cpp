
#include "ConditionCommands.h"


void IfCommand::doCommand() {
    int index = 1;
    Utils utils;
    Parser parser;
    list<string> condition;
    vector<string> blockCode;

    // condition until {
    while (this->code[index] != "{") {
        condition.push_back(this->code[index]);
        ++index;
    }

    // block code until the end
    index += 2;
    for (; index < this->code.size(); ++index) {
        blockCode.push_back(this->code[index]);
    }

    // execute expressions in block-code while condition is true
    if (utils.evaluate(condition, this->symTbl)) {
        parser.parse(blockCode, this->expressions, this->factory, this->cmdMap);
        this->expressions->executeAll();
    }
}

IfCommand::IfCommand(vector<string> &code, CommandFactory *factory, map<string, VarData *> *symTbl, map<string,string> &cmdMap) {
    // init members
    for (const auto &i : code) {
        this->code.push_back(i);
    }
    this->factory = factory;
    this->symTbl = symTbl;
    this->cmdMap = cmdMap;
    this->expressions = new ExpsCollection;
}

IfCommand::~IfCommand() {
    delete this->expressions;
}


void LoopCommand::doCommand() {
    int index = 1;
    Utils utils;
    Parser parser;
    list<string> condition;
    vector<string> blockCode;

    // condition until {
    while (this->code[index] != "{") {
        condition.push_back(this->code[index]);
        ++index;
    }

    // block code until the end
    index += 2;
    for (; index < this->code.size(); ++index) {
        blockCode.push_back(this->code[index]);
    }

    // execute expressions in block-code while condition is true
    while (utils.evaluate(condition, this->symTbl)) {
        this->expressions->deleteAll();
        parser.parse(blockCode, this->expressions, this->factory, this->cmdMap);
        this->expressions->executeAll();
    }
}

LoopCommand::LoopCommand(vector<string> &code, CommandFactory *factory, map<string, VarData *> *symTbl, map<string,string> &cmdMap) {
    // init members
    for (const auto &i : code) {
        this->code.push_back(i);
    }
    this->factory = factory;
    this->symTbl = symTbl;
    this->cmdMap = cmdMap;
    this->expressions = new ExpsCollection;
}

LoopCommand::~LoopCommand() {
    delete this->expressions;
}
