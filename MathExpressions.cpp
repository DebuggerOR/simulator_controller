

#include "MathExpressions.h"


double Number::calculate() {
    return this->value;
}

Number::Number(double value) {
    this->value = value;
}

double Plus::calculate() {
    return this->left->calculate() + this->right->calculate();
}

Plus::Plus(Expression *left, Expression *right) : BinaryExpression(left, right) {
}

double Minus::calculate() {
    return this->left->calculate() - this->right->calculate();
}

Minus::Minus(Expression *left, Expression *right) : BinaryExpression(left, right) {
}

double Mul::calculate() {
    return this->left->calculate() * this->right->calculate();
}

Mul::Mul(Expression *left, Expression *right) : BinaryExpression(left, right) {
}

double Div::calculate() {
    return this->left->calculate() / this->right->calculate();
}

Div::Div(Expression *left, Expression *right) : BinaryExpression(left, right) {
}

double Equal::calculate() {
    return this->left->calculate() == this->right->calculate();
}

Equal::Equal(Expression *left, Expression *right) : BinaryExpression(left, right) {
}

double NotEqual::calculate() {
    return this->left->calculate() != this->right->calculate();
}

NotEqual::NotEqual(Expression *left, Expression *right) : BinaryExpression(left, right) {
}

double Greater::calculate() {
    return this->left->calculate() > this->right->calculate();
}

Greater::Greater(Expression *left, Expression *right) : BinaryExpression(left, right) {
}

double GreaterEqual::calculate() {
    return this->left->calculate() >= this->right->calculate();
}

GreaterEqual::GreaterEqual(Expression *left, Expression *right) : BinaryExpression(left, right) {
}

double Smaller::calculate() {
    return this->left->calculate() < this->right->calculate();
}

Smaller::Smaller(Expression *left, Expression *right) : BinaryExpression(left, right) {
}

double SmallerEqual::calculate() {
    return this->left->calculate() <= this->right->calculate();
}

SmallerEqual::SmallerEqual(Expression *left, Expression *right) : BinaryExpression(left, right) {
}

double AND::calculate() {
    return this->left->calculate() && this->right->calculate();
}

AND::AND(Expression *left, Expression *right) : BinaryExpression(left, right) {
}

double OR::calculate() {
    return this->left->calculate() || this->right->calculate();
}

OR::OR(Expression *left, Expression *right) : BinaryExpression(left, right) {
}
