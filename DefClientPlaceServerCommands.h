

#ifndef CLIENTSERVERCOMMANDS_H
#define CLIENTSERVERCOMMANDS_H


#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <list>
#include <mutex>
#include "Utils.h"
#include "Command.h"
#include "Threads.h"
#include "VarData.h"

class ConnectCommand: public Command {
    //ipv6 max 39 chars
    char ip[39];
    double port;
    Threads *threads;
    map<string, VarData *> *symTbl;

public:
    ConnectCommand(vector <string> &code, map<string, VarData *> *symTbl, Threads* threads);

    void doCommand() override;

    ~ConnectCommand() override;
};

class DefineVarCommand : public Command {

public:
    explicit DefineVarCommand(vector<string> &code, map<string, VarData *> *symTbl);

    void doCommand() override;
};

class OpenServerCommand : public Command {
    int port;
    int rate;
    map<string, VarData *> *symTbl;
    Threads *threads;

public:
    OpenServerCommand(vector<string> &code, map<string, VarData *> *symTbl, Threads *threads);

    void doCommand() override;

    ~OpenServerCommand() override;
};

class PlacementCommand : public Command {
    map<string, VarData *> *symTbl;
    string left;
    list<string> right;

public:
    explicit PlacementCommand(vector<string> &code, map<string, VarData *> *symTbl);

    void doCommand() override;
};


#endif
