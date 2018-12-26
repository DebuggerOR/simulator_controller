

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <map>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <utility>

#include "VarData.h"
#include "Expression.h"
#include "BinaryExpression.h"
#include "MathExpressions.h"


using namespace std;

class Utils {
public:
    bool isOperator(string str);

    string removeApostrophes(string str);

    list<string> minusToInt(list<string> lst);

    list<string> infixToPostfix(list<string> infix);

    list<string> placeValue(list<string> lst, map<string, VarData *> *symTbl);

    double postfixToDouble(list<string> postfix);

    double evaluate(list<string> &infix, map<string, VarData *> *symTbl);
};


#endif
