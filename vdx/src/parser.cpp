#include "parser.h"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), pos(0) {}

const Token& Parser::cur() const { return tokens[pos]; }

const Token& Parser::advance() {
    const Token& t = tokens[pos];
    if (pos < tokens.size() - 1) pos++;
    return t;
}

bool Parser::check(TokenType t) const { return cur().type == t; }

Token Parser::expect(TokenType t, const std::string& msg) {
    if (check(t)) return advance();
    throw std::runtime_error("[VDX] " + msg + " at line " +
        std::to_string(cur().line) + " (got '" + cur().value + "')");
}

Program Parser::parse() {
    Program prog;
    while (!check(TokenType::EOF_TOKEN)) {
        prog.declarations.push_back(parseClassDecl());
    }
    return prog;
}

NodePtr Parser::parseClassDecl() {
    expect(TokenType::KW_CLASS, "Expected 'class'");
    auto cls = std::make_shared<ClassDecl>();
    cls->name = expect(TokenType::IDENTIFIER, "Expected class name").value;
    expect(TokenType::LBRACE, "Expected '{'");
    while (!check(TokenType::RBRACE) && !check(TokenType::EOF_TOKEN)) {
        cls->body.push_back(parseStatement());
    }
    expect(TokenType::RBRACE, "Expected '}'");
    return cls;
}

NodePtr Parser::parseStatement() {
    if (check(TokenType::KW_FN)) return parseFnDecl();
    if (check(TokenType::KW_LET)) return parseLetStmt();
    if (check(TokenType::KW_PRINT)) return parsePrintStmt();
    if (check(TokenType::KW_RETURN)) return parseReturnStmt();
    if (check(TokenType::KW_IF)) return parseIfStmt();
    if (check(TokenType::KW_WHILE)) return parseWhileStmt();
    if (check(TokenType::KW_AT_UNSAFE)) {
        advance(); // consume @unsafe
        if (!check(TokenType::KW_WHILE)) {
            throw std::runtime_error("[VDX] @unsafe can only be used before 'while' at line " +
                std::to_string(cur().line));
        }
        auto node = parseWhileStmt();
        auto whileStmt = std::dynamic_pointer_cast<WhileStmt>(node);
        whileStmt->isUnsafe = true;
        return node;
    }
    if (check(TokenType::KW_WAIT)) return parseWaitStmt();
    // assignment: identifier = expr;
    if (check(TokenType::IDENTIFIER) && pos + 1 < tokens.size() && tokens[pos + 1].type == TokenType::EQUALS) {
        auto stmt = std::make_shared<AssignStmt>();
        stmt->name = advance().value;
        advance(); // skip '='
        stmt->value = parseExpr();
        expect(TokenType::SEMICOLON, "Expected ';'");
        return stmt;
    }
    // expression statement (e.g., function call)
    return parseExprStmt();
}

NodePtr Parser::parseFnDecl() {
    expect(TokenType::KW_FN, "Expected 'fn'");
    auto fn = std::make_shared<FnDecl>();
    fn->name = expect(TokenType::IDENTIFIER, "Expected function name").value;
    expect(TokenType::LPAREN, "Expected '('");
    if (!check(TokenType::RPAREN)) {
        fn->params.push_back(expect(TokenType::IDENTIFIER, "Expected parameter name").value);
        while (check(TokenType::COMMA)) {
            advance();
            fn->params.push_back(expect(TokenType::IDENTIFIER, "Expected parameter name").value);
        }
    }
    expect(TokenType::RPAREN, "Expected ')'");
    expect(TokenType::LBRACE, "Expected '{'");
    while (!check(TokenType::RBRACE) && !check(TokenType::EOF_TOKEN)) {
        fn->body.push_back(parseStatement());
    }
    expect(TokenType::RBRACE, "Expected '}'");
    return fn;
}

NodePtr Parser::parseLetStmt() {
    expect(TokenType::KW_LET, "Expected 'let'");
    auto stmt = std::make_shared<LetStmt>();
    stmt->name = expect(TokenType::IDENTIFIER, "Expected variable name").value;
    expect(TokenType::EQUALS, "Expected '='");
    stmt->value = parseExpr();
    expect(TokenType::SEMICOLON, "Expected ';'");
    return stmt;
}

NodePtr Parser::parsePrintStmt() {
    expect(TokenType::KW_PRINT, "Expected 'print'");
    expect(TokenType::LPAREN, "Expected '('");
    auto stmt = std::make_shared<PrintStmt>();
    if (!check(TokenType::RPAREN)) {
        stmt->args.push_back(parseExpr());
        while (check(TokenType::COMMA)) {
            advance();
            stmt->args.push_back(parseExpr());
        }
    }
    expect(TokenType::RPAREN, "Expected ')'");
    expect(TokenType::SEMICOLON, "Expected ';'");
    return stmt;
}

NodePtr Parser::parseReturnStmt() {
    expect(TokenType::KW_RETURN, "Expected 'return'");
    auto stmt = std::make_shared<ReturnStmt>();
    if (!check(TokenType::SEMICOLON)) {
        stmt->value = parseExpr();
    }
    expect(TokenType::SEMICOLON, "Expected ';'");
    return stmt;
}

NodePtr Parser::parseIfStmt() {
    expect(TokenType::KW_IF, "Expected 'if'");
    auto stmt = std::make_shared<IfStmt>();
    expect(TokenType::LPAREN, "Expected '('");
    stmt->condition = parseExpr();
    expect(TokenType::RPAREN, "Expected ')'");
    expect(TokenType::LBRACE, "Expected '{'");
    while (!check(TokenType::RBRACE) && !check(TokenType::EOF_TOKEN)) {
        stmt->thenBody.push_back(parseStatement());
    }
    expect(TokenType::RBRACE, "Expected '}'");

    while (check(TokenType::KW_ELIF)) {
        advance();
        ElifClause elif;
        expect(TokenType::LPAREN, "Expected '('");
        elif.condition = parseExpr();
        expect(TokenType::RPAREN, "Expected ')'");
        expect(TokenType::LBRACE, "Expected '{'");
        while (!check(TokenType::RBRACE) && !check(TokenType::EOF_TOKEN)) {
            elif.body.push_back(parseStatement());
        }
        expect(TokenType::RBRACE, "Expected '}'");
        stmt->elifs.push_back(std::move(elif));
    }

    if (check(TokenType::KW_ELSE)) {
        advance();
        expect(TokenType::LBRACE, "Expected '{'");
        while (!check(TokenType::RBRACE) && !check(TokenType::EOF_TOKEN)) {
            stmt->elseBody.push_back(parseStatement());
        }
        expect(TokenType::RBRACE, "Expected '}'");
    }

    return stmt;
}

NodePtr Parser::parseWhileStmt() {
    expect(TokenType::KW_WHILE, "Expected 'while'");
    auto stmt = std::make_shared<WhileStmt>();
    expect(TokenType::LPAREN, "Expected '('");
    stmt->condition = parseExpr();
    expect(TokenType::RPAREN, "Expected ')'");
    expect(TokenType::LBRACE, "Expected '{'");
    while (!check(TokenType::RBRACE) && !check(TokenType::EOF_TOKEN)) {
        stmt->body.push_back(parseStatement());
    }
    expect(TokenType::RBRACE, "Expected '}'");
    return stmt;
}

NodePtr Parser::parseWaitStmt() {
    expect(TokenType::KW_WAIT, "Expected 'wait'");
    auto stmt = std::make_shared<WaitStmt>();
    expect(TokenType::LPAREN, "Expected '('");
    stmt->duration = parseExpr();
    expect(TokenType::RPAREN, "Expected ')'");
    expect(TokenType::SEMICOLON, "Expected ';'");
    return stmt;
}

NodePtr Parser::parseExprStmt() {
    auto stmt = std::make_shared<ExprStmt>();
    stmt->expr = parseExpr();
    expect(TokenType::SEMICOLON, "Expected ';'");
    return stmt;
}

// ── Expression parsing with precedence ──

ExprPtr Parser::parseExpr() {
    return parseEquality();
}

ExprPtr Parser::parseEquality() {
    auto left = parseComparison();
    while (check(TokenType::EQEQ) || check(TokenType::BANGEQ)) {
        std::string op = advance().value;
        auto right = parseComparison();
        auto bin = std::make_shared<BinaryExpr>();
        bin->left = left;
        bin->op = op;
        bin->right = right;
        left = bin;
    }
    return left;
}

ExprPtr Parser::parseComparison() {
    auto left = parseAddSub();
    while (check(TokenType::LT) || check(TokenType::GT) ||
           check(TokenType::LTEQ) || check(TokenType::GTEQ)) {
        std::string op = advance().value;
        auto right = parseAddSub();
        auto bin = std::make_shared<BinaryExpr>();
        bin->left = left;
        bin->op = op;
        bin->right = right;
        left = bin;
    }
    return left;
}

ExprPtr Parser::parseAddSub() {
    auto left = parseMulDiv();
    while (check(TokenType::PLUS) || check(TokenType::MINUS)) {
        std::string op = advance().value;
        auto right = parseMulDiv();
        auto bin = std::make_shared<BinaryExpr>();
        bin->left = left;
        bin->op = op;
        bin->right = right;
        left = bin;
    }
    return left;
}

ExprPtr Parser::parseMulDiv() {
    auto left = parsePrimary();
    while (check(TokenType::STAR) || check(TokenType::SLASH)) {
        std::string op = advance().value;
        auto right = parsePrimary();
        auto bin = std::make_shared<BinaryExpr>();
        bin->left = left;
        bin->op = op;
        bin->right = right;
        left = bin;
    }
    return left;
}

ExprPtr Parser::parsePrimary() {
    if (check(TokenType::STRING)) {
        auto lit = std::make_shared<StringLiteral>();
        lit->value = advance().value;
        return lit;
    }
    if (check(TokenType::INTEGER)) {
        auto lit = std::make_shared<IntLiteral>();
        lit->value = std::stoi(advance().value);
        return lit;
    }
    if (check(TokenType::IDENTIFIER)) {
        std::string name = advance().value;
        // Check for function call: identifier followed by '('
        if (check(TokenType::LPAREN)) {
            advance(); // skip '('
            auto call = std::make_shared<CallExpr>();
            call->name = name;
            if (!check(TokenType::RPAREN)) {
                call->args.push_back(parseExpr());
                while (check(TokenType::COMMA)) {
                    advance();
                    call->args.push_back(parseExpr());
                }
            }
            expect(TokenType::RPAREN, "Expected ')'");
            return call;
        }
        auto id = std::make_shared<IdentifierExpr>();
        id->name = name;
        return id;
    }
    if (check(TokenType::LPAREN)) {
        advance(); // skip '('
        auto expr = parseExpr();
        expect(TokenType::RPAREN, "Expected ')'");
        return expr;
    }
    if (check(TokenType::KW_THIS)) {
        advance();
        expect(TokenType::DOT, "Expected '.' after 'this'");
        auto te = std::make_shared<ThisExpr>();
        te->field = expect(TokenType::IDENTIFIER, "Expected field name after 'this.'").value;
        return te;
    }
    throw std::runtime_error("[VDX] Expected expression at line " +
        std::to_string(cur().line) + " (got '" + cur().value + "')");
}
