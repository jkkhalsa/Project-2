#include <vector>
#include <iostream>

#include "lexer.h"
#include "parser.h"

using namespace std;

Parser::Parser(){
    lexer = new LexicalAnalyzer();
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

}
