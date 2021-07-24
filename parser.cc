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
    symbolTable->addVariable("global", token.lexeme, true);
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
