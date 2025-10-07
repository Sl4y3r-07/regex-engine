#include "nfa.hpp"
#include <exception>
#include <iterator>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>


void State::addEpsilonTransition(State* s){
    epsilonTransitions.push_back(s);
}

void State::addTransition(char c, State* s){
    transitions[c].push_back(s);
}

NFA NFA::forChar(char c){
    NFA nfa;
    nfa.start->addTransition(c, nfa.accept);
    nfa.accept->is_accept = true;
    return nfa;
}

NFA NFA::forConcat(NFA &a, NFA &b){
    NFA nfa;
    a.accept->addEpsilonTransition(b.start);
    nfa.start = a.start;
    nfa.accept = b.accept;
    //make sure to make false the accept state of a
    a.accept->is_accept = false;
    return nfa;
}

NFA NFA::forUnion(NFA &a, NFA &b){
    NFA nfa;
    // this time there will be two states
    nfa.start->addEpsilonTransition(a.start);
    nfa.start->addEpsilonTransition(b.start);
    a.accept->addEpsilonTransition(nfa.accept);
    b.accept->addEpsilonTransition(nfa.accept);
    a.accept->is_accept = false;
    b.accept->is_accept = false;

    return nfa;
}

NFA NFA::forKleenClosure(NFA &a){
    NFA nfa;
    nfa.start->addEpsilonTransition(a.start);
    a.accept->addEpsilonTransition(nfa.accept);
    a.accept->addEpsilonTransition(a.start);
    nfa.start->addEpsilonTransition(nfa.accept);
    a.accept->is_accept=false;
    return nfa;
}

NFA NFA::forPositiveClosure(NFA &a){
    NFA nfa;
    nfa.start->addEpsilonTransition(a.start);
    a.accept->addEpsilonTransition(nfa.accept);
    a.accept->addEpsilonTransition(a.start);
    a.accept->is_accept=false;
    return nfa;
}

std::set<State*> NFAMatcher::epsilonClosures(const std::set<State*>& states){
    std::queue<State*> q;
    std::set<State*> closure(states.begin(), states.end());
    for(State* s: states){
        q.push(s);
    }

    while(!q.empty()){
        State* s_ = q.front();
        q.pop();

        for(State* es: s_->epsilonTransitions){
            if(closure.find(es) == closure.end()){
                q.push(es);
                closure.insert(es);
            }
        }
    }

    return closure;
}


bool NFAMatcher::matches(const NFA &nfa, const std::string &str){
    std::set<State*> curr_epsilonStates = epsilonClosures({nfa.start});

    for(char c :str){
           std::set<State*> nextStates;

           for(State* s: curr_epsilonStates){
             if(s->transitions.count(c)){
                for(State* s_: s->transitions[c]){
                    std::set<State*> states= epsilonClosures({s_});
                    nextStates.insert(states.begin(), states.end());
                }
             }
           }

           curr_epsilonStates = nextStates;

           if (curr_epsilonStates.empty()) 
              return false;
    }

    for (State* s : curr_epsilonStates)
    { 
        if (s->is_accept) 
           return true;
    }

 return false;
}





