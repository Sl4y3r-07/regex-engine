#include <vector>
#include <map>
#include <iostream>
#include <set>

using namespace std;
class State{

public:    
    
    void addTransition(char c, State *);
    void addEpsilonTransition(State*);
    bool is_accept;
    std::map<char, std::vector<State*>> transitions;
    std::vector<State*> epsilonTransitions;
};

class NFA{

public:
      State* start;
      State* accept;
      
      vector<State*> states;

      static NFA forUnion(NFA &a, NFA &b); // '|'
      static NFA forConcat(NFA &a, NFA &b); // 'ab'
      static NFA forKleenClosure(NFA &a); // 'a*'
      static NFA forPositiveClosure(NFA &a); // 'a+'
      static NFA forChar(char c);

      NFA(){
        start = new State();
        accept = new State();
        accept->is_accept = true;
        states.push_back(start);
        states.push_back(accept);
      }

};

class NFAMatcher{
   public:
      static bool matches(const NFA &nfa, const std::string &str);
   private:
      static std::set<State*> epsilonClosures(const std::set<State*>& states);
};

