#include <iostream>
#include "variable.h"

using namespace std;



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

//TODO: Search through our symbol table to see where the one that just got called is at
Variable VariableList::searchList(string currentScope, string variableName){

}