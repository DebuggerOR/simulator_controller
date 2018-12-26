

#include "Utils.h"

bool Utils::isOperator(string str) {
    return (str == "+" || str == "-" || str == "*" || str == "/" || str == ">" || str == "<"
    || str == ">=" || str == "<=" || str == "!=" || str == "==" || str == "OR" || str == "AND");
}

string Utils::removeApostrophes(string str) {
    string newStr;
    // copy all chars to new str without first and last
    for (int i = 1; i < str.size() - 1; ++i) {
        newStr += str[i];
    }
    return newStr;
}

list<string> Utils::infixToPostfix(list<string> infix) {
    // pre process
    infix = this->minusToInt(infix);

    list<string> postfix;
    stack<string> s;
    string lastOp;

    /**
     * Shunting Yard rules:
     *
     *  1. If the incoming symbols is an operand, print it.
     *  2. If the incoming symbol is a left parenthesis, push it on the stack.
     *  3. If the incoming symbol is a right parenthesis:
     *      discard the right parenthesis, pop and print the stack symbols
     *      until you see a left parenthesis. Pop the left parenthesis and discard it.
     *  4. If the incoming symbol is an operator and the stack is empty or contains
     *      a left parenthesis on top, push the incoming operator onto the stack.
     *  5. If the incoming symbol is an operator and has either higher precedence
     *      than the operator on the top of the stack, or has the same precedence as
     *      the operator on the top of the stack and is right associative -- push it on the stack.
     *  6. If the incoming symbol is an operator and has either lower precedence than the operator
     *      on the top of the stack, or has the same precedence as the operator on the top of the stack
     *      and is left associative -- continue to pop the stack until this is not true.
     *      Then, push the incoming operator.
     *  7. At the end of the expression, pop and print all operators on the stack.
     *      (No parentheses should remain.)
     */

    for (auto &it : infix) {
        if (!this->isOperator(it) && it != "(" && it != ")") {
            postfix.push_back(it);
        } else if (it == "(") {
            s.push(it);
            lastOp = it;
        } else if (it == "/") {
            s.push(it);
            lastOp = it;
        } else if (it == "*" && lastOp != "/") {
            s.push(it);
            lastOp = it;
        } else if (it == "-" && (lastOp != "*" && lastOp != "/")) {
            s.push(it);
            lastOp = it;
        } else if ((it == "+") && (lastOp != "*" && lastOp != "/" && lastOp != "-")) {
            s.push(it);
            lastOp = it;
        } else if ((it == "<" || it == ">" || it == "<=" || it == ">=" || it == "==" || it == "!=")
                   && (lastOp != "*" && lastOp != "/" && lastOp != "+" && lastOp != "-")) {
            s.push(it);
            lastOp = it;
        } else if ((it == "OR" || it == "AND") && (lastOp != "*" && lastOp != "/" && lastOp != "+"
                                                   && lastOp != "-" && it != "<" && it != ">" && it != "<=" &&
                                                   it != ">=" && it != "==" && it != "!=")) {
            s.push(it);
            lastOp = it;
        } else if ((it == "*") && (lastOp == "/")) {
            postfix.push_back(s.top());
            s.pop();
            s.push(it);
            lastOp = it;
        } else if ((it == "-") && (lastOp == "*" || lastOp == "/")) {
            postfix.push_back(s.top());
            s.pop();
            s.push(it);
            lastOp = it;
        } else if ((it == "+") && (lastOp == "*" || lastOp == "/" || lastOp == "-")) {
            postfix.push_back(s.top());
            s.pop();
            s.push(it);
            lastOp = it;
        } else if ((it == "<" || it == ">" || it == "<=" || it == ">=" || it == "==" || it == "!=")
                   && (lastOp == "*" || lastOp == "/" || lastOp == "+" || lastOp == "-")) {
            postfix.push_back(s.top());
            s.pop();
            s.push(it);
            lastOp = it;
        } else if ((it == "OR" || it == "AND") && (lastOp == "*" || lastOp == "/" || lastOp == "+" || lastOp == "-"
                                                   || it == "<" || it == ">" || it == "<=" || it == ">=" ||
                                                   it == "==" || it == "!=")) {
            postfix.push_back(s.top());
            s.pop();
            s.push(it);
            lastOp = it;
            // to the queue until (
        } else if (it == ")") {
            while (s.top() != "(") {
                postfix.push_back(s.top());
                s.pop();
            }
            s.pop();
            if (!s.empty()) {
                lastOp = s.top();
            } else {
                lastOp = "";
            }
        } else {
            throw "be careful!!";
        }
    }
    // put all the rest to the queue
    while (!s.empty()) {
        postfix.push_back(s.top());
        s.pop();
    }
    return postfix;
}

list<string> Utils::placeValue(list<string> lst, map<string, VarData *> *symTbl) {
    list<string> retList;
    for (auto &it : lst) {
        // if the str is var, place it's value
        if (symTbl->count(it)) {
            retList.push_back(to_string(symTbl->at(it)->getValue()));
        } else {
            retList.push_back(it);
        }
    }
    return retList;
}

double Utils::postfixToDouble(list<string> postfix) {
    stack<Expression *> stack;

    for (auto &it : postfix) {
        if (!this->isOperator(it)) {
            stack.push(new Number(stod(it)));
        } else {
            Expression *o1 = stack.top();
            stack.pop();
            Expression *o2;
            // avoid empty stack case
            if (!stack.empty()) {
                o2 = stack.top();
                stack.pop();
            } else {
                o2 = new Number(0);
            }

            if (it == "+") {
                stack.push(new Plus(o2, o1));
            } else if (it == "-") {
                stack.push(new Minus(o2, o1));
            } else if (it == "/") {
                stack.push(new Div(o2, o1));
            } else if (it == "*") {
                stack.push(new Mul(o2, o1));
            } else if (it == ">") {
                stack.push(new Greater(o2, o1));
            } else if (it == "<") {
                stack.push(new Smaller(o2, o1));
            } else if (it == ">=") {
                stack.push(new GreaterEqual(o2, o1));
            } else if (it == "<=") {
                stack.push(new SmallerEqual(o2, o1));
            } else if (it == "==") {
                stack.push(new Equal(o2, o1));
            } else if (it == "!=") {
                stack.push(new NotEqual(o2, o1));
            } else if (it == "OR") {
                stack.push(new OR(o2, o1));
            } else if (it == "AND") {
                stack.push(new AND(o2, o1));
            } else {
                throw "be careful!!";
            }
        }
    }
    double result = stack.top()->calculate();
    // free all the expressions in the tree
    delete stack.top();
    return result;
}

double Utils::evaluate(list<string> &infix, map<string, VarData *> *symTbl) {
    return this->postfixToDouble(this->infixToPostfix(this->placeValue(infix, symTbl)));
}

list<string> Utils::minusToInt(list<string> lst) {
    auto it = lst.begin();
    // if first is -
    if (*it == "-") {
        if (it != lst.end()) {
            (*it) = "";
            int negNum = -1 * stod(*(++it));
            (*it) = to_string(negNum);
        }
    }
    // if - after operator or (
    for (; it != lst.end(); it++) {
        if (*it == "(" || this->isOperator(*it)) {
            if (*(++it) == "-") {
                (*it) = "";
                int negNum = -1 * stod(*(++it));
                (*it) = to_string(negNum);
            }
        }
    }
    lst.remove("");
    return lst;
}
