#include <cctype>
#include "regex.hpp"
#include <string>


bool regexEngine::isOperator(char c) {
    return c == '*' || c == '|' || c == '.' || c == '+';
}

int regexEngine::precedence(char c) {
    if ((c == '*') || (c == '+')) return 3;
    if (c == '.') return 2;
    if (c == '|') return 1;
    return 0;
}

NFA regexEngine::compile(std::string &regex){
   std::string postfix = toPostfix(regex);
   std::stack<NFA> st;

    for (char c : postfix) {
        if (isalnum(c)) st.push(NFA::forChar(c));
        else if (c == '*') {
            NFA a = st.top(); 
            st.pop();
            st.push(NFA::forKleenClosure(a));
        }
        else if(c == '+'){
            NFA a = st.top(); 
            st.pop();
            st.push(NFA::forPositiveClosure(a));
        }
        else if (c == '.') {
            NFA b = st.top(); 
            st.pop();
            NFA a = st.top(); 
            st.pop();
            st.push(NFA::forConcat(a, b));
        }
        else if (c == '|') {
            NFA b = st.top(); 
            st.pop();
            NFA a = st.top(); 
            st.pop();
            st.push(NFA::forUnion(a, b));
        }
    }
    
    return nfa_g = st.top();
}

std::string regexEngine::toPostfix(std::string &regex){
    // need to add explicit concatenation -> '.'
    std::string output;
    std::stack<char> stk;
   
    int len = regex.length();
    std::string new_regex; // after explicit concatenation
    for(int i=0; i<len; i++){
        new_regex += regex[i];
        if(i+1<len){
            if ((isalnum(regex[i]) || regex[i] == ')' || regex[i] == '*' || regex[i] == '+') && (isalnum(regex[i+1]) || regex[i+1] == '(')) {
                new_regex += '.';
            }
        }
    }

    //postfix
    for (char c : new_regex) {
        if (isalnum(c)) output += c;
        else if (c == '(') stk.push(c);
        else if (c == ')') {
            while (!stk.empty() && stk.top() != '(') {
                output += stk.top(); 
                stk.pop();
            }
            stk.pop();
        } else {
            while (!stk.empty() && precedence(stk.top()) >= precedence(c)) {
                output += stk.top(); 
                stk.pop();
            }
            stk.push(c);
        }
    }

    while (!stk.empty()) { 
        output += stk.top(); 
        stk.pop(); 
    }

    return output;
}

bool regexEngine::match(std::string &string){
    return NFAMatcher::matches(nfa_g, string);
}