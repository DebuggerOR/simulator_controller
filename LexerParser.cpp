

#include "LexerParser.h"

vector<string> Lexer::lexer(string input) {
    int i = 0;
    string word;
    bool isFirstBlank = true;
    bool isPath = false;
    vector<string> words;

    // pass blanks at the beginning
    while (i < input.size() && input[i] == ' ' || input[i] == '\t' || input[i] == '\n') {
        ++i;
    }

    // pass over input
    while (i < input.size()) {
        // case " set flag that -,/ etc. aren't operators
        if (input[i] == '"') {
            isPath = true;
        }
        // mark end line with ;
        if (input[i] == '\n') {
            if (word != "") {
                words.push_back(word);
                word = "";
            }
            words.push_back(";");
            isFirstBlank = true;
            isPath = false;
            // operators
        } else if (input[i] == '+' || input[i] == '*' || input[i] == '('
                   || input[i] == ')' || input[i] == ',' || input[i] == '{') {
            if (word != "") {
                words.push_back(word);
                word = "";
            }
            words.push_back(string(1, input[i]));
            // more operators
        } else if (input[i] == '=' || input[i] == '<' || input[i] == '>') {
            // ==, <=, >=
            if (input[i + 1] == '=') {
                if (word != "") {
                    words.push_back(word);
                    word = "";
                }
                word += input[i];
                word += input[i + 1];
                words.push_back(word);
                word = "";
                i++;
                // =, <, >
            } else {
                if (word != "") {
                    words.push_back(word);
                    word = "";
                }
                words.push_back(string(1, input[i]));
            }
            // operator !=
        } else if (input[i] == '!') {
            if (input[i + 1] == '=') {
                if (word != "") {
                    words.push_back(word);
                    word = "";
                }
                word += input[i];
                word += input[i + 1];
                words.push_back(word);
                word = "";
                i++;
            }
            // case not in path, /,- are operators
        } else if (!isPath && (input[i] == '/' || input[i] == '-')) {
            if (word != "") {
                words.push_back(word);
                word = "";
            }
            words.push_back(string(1, input[i]));
            // blanks
        } else if (input[i] == ' ' || input[i] == '\t') {
            if (isFirstBlank && word != "") {
                words.push_back(word);
                word = "";
                isFirstBlank = false;
            }
            //regular chars
        } else {
            word += input[i];
            isFirstBlank = true;
        }
        i++;
    }
    // add last word
    if (word != "") {
        words.push_back(word);
    }
    // remove double ";"
    for (int j = 1; j < words.size(); ++j) {
        if (words[j - 1] == ";" && words[j] == ";") {
            words[j - 1] = "";
        }
    }

    return words;
}

void Parser::parse(vector<string> &code, ExpsCollection *expressions, CommandFactory *factory,
                   map<string, string> &codeMap) {
    int index = 0;
    if (code.size() > 0 && code[code.size() - 1] != ";") {
        code.push_back(";");
    }
    vector<string> commandCode;
    while (index < code.size()) {
        // skip empty string
        if (code[index] == "" && index < code.size() - 1) {
            ++index;
        }
        if (codeMap.count(code[index])) {
            if (codeMap[code[index]] == "command") {
                commandCode.clear();
                commandCode.push_back(code[index]);
                index++;
                while (index < code.size() && code[index] != ";") {
                    commandCode.push_back(code[index]);
                    index++;
                    // skip empty string
                    if (code[index] == "" && index < code.size() - 1) {
                        ++index;
                    }
                }
                index++;
                expressions->addExpression(new CommandExpression(factory->createCommand(commandCode)));
                // case command with block
            } else {
                commandCode.clear();
                commandCode.push_back(code[index]);
                index++;
                while (index < code.size() && code[index] != "}") {
                    commandCode.push_back(code[index]);
                    index++;
                    // skip empty string
                    if (code[index] == "" && index < code.size() - 1) {
                        ++index;
                    }
                }
                index += 2;
                expressions->addExpression(new CommandExpression(factory->createCommand(commandCode)));
            }
            // case command word is the second word ("=")
        } else if (code.size() > index + 1 && codeMap.count(code[index + 1])) {
            commandCode.clear();
            commandCode.push_back(code[index]);
            index++;
            while (code[index] != ";") {
                commandCode.push_back(code[index]);
                index++;
                // skip empty string
                if (code[index] == "" && index < code.size() - 1) {
                    ++index;
                }
            }
            expressions->addExpression(new CommandExpression(factory->createCommand(commandCode)));
            index++;
        } else {
            index++;
        }
    }
}
