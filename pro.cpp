#include <iostream>
#include <string>
#include <stack>
#include <cmath>
#include <stdexcept>

using namespace std;

// Tokenization
vector<string> tokenize(const string& input) {
    vector<string> tokens;
    string token;
    for (char c : input) {
        if (c == ') continue; // ignore whitespace
        if (c == '(' || c == ')') {
            tokens.push_back(string(1, c));
        } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            tokens.push_back(string(1, c));
        } else if (isdigit(c)) {
            token += c;
        } else {
            throw invalid_argument("Invalid character in input expression");
        }
    }
    if (!token.empty()) tokens.push_back(token);
    return tokens;
}

// Shunting-yard Algorithm
vector<string> shuntingYard(const vector<string>& tokens) {
    stack<char> opStack;
    vector<string> postfix;
    for (const string& token : tokens) {
        if (token == "(") {
            opStack.push('(');
        } else if (token == ")") {
            while (opStack.top()!= '(') {
                postfix.push_back(string(1, opStack.top()));
                opStack.pop();
            }
            opStack.pop(); // remove '('
        } else if (token == "+" || token == "-") {
            while (!opStack.empty() && opStack.top()!= '(') {
                postfix.push_back(string(1, opStack.top()));
                opStack.pop();
            }
            opStack.push(token[0]);
        } else if (token == "*" || token == "/") {
            while (!opStack.empty() && opStack.top()!= '(' && opStack.top()!= "+" && opStack.top()!= "-") {
                postfix.push_back(string(1, opStack.top()));
                opStack.pop();
            }
            opStack.push(token[0]);
        } else if (token == "^") {
            while (!opStack.empty() && opStack.top()!= '(' && opStack.top()!= "+" && opStack.top()!= "-" && opStack.top()!= "*" && opStack.top()!= "/") {
                postfix.push_back(string(1, opStack.top()));
                opStack.pop();
            }
            opStack.push(token[0]);
        } else {
            postfix.push_back(token);
        }
    }
    while (!opStack.empty()) {
        postfix.push_back(string(1, opStack.top()));
        opStack.pop();
    }
    return postfix;
}

// Postfix Evaluation
double evaluatePostfix(const vector<string>& postfix) {
    stack<double> evalStack;
    for (const string& token : postfix) {
        if (token == "+") {
            double op2 = evalStack.top();
            evalStack.pop();
            double op1 = evalStack.top();
            evalStack.pop();
            evalStack.push(op1 + op2);
        } else if (token == "-") {
            double op2 = evalStack.top();
            evalStack.pop();
            double op1 = evalStack.top();
            evalStack.pop();
            evalStack.push(op1 - op2);
        } else if (token == "*") {
            double op2 = evalStack.top();
            evalStack.pop();
            double op1 = evalStack.top();
            evalStack.pop();
            evalStack.push(op1 * op2);
        } else if (token == "/") {
            double op2 = evalStack.top();
            evalStack.pop();
            double op1 = evalStack.top();
            evalStack.pop();
            if (op2 == 0) throw runtime_error("Division by zero");
            evalStack.push(op1 / op2);
        } else if (token == "^") {
            double op2 = evalStack.top();
            evalStack.pop();
            double op1 = evalStack.top();
            evalStack.pop();
            evalStack.push(pow(op1, op2));
        } else {
            evalStack.push(stod(token));
        }
    }
    return evalStack.top();
}

int main() {
    string input;
    cout << "Enter an expression: ";
    getline(cin, input);
    try {
        vector<string> tokens = tokenize(input);
        vector<string> postfix = shuntingYard(tokens);
        double result = evaluatePostfix(postfix);
        cout << "Result: " << result << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}