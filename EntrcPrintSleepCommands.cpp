

#include "EntrcPrintSleepCommands.h"


void EntrcCommand::doCommand() {
    cout << "enter a char.." << endl;
    // wait until get char
    getchar();
}

void PrintCommand::doCommand() {
    Utils utils;

    // case print "dovi" display dovi
    if ((*this->strings.begin())[0] == '"') {
        cout << utils.removeApostrophes(*strings.begin()) << endl;
        // case print expression display expression's value
    } else {
        cout << utils.evaluate(this->strings, this->symTbl) << endl;
    }
}

PrintCommand::PrintCommand(vector<string>& code, map<string, VarData *> *symTbl) {
    int index = 1;
    this->symTbl = symTbl;

    // print strings until end
    while (index < code.size() && code[index] != ";") {
        this->strings.push_back(code[index]);
        index++;
    }
}

void SleepCommand::doCommand() {
    // sleep for x msc
    usleep(static_cast<__useconds_t>(this->sleepTime));
}

SleepCommand::SleepCommand(vector<string>& code, map<string, VarData *> *symTbl) {
    int index = 1;
    Utils utils;
    list<string> sleepList;

    // sleep expression until end
    while (index < code.size() && code[index] != ";") {
        sleepList.push_back(code[index]);
        index++;
    }
    this->sleepTime = static_cast<int>(utils.evaluate(sleepList, symTbl));
}
