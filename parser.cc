#include <vector>
#include <iostream>

#include "lexer.h"
#include "parser.h"
#include "variable.h"

using namespace std;



Parser::Parser(){
    lexer = new LexicalAnalyzer();
    index = 0;
    symbolTable = new VariableList();
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
    } else
        return tokenList[peekIndex];
}

void Parser::SyntaxError()
{
    cout << "SYNTAX ERROR\n";
    exit(1);
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
    //will either start with global variables or a scope
    //is global variables if we have an ID and a comma
    //place 0 will always be an ID - both gv and scope start that way
    //scope place 1 will have a lbrace, gv place 1 will have a comma
    token = Peek(1);
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
    currentScope = ":";
    parseVarList();
    //if this doesn't end with a semicolon, we've a fuckin problem m8
    expect(SEMICOLON);
    return;
}

void Parser::parseVarList(){
    token = tokenList[index];
    symbolTable->addVariable(currentScope, token.lexeme, currentlyPublic);
    cout << "DEBUG: added variable with scope " << currentScope << ", name " << token.lexeme << ", and isPublic " << currentlyPublic << "\n";
    index++;  //we've now made sense of this token

    //what next? well, check if there's a comma
    token = tokenList[index];
    if(token.token_type == COMMA){
        parseVarList();
    }
    return;
}

void Parser::parseScope(){
    token = tokenList[index];
    currentScope = token.lexeme;
    //we've got our scope locked in, make sure our syntax is correct
    expect(LBRACE);
    //ok good now we have public and private variables to check for
    index++;
    token = tokenList[index];
    if(token.token_type == PUBLIC){
        parsePublicVars();
    }
    if(token.token_type == PRIVATE){
        parsePrivateVars();
    }

    index++;
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
    //TODO: delete everything in the symbol list that has this scope in it, then clear the scope
}

void Parser::parsePublicVars(){
    
}




int main()
{
    LexicalAnalyzer lexer;
    Parser parser;
    Token token;

    token = lexer.GetToken();
    parser.tokenList.push_back(token);
    //token.Print();

    //find the list of tokens
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        parser.tokenList.push_back(token); //add what we just determined to the end of the token list
        token.Print();
    }


    //parse that list into actual output
    parser.parseProgram();

}
