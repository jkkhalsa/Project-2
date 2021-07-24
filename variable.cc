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