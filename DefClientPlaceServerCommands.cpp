

#include "DefClientPlaceServerCommands.h"

// client details
bool isClientSend = false;
string clientPath;
int clientValue;

// mutex
mutex mtxMap;
mutex mtxClient;

bool programRunning = true;


void Connect(int port, char* ip) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }
    server = gethostbyname(ip);

    if (server == nullptr) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        exit(1);
    }

    while (programRunning) {
        if (isClientSend)
            isClientSend = false;
        bzero(buffer, 256);
        string msg;
        // syntax: "set /instrumentation/attitude-indicator/internal-pitch-deg 10000\r\n"
        msg = "set " + clientPath + " " + to_string(clientValue) + "\r\n";
        for (int i = 0; i < msg.size(); i++) {
            buffer[i] = msg[i];
        }
        n = write(sockfd, buffer, msg.size());

        if (n < 0) {
            perror("ERROR writing to socket");
            exit(1);
        }
        memset(buffer, 0, 256);
    }
}

void ConnectCommand::doCommand() {
    this->threads->addThread(new thread(Connect, this->port, this->ip));
}

ConnectCommand::ConnectCommand(vector <string> &code, map<string, VarData *> *symTbl, Threads* threads) {
    Utils utils;
    int index = 1;

    // ip
    memset(this->ip, 0, 39);
    for (int i = 0; i < code[index].size(); i++) {
        this->ip[i] = code[index][i];
    }
    ++index;

    // skip ,
    if(code[index]==",") {
        ++index;
    }

    // port expression
    list<string> portList;
    portList.push_back(code[index]);
    index++;
    while (index < code.size() && code[index] != ";") {
        portList.push_back(code[index]);
        index++;
    }
    this->port = utils.evaluate(portList, symTbl);

    // threads collection
    this->threads = threads;
}

ConnectCommand::~ConnectCommand() {
    programRunning = false;
}

void OpenServer(map<string, VarData*> *symTbl, int port, int rate) {
    int sockfd, newsockfd, clilen;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[256];
    vector<string> pathes = {"/instrumentation/airspeed-indicator/indicated-speed-kt",
                            "/instrumentation/altimeter/indicated-altitude-ft",
                            "/instrumentation/altimeter/pressure-alt-ft",
                            "/instrumentation/attitude-indicator/indicated-pitch-deg",
                            "/instrumentation/attitude-indicator/indicated-roll-deg",
                            "/instrumentation/attitude-indicator/internal-pitch-deg",
                            "/instrumentation/attitude-indicator/internal-roll-deg",
                            "/instrumentation/encoder/indicated-altitude-ft",
                            "/instrumentation/encoder/pressure-alt-ft",
                            "/instrumentation/gps/indicated-altitude-ft",
                            "/instrumentation/gps/indicated-ground-speed-kt",
                            "/instrumentation/gps/indicated-vertical-speed",
                            "/instrumentation/heading-indicator/indicated-heading-deg",
                            "/instrumentation/magnetic-compass/indicated-heading-deg",
                            "/instrumentation/slip-skid-ball/indicated-slip-skid",
                            "/instrumentation/turn-indicator/indicated-turn-rate",
                            "/instrumentation/vertical-speed-indicator/indicated-speed-fpm",
                            "/controls/flight/aileron",
                            "/controls/flight/elevator",
                            "/controls/flight/rudder",
                            "/controls/flight/flaps",
                            "/controls/engines/current-engine/throttle",
                            "/engines/engine/rpm"};

    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // prepare the sockaddr_in structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    // bind
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    // listen
    listen(sockfd, rate);
    clilen = sizeof(cli_addr);

    // accept connection from an incoming client
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
    if (newsockfd < 0) {
        perror("ERROR on accept");
        exit(1);
    }

    string nextBuf;
    bool flash = false;

    // if connection is established then start communicating
    while (programRunning) {
        if (!isClientSend) {
            sleep(1 / rate);
            bzero(buffer, 256);
            read(newsockfd, buffer, 255);
            string conectedBuffer = nextBuf + buffer;
            nextBuf = "";
            string segment;
            vector<string> seglist;
            flash = false;
            for (auto c:conectedBuffer) {
                if (c == '\n') {
                    flash = true;
                    continue;
                }
                if (flash) {
                    nextBuf += c;
                    continue;
                }
                if (c != ',') {
                    segment += c;
                } else {
                    seglist.push_back(segment);
                    segment = "";
                }
            }
            if (!segment.empty()) {
                seglist.push_back(segment);
            }
            // update the value of the relevant vars
            for (int i = 0; i < min(pathes.size(), seglist.size()); ++i) {
                mtxMap.lock();
                for (auto &it : *symTbl) {
                    if(it.second->getPath() == pathes[i]){
                        it.second->setValue(stod(seglist[i]));
                    }
                }
                mtxMap.unlock();
            }
        }
    }
}

void OpenServerCommand::doCommand() {
    this->threads->addThread(new thread(OpenServer, this->symTbl, this->port, this->rate));
}

OpenServerCommand::OpenServerCommand(vector<string> &code, map<string, VarData *> *symTbl, Threads* threads) {
    int index = 1;
    Utils utils;
    list<string> portList;
    list<string> rateList;
    bool isLastOp = true;

    // port expression until 2 numbers or ,
    while (code[index] != ","
    && (isLastOp || utils.isOperator(code[index]) || code[index] == "(" || code[index] == ")")) {
        portList.push_back(code[index]);
        isLastOp = (utils.isOperator(code[index]) || code[index] == "(" || code[index] == ")");
        index++;
    }

    // skip ,
    if(code[index]==",") {
        ++index;
    }

    // rate expression until end
    while (index < code.size() && code[index] != ";") {
        rateList.push_back(code[index]);
        ++index;
    }

    // init members
    this->port = static_cast<int>(utils.evaluate(portList, symTbl));
    this->rate = static_cast<int>(utils.evaluate(rateList, symTbl));
    this->symTbl = symTbl;
    this->threads = threads;
}

OpenServerCommand::~OpenServerCommand() {
    programRunning = false;
}

void DefineVarCommand::doCommand() {}

DefineVarCommand::DefineVarCommand(vector<string> &code, map<string, VarData *> *symTbl) {
    Utils utils;
    string path;
    double value;
    // name is second word
    string name = code[1];

    if (code[3] == "bind") {
        // case var x = bind "/bla/bla"
        if (code[4][0] == '"') {
            path = utils.removeApostrophes(code[4]);
            value = 0;
            // case var x = bind y
        } else {
            path = symTbl->at(code[4])->getPath();
            value = symTbl->at(code[4])->getValue();
        }
    } else {
        // '/default' because not bind
        path = "/default";
        // case var x = y
        if (symTbl->count(code[3])) {
            value = symTbl->at(code[3])->getValue();
            // case var x = 5
        } else {
            list<string> expList;
            // expression is after =
            expList.push_back(code[3]);
            int index = 4;
            while (index < code.size() && code[index] != ";") {
                expList.push_back(code[index]);
                ++index;
            }
            value = utils.evaluate(expList, symTbl);
        }
    }

    mtxMap.lock();
    // insert to symTbl the name, value and path
    symTbl->insert(pair<string, VarData *>(name, new VarData(value, path)));
    mtxMap.unlock();
}

void PlacementCommand::doCommand() {
    Utils utils;
    double value = utils.evaluate(this->right,this->symTbl);
    this->symTbl->at(this->left)->setValue(value);

    mtxClient.lock();
    // case no bind
    if(this->symTbl->at(this->left)->getPath()!="/default") {
        // client details
        clientPath = this->symTbl->at(this->left)->getPath();
        clientValue = static_cast<int>(value);
        isClientSend = true;
    }
    mtxClient.unlock();
}

PlacementCommand::PlacementCommand(vector<string> &code, map<string, VarData *> *symTbl) {
    int index = 1;

    // left is before =
    this->left = code[index - 1];

    // right expression is after =
    ++index;
    while (index < code.size()) {
        this->right.push_back(code[index]);
        ++index;
    }

    // init symTbl
    this->symTbl = symTbl;
}
