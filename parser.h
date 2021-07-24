#include "lexer.h"


class Parser{
    public:
    LexicalAnalyzer* lexer;
    std::vector<Token> tokenList;

    Parser();
    Token Peek(int);


    private:
    int index; //how far along in the token list we are

    void SyntaxError();
    Token expect(TokenType);
    void parseProgram();
    void parseGlobalVars();
    void parseVarList();
    void parseScope();
    void parsePublicVars();
    void parsePrivateVars();
    void parseStmtList();
    void parseStmt();
};