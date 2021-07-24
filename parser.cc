#include <vector>

#include "lexer.h"
#include "parser.h"

using namespace std;






int main()
{
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        lexer.tokenList.push_back(token); //add what we just determined to the end of the token list
        token.Print();
    }
}
