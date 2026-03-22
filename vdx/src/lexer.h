#pragma once
#include "token.h"
#include <string>
#include <vector>

class Lexer {
public:
    explicit Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    std::string src;
    size_t pos;
    int line;
    int col;

    char cur() const;
    char peek() const;
    void advance();
    void skipWhitespace();
    void skipComment();
    Token readString();
    Token readNumber();
    Token readWord();
};
