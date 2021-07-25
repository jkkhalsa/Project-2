#include <vector>
#include <iostream>

#include "lexer.h"
#include "parser.h"
#include "variable.h"

using namespace std;



Parser::Parser(){
    //lexer = new LexicalAnalyzer();
    index = 0;
}


// peek requires that the argument "howFar" be positive.
Token Parser::Peek(int howFar)
{
    if (howFar <= 0) {      // peeking backward or in place is not allowed
        cout << "LexicalAnalyzer:peek:Error: non positive argument\n";
        exit(-1);
    } 

    int peekIndex = index + howFar - 1;
    if (peekIndex > (int)(tokenList.size()-1)) { // if peeking too far
        Token token;                        // return END_OF_FILE
        token.lexeme = "";
        token.line_no = -1;
        token.token_type = END_OF_FILE;
        return token;
    } else{
        cout << "DEBUG: token type peeked at is " << tokenList[peekIndex].token_type << "\n";
        return tokenList[peekIndex];
        
    }

        
}

void Parser::SyntaxError()
{
    cout << "SYNTAX ERROR\n";
    cout << "error on token type " << token.token_type << "\n";
    cout << "token is " << token.lexeme << " at line " << token.line_no << "\n";
    exit(1);
}

void Parser::printTokenList(){
    for(auto elem : tokenList){
        elem.Print();
    }
    return;
}

//increments the index of tokenList in the function itself
Token Parser::expect(TokenType expected_type)
{
    index++;
    Token t = tokenList[index];
    if(t.token_type != expected_type){
        SyntaxError();
    }
    return t;

    
    /* BAZZI CODE
    Token t = lexer->GetToken();
    if (t.token_type != expected_type)
        SyntaxError();
    return t;*/
}

void Parser::parseProgram(){
    cout << "DEBUG: began parsing the program\n";
    //will either start with global variables or a scope
    //is global variables if we have an ID and a comma
    //place 0 will always be an ID - both gv and scope start that way
    //scope place 1 will have a lbrace, gv place 1 will have a comma
    token = Peek(2);
    if(token.token_type == COMMA){
        //this is global variables
        //need to parse a var_list
        parseVarList();
    }
    if(token.token_type == LBRACE){
        //this is scope
        //need to parse the scope
        parseScope();
    }
    else{
        SyntaxError();
    }
}

void Parser::parseGlobalVars(){
    cout << "DEBUG: parsing global variables\n";
    scopeList.push_back(":");
    //currentScope = ":";
    parseVarList();
    //if this doesn't end with a semicolon, we've a fuckin problem m8
    expect(SEMICOLON);
    return;
}

void Parser::parseVarList(){
    cout << "DEBUG: parsing a variable list\n";
    cout << "DEBUG: index is " << index << "\n";
    token = tokenList[index];
    cout << "DEBUG: token parsing is " << token.token_type << " with content " << token.lexeme << "\n";
    //a var list MUST start with an ID
    if(token.token_type == ID){
        cout << "DEBUG: token type is, in fact, ID\n";
        //if we've got an ID, add it as a variable with all the niceties
        symbolTable.addVariable(scopeList.back(), token.lexeme, currentlyPublic);
        cout << "DEBUG: added variable with scope " << scopeList.back() << ", name " << token.lexeme << ", and isPublic " << currentlyPublic << "\n";
        index++;  //we've now made sense of this token
    }
    else{
        //we did a whoopsie fuck off now <3
        SyntaxError();
    }
    
    //what next? well, check if there's a comma, which means the var list keeps going
    token = tokenList[index];
    if(token.token_type == COMMA){
        index++; //we've now made sense of this token
        parseVarList();
    }
    return;
}

void Parser::parseScope(){
    cout << "DEBUG: parsing scope\n";
    token = tokenList[index];
    scopeList.push_back(token.lexeme);
    //currentScope = token.lexeme;
    //we've got our scope locked in, make sure our syntax is correct
    expect(LBRACE);
    //ok good now we have public and private variables to check for
    index++;
    token = tokenList[index];
    if(token.token_type == PUBLIC){
        index++; //we've now made sense of this token
        parsePublicVars();
    }
    if(token.token_type == PRIVATE){
        index++; //we've now made sense of this token
        parsePrivateVars();
    }

    token = tokenList[index];
    //if we're past both public and private, then this is a statement list
    //so if we don't have an equals sign we've got a syntax error
    if(Peek(1).token_type == EQUAL){
        parseStmtList();
    }
    else{
        SyntaxError();
    }
    //past the statement list, now do we have an rbrace?
    expect(RBRACE);
    //we've hit an rbrace - that means we need to delete all the variables belonging to this scope
    symbolTable.eraseScope(scopeList.back());
    //and now delete this from our list of nested scopes
    scopeList.pop_back();
    return;
}

void Parser::parsePublicVars(){
    cout << "DEBUG: parsing public variables\n";
    currentlyPublic = true;
    expect(COLON);
    parseVarList();
    expect(SEMICOLON);
    return;
}

void Parser::parsePrivateVars(){
    cout << "DEBUG: parsing private variables\n";
    currentlyPublic = false;
    expect(COLON);
    parseVarList();
    expect(SEMICOLON);
    return;
}

void Parser::parseStmtList(){
    //so if this doesn't start with an ID we've got a problem
    token = tokenList[index];
    if(token.token_type != ID){
        SyntaxError();
    }
    //cool we're here now so this is either going to stmt or scope, check one ahead
    if(Peek(1).token_type == EQUAL){
        parseStmt();
    }
    if(Peek(1).token_type == LBRACE){
        parseScope();
    }

    //there's either another statement list starting here or else we need to return
    //once again, only way to know is check one ahead
    if(Peek(1).token_type == EQUAL || Peek(1).token_type == LBRACE){
        parseStmtList();
    }
    return;


}

void Parser::parseStmt(){
    //here's where we're going to output what's needed for the program
    if(Peek(3).token_type != SEMICOLON){  //TODO: make sure this is peeking the right number of spaces
        SyntaxError();
    }
    //TODO: search through the list for each variable name and print out their scopes as directed


}



int main()
{
    LexicalAnalyzer lexer;
    Parser* parser = new Parser();
    Token token;

    token = lexer.GetToken();
    if(token.token_type != ERROR){  //is this cheating? we'll find out
         parser->tokenList.push_back(token); //add what we just determined to the end of the token list
    }
    //token.Print();

    //find the list of tokens
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        if(token.token_type != ERROR){  //is this cheating? we'll find out
             parser->tokenList.push_back(token); //add what we just determined to the end of the token list
        }
        //token.Print();
    }
    parser->printTokenList();

    //parse that list into actual output
    parser->parseProgram();

}
