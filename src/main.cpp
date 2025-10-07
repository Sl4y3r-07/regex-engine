
#include "regex.hpp"

int main() {
   
    std::string regex;
    std::string input;
    std::cout<<"Enter regex pattern:\n"<<std::endl;
    std::cin>>regex;
    std::cout<<"Enter input:\n"<<std::endl;
    std::cin>>input;
    
    regexEngine re;
    NFA nfa = re.compile(regex);
    bool isMatch = re.match(input);
    std::cout<<isMatch<<std::endl;
    
}
