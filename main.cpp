#include <iostream>
#include <sstream>
#include <queue>
#include <stack>
#include <map>
#include <cctype>

using namespace std;

class Tokenization {
public:
    static queue<string> Tokenize(const string& expression) {
        queue<string> tokens;
        string token;
        size_t i = 0;

        while (i < expression.size()) {
            char ch = expression[i];

            if (isdigit(ch) || ch == '.') {
                token += ch;
                i++;

                while (i < expression.size() && (isdigit(expression[i]) || expression[i] == '.')) {
                    token += expression[i];
                    i++;
                }

                tokens.push(token);
                token.clear();
            } else if (isalpha(ch)) {
                token += ch;
                i++;

                while (i < expression.size() && isalpha(expression[i])) {
                    token += expression[i];
                    i++;
                }

                tokens.push(token);
                token.clear();
            } else if (string("+-*/").find(ch) != string::npos || ch == '(' || ch == ')' || ch == '.') {
                tokens.push(string(1, ch));
                i++;
            } else if (isspace(ch)) {
                i++;
            } else {
                throw invalid_argument("Invalid token in expression: " + string(1, ch));
            }
        }

        return tokens;
    }
};

class ShuntingYard {
public:
    static queue<string> RPN(queue<string>& tokens) {
        queue<string> rpn;
        stack<string> opersStack;

        while (!tokens.empty()) {
            string token = tokens.front();
            tokens.pop();

            if (isdigit(token[0]) || (token[0] == '-' && token.size() > 1)) {
                rpn.push(token);
            } else if (isFunction(token)) {
                opersStack.push(token);
            } else if (token == ",") {
                while (!opersStack.empty() && opersStack.top() != "(") {
                    rpn.push(opersStack.top());
                    opersStack.pop();
                }
                if (opersStack.empty() || opersStack.top() != "(") {
                    throw runtime_error("Mismatched parentheses or misplaced comma.");
                }
            } else if (token == "(") {
                opersStack.push(token);
            } else if (token == ")") {
                while (!opersStack.empty() && opersStack.top() != "(") {
                    rpn.push(opersStack.top());
                    opersStack.pop();
                }
                if (opersStack.empty() || opersStack.top() != "(") {
                    throw runtime_error("Mismatched parentheses.");
                }
                opersStack.pop();
                if (!opersStack.empty() && isFunction(opersStack.top())) {
                    rpn.push(opersStack.top());
                    opersStack.pop();
                }
            } else if (isOperator(token[0])) {
                while (!opersStack.empty() && priority(opersStack.top()) >= priority(token)) {
                    rpn.push(opersStack.top());
                    opersStack.pop();
                }
                opersStack.push(token);
            } else if (isalpha(token[0])) {
                rpn.push(token);
            } else {
                throw runtime_error("Invalid token in expression: " + token);
            }
        }

        while (!opersStack.empty()) {
            if (opersStack.top() == "(" || opersStack.top() == ")") {
                throw runtime_error("Mismatched parentheses.");
            }
            rpn.push(opersStack.top());
            opersStack.pop();
        }

        return rpn;
    }
private:
    static bool isFunction(const string& token) {
        return token == "pow" || token == "abs" || token == "max" || token == "min";
    }

    static bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/';
    }

    static int priority(const string& op) {
        if (op == "+" || op == "-") return 1;
        if (op == "*" || op == "/") return 2;
        return 0;
    }
};

int main() {
    string input;
    while (true) {
        try {
            cout << "Enter expression(or 'exit' to quit): ";
            getline(cin, input);
            if (input != "exit") {
                queue<string> tokens = Tokenization::Tokenize(input);
                queue<string> rpn = ShuntingYard::RPN(tokens);

                while (!rpn.empty()) {
                    cout << rpn.front() << " ";
                    rpn.pop();
                }
                cout << endl;
            } else {
                break;
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }

        return 0;
    }
};