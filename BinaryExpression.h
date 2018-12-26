

#ifndef BINARYEXPRESSION_H
#define BINARYEXPRESSION_H

#include "Expression.h"

class BinaryExpression :public Expression {
protected:
    Expression *right;
    Expression *left;

public:
    BinaryExpression(Expression *left, Expression *right);

    virtual double calculate() = 0;

    ~BinaryExpression() override;
};


#endif
