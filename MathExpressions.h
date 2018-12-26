

#ifndef MATHEXPRESSIONS_H
#define MATHEXPRESSIONS_H


#include "Expression.h"
#include "BinaryExpression.h"


class Number: public Expression {
    double value;

public:
    explicit Number(double value);

    double calculate() override;
};


class Plus : public BinaryExpression {

public:
    double calculate() override;

    Plus(Expression* left, Expression* right);
};


class Minus : public BinaryExpression {

public:
    double calculate() override;

    Minus(Expression *left, Expression *right);
};


class Mul : public BinaryExpression {

public:
    double calculate() override;

    Mul(Expression *left, Expression *right);
};


class Div : public BinaryExpression{

public:
    double calculate() override;
    Div(Expression* left, Expression* right);
};


class Equal : public BinaryExpression{
public:
    double calculate() override;

    Equal(Expression* left, Expression* right);

};


class NotEqual : public BinaryExpression {
public:
    double calculate() override;

    NotEqual(Expression *left, Expression *right);
};


class Greater : public BinaryExpression{
public:
    double calculate() override;

    Greater(Expression* left, Expression* right);

};


class GreaterEqual : public BinaryExpression{
public:
    double calculate() override;

    GreaterEqual(Expression* left, Expression* right);
};


class Smaller : public BinaryExpression{
public:
    double calculate() override;

    Smaller(Expression* left, Expression* right);

};


class SmallerEqual : public BinaryExpression{
public:
    double calculate() override;

    SmallerEqual(Expression* left, Expression* right);
};


class AND : public BinaryExpression{
public:
    double calculate() override;

    AND(Expression* left, Expression* right);
};


class OR : public BinaryExpression{
public:
    double calculate() override;

    OR(Expression* left, Expression* right);

};


#endif
