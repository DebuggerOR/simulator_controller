
#ifndef COMMAND_H
#define COMMAND_H

class Command {
public:
    virtual void doCommand() = 0;

    virtual ~Command() = default;
};


#endif
