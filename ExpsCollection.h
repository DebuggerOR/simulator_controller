

#ifndef EXPSCOLLECTION_H
#define EXPSCOLLECTION_H

#include <iostream>
#include <list>
#include <typeinfo>
#include "Expression.h"

using namespace std;

class ExpsCollection {
    list<Expression *> expressions;

public:
    ExpsCollection();

    ~ExpsCollection();

    void addExpression(Expression *expression);

    void executeAll();

    void deleteAll();
};


#endif
