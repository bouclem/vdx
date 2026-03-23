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
    int ln = cur().line;
    expect(TokenType::KW_CLASS, "Expected 'class'");
    auto cls = std::make_shared<ClassDecl>();
    cls->line = ln;
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
    if (check(TokenType::KW_FOR)) return parseForStmt();
    if (check(TokenType::KW_AT_UNSAFE)) {
        advance(); // consume @unsafe
        if (check(TokenType::KW_WHILE)) {
            auto node = parseWhileStmt();
            auto whileStmt = std::dynamic_pointer_cast<WhileStmt>(node);
            whileStmt->isUnsafe = true;
            return node;
        }
        if (check(TokenType::KW_FOR)) {
            auto node = parseForStmt();
            auto forStmt = std::dynamic_pointer_cast<ForStmt>(node);
            if (forStmt) {
                forStmt->isUnsafe = true;
                return node;
            }
            // for-in doesn't need @unsafe (no speed check), but allow it
            return node;
        }
        throw std::runtime_error("[VDX] @unsafe can only be used before 'while' or 'for' at line " +
            std::to_string(cur().line));
    }
    if (check(TokenType::KW_WAIT)) return parseWaitStmt();
    // assignment, index assignment, or dot assignment
    if (check(TokenType::IDENTIFIER) && pos + 1 < tokens.size()) {
        if (tokens[pos + 1].type == TokenType::LBRACKET) {
            size_t saved = pos;
            int ln = cur().line;
            std::string name = advance().value;
            advance(); // skip '['
            auto idx = parseExpr();
            expect(TokenType::RBRACKET, "Expected ']'");
            if (check(TokenType::EQUALS)) {
                advance(); // skip '='
                auto val = parseExpr();
                expect(TokenType::SEMICOLON, "Expected ';'");
                auto stmt = std::make_shared<IndexAssignStmt>();
                stmt->line = ln;
                stmt->name = name;
                stmt->index = idx;
                stmt->value = val;
                return stmt;
            }
            pos = saved;
        } else if (tokens[pos + 1].type == TokenType::DOT) {
            // Could be dot assignment: obj.field = value;
            // Or dot call / dot access as expression statement
            size_t saved = pos;
            int ln = cur().line;
            // Parse the left side as an expression
            auto expr = parseExpr();
            if (check(TokenType::EQUALS)) {
                // dot assignment: obj.field = value;
                advance(); // skip '='
                auto dotExpr = std::dynamic_pointer_cast<DotExpr>(expr);
                if (!dotExpr) {
                    throw std::runtime_error("[VDX] Invalid assignment target at line " + std::to_string(ln));
                }
                auto val = parseExpr();
                expect(TokenType::SEMICOLON, "Expected ';'");
                auto stmt = std::make_shared<DotAssignStmt>();
                stmt->line = ln;
                stmt->object = dotExpr->object;
                stmt->field = dotExpr->field;
                stmt->value = val;
                return stmt;
            }
            // Otherwise it's an expression statement (e.g., obj.method();)
            expect(TokenType::SEMICOLON, "Expected ';'");
            auto stmt = std::make_shared<ExprStmt>();
            stmt->line = ln;
            stmt->expr = expr;
            return stmt;
        } else if (tokens[pos + 1].type == TokenType::EQUALS) {
            int ln = cur().line;
            auto stmt = std::make_shared<AssignStmt>();
            stmt->line = ln;
            stmt->name = advance().value;
            advance(); // skip '='
            stmt->value = parseExpr();
            expect(TokenType::SEMICOLON, "Expected ';'");
            return stmt;
        }
    }
    return parseExprStmt();
}

NodePtr Parser::parseFnDecl() {
    int ln = cur().line;
    expect(TokenType::KW_FN, "Expected 'fn'");
    auto fn = std::make_shared<FnDecl>();
    fn->line = ln;
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
    int ln = cur().line;
    expect(TokenType::KW_LET, "Expected 'let'");
    auto stmt = std::make_shared<LetStmt>();
    stmt->line = ln;
    stmt->name = expect(TokenType::IDENTIFIER, "Expected variable name").value;
    // Optional type annotation: let x: int = ...
    if (check(TokenType::COLON)) {
        advance(); // skip ':'
        stmt->typeAnnotation = expect(TokenType::IDENTIFIER, "Expected type name after ':'").value;
    }
    expect(TokenType::EQUALS, "Expected '='");
    stmt->value = parseExpr();
    expect(TokenType::SEMICOLON, "Expected ';'");
    return stmt;
}

NodePtr Parser::parsePrintStmt() {
    int ln = cur().line;
    expect(TokenType::KW_PRINT, "Expected 'print'");
    expect(TokenType::LPAREN, "Expected '('");
    auto stmt = std::make_shared<PrintStmt>();
    stmt->line = ln;
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
    int ln = cur().line;
    expect(TokenType::KW_RETURN, "Expected 'return'");
    auto stmt = std::make_shared<ReturnStmt>();
    stmt->line = ln;
    if (!check(TokenType::SEMICOLON)) {
        stmt->value = parseExpr();
    }
    expect(TokenType::SEMICOLON, "Expected ';'");
    return stmt;
}

NodePtr Parser::parseIfStmt() {
    int ln = cur().line;
    expect(TokenType::KW_IF, "Expected 'if'");
    auto stmt = std::make_shared<IfStmt>();
    stmt->line = ln;
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
    int ln = cur().line;
    expect(TokenType::KW_WHILE, "Expected 'while'");
    auto stmt = std::make_shared<WhileStmt>();
    stmt->line = ln;
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

NodePtr Parser::parseForStmt() {
    int ln = cur().line;
    expect(TokenType::KW_FOR, "Expected 'for'");
    expect(TokenType::LPAREN, "Expected '('");

    // Check for for-in: for (name in expr)
    if (check(TokenType::IDENTIFIER) && pos + 1 < tokens.size() && tokens[pos + 1].type == TokenType::KW_IN) {
        auto stmt = std::make_shared<ForInStmt>();
        stmt->line = ln;
        stmt->varName = advance().value; // variable name
        advance(); // skip 'in'
        stmt->iterable = parseExpr();
        expect(TokenType::RPAREN, "Expected ')'");
        expect(TokenType::LBRACE, "Expected '{'");
        while (!check(TokenType::RBRACE) && !check(TokenType::EOF_TOKEN)) {
            stmt->body.push_back(parseStatement());
        }
        expect(TokenType::RBRACE, "Expected '}'");
        return stmt;
    }

    // C-style for: for (init; cond; update) { body }
    auto stmt = std::make_shared<ForStmt>();
    stmt->line = ln;

    // init: let or assignment
    if (check(TokenType::KW_LET)) {
        stmt->init = parseLetStmt(); // includes semicolon
    } else {
        // assignment: name = expr;
        int aln = cur().line;
        auto assign = std::make_shared<AssignStmt>();
        assign->line = aln;
        assign->name = expect(TokenType::IDENTIFIER, "Expected variable name in for init").value;
        expect(TokenType::EQUALS, "Expected '='");
        assign->value = parseExpr();
        expect(TokenType::SEMICOLON, "Expected ';'");
        stmt->init = assign;
    }

    stmt->condition = parseExpr();
    expect(TokenType::SEMICOLON, "Expected ';' after for condition");

    // update: assignment (no semicolon, followed by ')')
    {
        int aln = cur().line;
        auto assign = std::make_shared<AssignStmt>();
        assign->line = aln;
        assign->name = expect(TokenType::IDENTIFIER, "Expected variable name in for update").value;
        expect(TokenType::EQUALS, "Expected '='");
        assign->value = parseExpr();
        stmt->update = assign;
    }

    expect(TokenType::RPAREN, "Expected ')'");
    expect(TokenType::LBRACE, "Expected '{'");
    while (!check(TokenType::RBRACE) && !check(TokenType::EOF_TOKEN)) {
        stmt->body.push_back(parseStatement());
    }
    expect(TokenType::RBRACE, "Expected '}'");
    return stmt;
}

NodePtr Parser::parseWaitStmt() {
    int ln = cur().line;
    expect(TokenType::KW_WAIT, "Expected 'wait'");
    auto stmt = std::make_shared<WaitStmt>();
    stmt->line = ln;
    expect(TokenType::LPAREN, "Expected '('");
    stmt->duration = parseExpr();
    expect(TokenType::RPAREN, "Expected ')'");
    expect(TokenType::SEMICOLON, "Expected ';'");
    return stmt;
}

NodePtr Parser::parseExprStmt() {
    int ln = cur().line;
    auto stmt = std::make_shared<ExprStmt>();
    stmt->line = ln;
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
        int ln = cur().line;
        std::string op = advance().value;
        auto right = parseComparison();
        auto bin = std::make_shared<BinaryExpr>();
        bin->line = ln;
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
        int ln = cur().line;
        std::string op = advance().value;
        auto right = parseAddSub();
        auto bin = std::make_shared<BinaryExpr>();
        bin->line = ln;
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
        int ln = cur().line;
        std::string op = advance().value;
        auto right = parseMulDiv();
        auto bin = std::make_shared<BinaryExpr>();
        bin->line = ln;
        bin->left = left;
        bin->op = op;
        bin->right = right;
        left = bin;
    }
    return left;
}

ExprPtr Parser::parseMulDiv() {
    auto left = parseUnary();
    while (check(TokenType::STAR) || check(TokenType::SLASH)) {
        int ln = cur().line;
        std::string op = advance().value;
        auto right = parseUnary();
        auto bin = std::make_shared<BinaryExpr>();
        bin->line = ln;
        bin->left = left;
        bin->op = op;
        bin->right = right;
        left = bin;
    }
    return left;
}

ExprPtr Parser::parseUnary() {
    auto expr = parsePrimary();
    return parsePostfix(expr);
}

ExprPtr Parser::parsePostfix(ExprPtr left) {
    while (true) {
        if (check(TokenType::DOT)) {
            int ln = cur().line;
            advance(); // skip '.'
            std::string field = expect(TokenType::IDENTIFIER, "Expected field name after '.'").value;
            // Check if it's a method call: obj.method(args)
            if (check(TokenType::LPAREN)) {
                advance(); // skip '('
                auto call = std::make_shared<DotCallExpr>();
                call->line = ln;
                call->object = left;
                call->method = field;
                if (!check(TokenType::RPAREN)) {
                    call->args.push_back(parseExpr());
                    while (check(TokenType::COMMA)) {
                        advance();
                        call->args.push_back(parseExpr());
                    }
                }
                expect(TokenType::RPAREN, "Expected ')'");
                left = call;
            } else {
                auto dot = std::make_shared<DotExpr>();
                dot->line = ln;
                dot->object = left;
                dot->field = field;
                left = dot;
            }
        } else if (check(TokenType::LBRACKET)) {
            int ln = cur().line;
            advance(); // skip '['
            auto idx = std::make_shared<IndexExpr>();
            idx->line = ln;
            idx->object = left;
            idx->index = parseExpr();
            expect(TokenType::RBRACKET, "Expected ']'");
            left = idx;
        } else {
            break;
        }
    }
    return left;
}

ExprPtr Parser::parsePrimary() {
    if (check(TokenType::STRING)) {
        int ln = cur().line;
        auto lit = std::make_shared<StringLiteral>();
        lit->line = ln;
        lit->value = advance().value;
        return lit;
    }
    if (check(TokenType::INTEGER)) {
        int ln = cur().line;
        auto lit = std::make_shared<IntLiteral>();
        lit->line = ln;
        lit->value = std::stoi(advance().value);
        return lit;
    }
    if (check(TokenType::FLOAT)) {
        int ln = cur().line;
        auto lit = std::make_shared<FloatLiteral>();
        lit->line = ln;
        lit->value = std::stod(advance().value);
        return lit;
    }
    if (check(TokenType::KW_TRUE)) {
        int ln = cur().line;
        advance();
        auto lit = std::make_shared<BoolLiteral>();
        lit->line = ln;
        lit->value = true;
        return lit;
    }
    if (check(TokenType::KW_FALSE)) {
        int ln = cur().line;
        advance();
        auto lit = std::make_shared<BoolLiteral>();
        lit->line = ln;
        lit->value = false;
        return lit;
    }
    // Array literal: [expr, expr, ...]
    if (check(TokenType::LBRACKET)) {
        int ln = cur().line;
        advance(); // skip '['
        auto arr = std::make_shared<ArrayLiteral>();
        arr->line = ln;
        if (!check(TokenType::RBRACKET)) {
            arr->elements.push_back(parseExpr());
            while (check(TokenType::COMMA)) {
                advance();
                arr->elements.push_back(parseExpr());
            }
        }
        expect(TokenType::RBRACKET, "Expected ']'");
        return arr;
    }
    // new ClassName()
    if (check(TokenType::KW_NEW)) {
        int ln = cur().line;
        advance(); // skip 'new'
        auto expr = std::make_shared<NewExpr>();
        expr->line = ln;
        expr->className = expect(TokenType::IDENTIFIER, "Expected class name after 'new'").value;
        expect(TokenType::LPAREN, "Expected '(' after class name");
        expect(TokenType::RPAREN, "Expected ')' after 'new ClassName('");
        return expr;
    }
    if (check(TokenType::IDENTIFIER)) {
        int ln = cur().line;
        std::string name = advance().value;
        // Function call: identifier(...)
        if (check(TokenType::LPAREN)) {
            advance(); // skip '('
            auto call = std::make_shared<CallExpr>();
            call->line = ln;
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
        id->line = ln;
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
        int ln = cur().line;
        advance();
        expect(TokenType::DOT, "Expected '.' after 'this'");
        auto te = std::make_shared<ThisExpr>();
        te->line = ln;
        te->field = expect(TokenType::IDENTIFIER, "Expected field name after 'this.'").value;
        return te;
    }
    throw std::runtime_error("[VDX] Expected expression at line " +
        std::to_string(cur().line) + " (got '" + cur().value + "')");
}
