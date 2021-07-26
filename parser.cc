#include <vector>
#include <iostream>

#include "lexer.h"
#include "parser.h"
#include "variable.h"

using namespace std;



Parser::Parser(){
    index = 0;
    output = "";
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
        //cout << "DEBUG: token type peeked at is " << tokenList[peekIndex].token_type << "\n";
        return tokenList[peekIndex];
        
    }

        
}

void Parser::SyntaxError()
{
    cout << "Syntax Error\n";
    //cout << "error on token type " << token.token_type << "\n";
    //cout << "token is " << token.lexeme << " at line " << token.line_no << "\n";
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
    Token t = tokenList[index];
    if(t.token_type != expected_type){
        SyntaxError();
    }
    //we've now made sense of this symbol
    index++;
    return t;
}

void Parser::parseProgram(){
    //cout << "DEBUG: began parsing the program\n";
    //will either start with global variables or a scope
    //is global variables if we have an ID and a comma
    //place 0 will always be an ID - both gv and scope start that way
    token = tokenList[index];
    if(token.token_type != ID){
        SyntaxError();
    }
    //scope place 1 will have a lbrace, gv place 1 will have a comma or semicolon
    token = Peek(2);
    if(token.token_type == COMMA || token.token_type == SEMICOLON){
        //this is global variables
        //need to parse global variables
        parseGlobalVars();
    }
    if(token.token_type == LBRACE){
        //this is scope
        //need to parse the scope
        parseScope();
    }
    //if we're here, then we've got an end of file, hopefully
    token = tokenList[index];
    if(token.token_type == END_OF_FILE){
        return;
    }
    else{
        SyntaxError();
    }
}

void Parser::parseGlobalVars(){
    //cout << "DEBUG: parsing global variables\n";
    scopeList.push_back(":");
    parseVarList();
    //if this doesn't end with a semicolon, we've a fuckin problem m8
    //cout << "DEBUG: returned from parsing var list in global variables\n";
    expect(SEMICOLON);
    
    //if we've got our semicolon, the next thing to happen should be a scope
    //check for this by peeking for an lbrace
    if(Peek(2).token_type != LBRACE){
        SyntaxError();
    }
    parseScope();
    return;
}

void Parser::parseVarList(){
    //cout << "DEBUG: parsing a variable list\n";
    token = tokenList[index];
    //a var list MUST start with an ID
    if(token.token_type == ID){
        //if we've got an ID, add it as a variable with all the niceties
        symbolTable.addVariable(scopeList.back(), token.lexeme, currentlyPublic);
        //cout << "DEBUG: added variable with scope " << scopeList.back() << ", name " << token.lexeme << ", and isPublic " << currentlyPublic << "\n";
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
    //cout << "DEBUG: parsing scope\n";
    token = tokenList[index];
    scopeList.push_back(token.lexeme);
    //cout << "our current scope is " << scopeList[scopeList.size()-1] << "\n";
    index++; //now made sense of the scope
    //we've got our scope locked in, make sure our syntax is correct
    expect(LBRACE);
    //ok good now we have public and private variables to check for
    token = tokenList[index];
    if(token.token_type == PUBLIC){
        index++; //we've now made sense of this token
        parsePublicVars();
    }
    if(token.token_type == PRIVATE){
        index++; //we've now made sense of this token
        parsePrivateVars();
    }
    //cout << "DEBUG: returned from parsing public and private variable lists, back in scope\n";
    //if we're past both public and private, then this is a statement list
    //the statement list will handle its own error cases
    parseStmtList();
    //past the statement list, now do we have an rbrace?
    expect(RBRACE);
    //we've hit an rbrace - that means we need to delete all the variables belonging to this scope
    symbolTable.eraseScope(scopeList.back());
    //and now delete this from our list of nested scopes
    //cout << "DEBUG: erased variables from scope " << scopeList[scopeList.size()-1] << "\n";
    scopeList.pop_back();
    return;
}

void Parser::parsePublicVars(){
    //cout << "DEBUG: parsing public variables\n";
    currentlyPublic = true;
    expect(COLON);
    parseVarList();
    expect(SEMICOLON);
    if(tokenList[index].token_type == PRIVATE){
        index++;
        parsePrivateVars();
    }
    return;
}

void Parser::parsePrivateVars(){
    //cout << "DEBUG: parsing private variables\n";
    currentlyPublic = false;
    expect(COLON);
    parseVarList();
    expect(SEMICOLON);
    return;
}

void Parser::parseStmtList(){
    //cout << "DEBUG: parsing a statement list\n";
    //so if this doesn't start with an ID we've got a problem
    token = tokenList[index];
    if(token.token_type != ID){
        SyntaxError();
    }
    //cool we're here now so this is either going to stmt or scope, check one ahead
    if(Peek(2).token_type == EQUAL){
        parseStmt();
    }
    if(Peek(2).token_type == LBRACE){
        parseScope();
    }

    //there's either another statement list starting here or else we need to return
    //once again, only way to know is check one ahead
    //could go at the bottom of parseStmt but we wrote it this way so fuck you
    if(Peek(2).token_type == EQUAL || Peek(2).token_type == LBRACE){
        parseStmtList();
    }
    return;


}

void Parser::parseStmt(){
    //cout << "DEBUG: parsing a statement\n";
    token = tokenList[index];
    Variable var;
    if(Peek(4).token_type != SEMICOLON){
        SyntaxError();
    }
    //search through the list for each variable name and print out their scopes as directed
    if(token.token_type == ID){
        var = symbolTable.searchList(scopeList[scopeList.size()-1], token.lexeme);
        //we've now made sense of this token so we increment
        index++;
        expect(EQUAL);
        token = tokenList[index];
        if(token.token_type == ID){
            //if we're here then it's safe to print out the full statement
            output += var.printVariable();
            var = symbolTable.searchList(scopeList[scopeList.size()-1], token.lexeme);
            //we've now made sense of this token so we increment
            index++;
            
            //and print out the statement
            output += " = " + var.printVariable() + "\n";
        }
        else{
            //if this isn't a variable there's an issue
            SyntaxError();
        }
    }
    else{
        //if this isn't a variable there's an issue
        SyntaxError();
    }
    expect(SEMICOLON);  //we already checked for the semicolon here, but we have to skip it
    return;
}



int main()
{
    LexicalAnalyzer lexer;
    Parser* parser = new Parser();
    Token token;

    token = lexer.GetToken();
    if(token.token_type != ERROR){  //is this cheating? yup 100%
         parser->tokenList.push_back(token); //add what we just determined to the end of the token list
    }

    //find the list of tokens
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        if(token.token_type != ERROR){  //is this cheating? we'll find out
             parser->tokenList.push_back(token); //add what we just determined to the end of the token list
        }
    }

    //parse that list into actual output
    parser->parseProgram();
    cout << parser->output;
    return 1;
}
