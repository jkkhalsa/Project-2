#include <iostream>
#include "variable.h"

using namespace std;

string Variable::printVariable(){
    if(scope == ":"){
        return scope + ':' + name;
    }
    return scope + '.' + name;
}


VariableList::VariableList(){

}

//scope, name, isPublic?
void VariableList::addVariable(string s, string n, bool p){
    //create new variable
    Variable variable;
    variable.scope = s;
    variable.name = n;
    variable.isPublic = p;

    //add it to the symbol table
    list.push_back(variable);
    return;
}

//goes through the variable list and erases the ones belonging to the scope we just exited
void VariableList::eraseScope(string deletingScope){
    auto iterator = list.begin();
    while(iterator != list.end()){
        if(iterator->scope == deletingScope){
            iterator = list.erase(iterator);
        }
        else{
            ++iterator;
        }
    }
    return;
}

//Search through our symbol table to see where the one that just got called is at
//pass current scope, then variable name we're looking for
Variable VariableList::searchList(string currentScope, string variableName){
    auto iterator = list.end();
    while(iterator >= list.begin()){
        if(iterator->name == variableName){
            if(iterator->scope == currentScope || iterator->isPublic == true || iterator->scope == ":"){
                //if we find the variable matching the scope and the name
                return *iterator;
            }
        }
        --iterator;
    }
    //if we're down here then there's no variable in the entire list that matches
    //so just make one ez
    Variable variable;
    variable.scope = "?";
    variable.name = variableName;
    return variable;
}