#include "lexer.h"
#include <stdexcept>

Lexer::Lexer(const std::string& source) : src(source), pos(0), line(1), col(1) {}

char Lexer::cur() const { return pos < src.size() ? src[pos] : '\0'; }
char Lexer::peek() const { return (pos + 1) < src.size() ? src[pos + 1] : '\0'; }

void Lexer::advance() {
    if (cur() == '\n') { line++; col = 1; } else { col++; }
    pos++;
}

void Lexer::skipWhitespace() {
    while (pos < src.size() && (cur() == ' ' || cur() == '\t' || cur() == '\n' || cur() == '\r'))
        advance();
}

void Lexer::skipComment() {
    if (cur() == '/' && peek() == '/') {
        while (pos < src.size() && cur() != '\n') advance();
    }
}

Token Lexer::readString() {
    int sl = line, sc = col;
    advance(); // skip "
    std::string val;
    while (pos < src.size() && cur() != '"') {
        if (cur() == '\\') {
            advance();
            if (cur() == 'n') val += '\n';
            else if (cur() == 't') val += '\t';
            else if (cur() == '\\') val += '\\';
            else if (cur() == '"') val += '"';
            else val += cur();
        } else {
            val += cur();
        }
        advance();
    }
    if (pos >= src.size())
        throw std::runtime_error("Unterminated string at line " + std::to_string(sl));
    advance(); // skip closing "
    return Token(TokenType::STRING, val, sl, sc);
}

Token Lexer::readNumber() {
    int sl = line, sc = col;
    std::string val;
    while (pos < src.size() && isdigit(cur())) { val += cur(); advance(); }
    return Token(TokenType::INTEGER, val, sl, sc);
}

Token Lexer::readWord() {
    int sl = line, sc = col;
    std::string val;
    while (pos < src.size() && (isalnum(cur()) || cur() == '_')) { val += cur(); advance(); }
    if (val == "class") return Token(TokenType::KW_CLASS, val, sl, sc);
    if (val == "print") return Token(TokenType::KW_PRINT, val, sl, sc);
    if (val == "let") return Token(TokenType::KW_LET, val, sl, sc);
    if (val == "fn") return Token(TokenType::KW_FN, val, sl, sc);
    if (val == "return") return Token(TokenType::KW_RETURN, val, sl, sc);
    if (val == "if") return Token(TokenType::KW_IF, val, sl, sc);
    if (val == "else") return Token(TokenType::KW_ELSE, val, sl, sc);
    if (val == "elif") return Token(TokenType::KW_ELIF, val, sl, sc);
    if (val == "this") return Token(TokenType::KW_THIS, val, sl, sc);
    if (val == "while") return Token(TokenType::KW_WHILE, val, sl, sc);
    if (val == "wait") return Token(TokenType::KW_WAIT, val, sl, sc);
    return Token(TokenType::IDENTIFIER, val, sl, sc);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (pos < src.size()) {
        skipWhitespace();
        if (pos >= src.size()) break;
        if (cur() == '/' && peek() == '/') { skipComment(); continue; }
        if (cur() == '"') { tokens.push_back(readString()); continue; }
        if (cur() == '@') {
            // Check for @unsafe annotation
            int sl = line, sc = col;
            advance(); // skip '@'
            if (pos < src.size() && (isalpha(cur()) || cur() == '_')) {
                std::string word;
                while (pos < src.size() && (isalnum(cur()) || cur() == '_')) {
                    word += cur();
                    advance();
                }
                if (word == "unsafe") {
                    tokens.push_back(Token(TokenType::KW_AT_UNSAFE, "@unsafe", sl, sc));
                    continue;
                }
                throw std::runtime_error(std::string("Unknown annotation '@") + word +
                    "' at line " + std::to_string(sl) + ":" + std::to_string(sc));
            }
            throw std::runtime_error(std::string("Unexpected '@' at line ") +
                std::to_string(sl) + ":" + std::to_string(sc));
        }
        if (isdigit(cur())) { tokens.push_back(readNumber()); continue; }
        if (isalpha(cur()) || cur() == '_') { tokens.push_back(readWord()); continue; }

        int sl = line, sc = col;
        char c = cur();
        advance();

        // Two-character tokens
        if (c == '=' && cur() == '=') { advance(); tokens.push_back(Token(TokenType::EQEQ, "==", sl, sc)); continue; }
        if (c == '!' && cur() == '=') { advance(); tokens.push_back(Token(TokenType::BANGEQ, "!=", sl, sc)); continue; }
        if (c == '<' && cur() == '=') { advance(); tokens.push_back(Token(TokenType::LTEQ, "<=", sl, sc)); continue; }
        if (c == '>' && cur() == '=') { advance(); tokens.push_back(Token(TokenType::GTEQ, ">=", sl, sc)); continue; }

        switch (c) {
            case '{': tokens.push_back(Token(TokenType::LBRACE, "{", sl, sc)); break;
            case '}': tokens.push_back(Token(TokenType::RBRACE, "}", sl, sc)); break;
            case '(': tokens.push_back(Token(TokenType::LPAREN, "(", sl, sc)); break;
            case ')': tokens.push_back(Token(TokenType::RPAREN, ")", sl, sc)); break;
            case ';': tokens.push_back(Token(TokenType::SEMICOLON, ";", sl, sc)); break;
            case ',': tokens.push_back(Token(TokenType::COMMA, ",", sl, sc)); break;
            case '=': tokens.push_back(Token(TokenType::EQUALS, "=", sl, sc)); break;
            case '+': tokens.push_back(Token(TokenType::PLUS, "+", sl, sc)); break;
            case '-': tokens.push_back(Token(TokenType::MINUS, "-", sl, sc)); break;
            case '*': tokens.push_back(Token(TokenType::STAR, "*", sl, sc)); break;
            case '/': tokens.push_back(Token(TokenType::SLASH, "/", sl, sc)); break;
            case '<': tokens.push_back(Token(TokenType::LT, "<", sl, sc)); break;
            case '>': tokens.push_back(Token(TokenType::GT, ">", sl, sc)); break;
            case '.': tokens.push_back(Token(TokenType::DOT, ".", sl, sc)); break;
            case '[': tokens.push_back(Token(TokenType::LBRACKET, "[", sl, sc)); break;
            case ']': tokens.push_back(Token(TokenType::RBRACKET, "]", sl, sc)); break;
            default:
                throw std::runtime_error(std::string("Unexpected character '") + c +
                    "' at line " + std::to_string(sl) + ":" + std::to_string(sc));
        }
    }
    tokens.push_back(Token(TokenType::EOF_TOKEN, "", line, col));
    return tokens;
}
