#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "variable.h"


class Parser{
    public:
    LexicalAnalyzer* lexer;
    std::vector<Token> tokenList;
    VariableList* symbolTable;
    Token token;

    Parser();
    Token Peek(int);

    void parseProgram();
    void parseGlobalVars();
    void parseVarList();
    void parseScope();
    void parsePublicVars();
    void parsePrivateVars();
    void parseStmtList();
    void parseStmt();
    Token expect(TokenType);

    private:
    int index; //how far along in the token list we are
    bool currentlyPublic;
    std::string currentScope;

    void SyntaxError();
};

#endif