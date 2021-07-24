#include <vector>
#include <iostream>

class Variable{

    public:
    std::string scope;
    std::string name;
    //std::string content;
    bool isPublic;

    Variable();
};

class VariableList{

    public:
    std::vector <Variable> list;

    VariableList();
    void addVariable(std::string, std::string, bool); 


};