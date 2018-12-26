

#ifndef CONDITIONCOMMAND_H
#define CONDITIONCOMMAND_H

#include "Command.h"

class ConditionCommand : public Command {

public:
    virtual void doCommand() override = 0;
};

#endif
