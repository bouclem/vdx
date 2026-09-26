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
    int blockDepth = 0;   // >0 inside if/while/for/fn bodies — fn decls not allowed there
    int loopDepth = 0;    // >0 inside loop bodies — break/continue allowed there
    int exprDepth = 0;    // guards against stack overflow on deeply nested expressions
    static const int MAX_EXPR_DEPTH = 512;

    const Token& cur() const;
    const Token& advance();
    bool check(TokenType t) const;
    Token expect(TokenType t, const std::string& msg);
    NodePtr parseAssignOrExprStmt();

    NodePtr parseClassDecl();
    NodePtr parseImportStmt();
    NodePtr parseStatement();
    NodePtr parseFnDecl();
    NodePtr parseLetStmt();
    NodePtr parseConstStmt();
    NodePtr parseBreakStmt();
    NodePtr parseContinueStmt();
    NodePtr parsePrintStmt();
    NodePtr parseReturnStmt();
    NodePtr parseIfStmt();
    NodePtr parseWhileStmt();
    NodePtr parseForStmt();
    NodePtr parseWaitStmt();

    ExprPtr parseExpr();
    ExprPtr parseOr();
    ExprPtr parseAnd();
    ExprPtr parseEquality();
    ExprPtr parseComparison();
    ExprPtr parseAddSub();
    ExprPtr parseMulDiv();
    ExprPtr parseUnary();
    ExprPtr parsePostfix(ExprPtr left);
    ExprPtr parsePrimary();
};
