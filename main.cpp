#include <iostream>
#include <sstream>
#include <queue>
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

int main() {
    string input;
    while (true) {
            cout << "Enter expression (or 'exit' to quit): ";
            getline(cin, input);
            if (input != "exit") {
                queue<string> tokens = Tokenization::Tokenize(input);

                while (!tokens.empty()) {
                    cout << tokens.front() << " ";
                    tokens.pop();
                }
                cout << endl;
            } else {
                break;
            }
    }

    return 0;
}