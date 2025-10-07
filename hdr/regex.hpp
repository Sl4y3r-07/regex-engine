#include "nfa.hpp"
#include <stack>
#include <iostream>
#include <string>

class regexEngine{
public:   
    NFA compile(std::string &regex);
    std::string toPostfix(std::string &regex);
    bool match(std::string &string);
private:
    NFA nfa_g;
    bool isOperator(char c);
    int precedence(char c);
};