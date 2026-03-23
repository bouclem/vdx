#pragma once
#include "token.h"
#include "ast.h"
#include <vector>

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    Program parse();

private:
    std::vector<Token> tokens;
    size_t pos;

    const Token& cur() const;
    const Token& advance();
    bool check(TokenType t) const;
    Token expect(TokenType t, const std::string& msg);

    NodePtr parseClassDecl();
    NodePtr parseStatement();
    NodePtr parseFnDecl();
    NodePtr parseLetStmt();
    NodePtr parsePrintStmt();
    NodePtr parseReturnStmt();
    NodePtr parseIfStmt();
    NodePtr parseWhileStmt();
    NodePtr parseForStmt();
    NodePtr parseWaitStmt();
    NodePtr parseExprStmt();

    ExprPtr parseExpr();
    ExprPtr parseEquality();
    ExprPtr parseComparison();
    ExprPtr parseAddSub();
    ExprPtr parseMulDiv();
    ExprPtr parseUnary();
    ExprPtr parsePostfix(ExprPtr left);
    ExprPtr parsePrimary();
};
