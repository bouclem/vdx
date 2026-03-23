#pragma once
#include <string>

enum class TokenType {
    STRING,
    INTEGER,
    FLOAT,
    KW_CLASS,
    KW_PRINT,
    KW_LET,
    KW_FN,
    KW_RETURN,
    KW_IF,
    KW_ELSE,
    KW_ELIF,
    KW_THIS,
    KW_WHILE,
    KW_WAIT,
    KW_AT_UNSAFE,
    KW_NEW,
    KW_FOR,
    KW_IN,
    KW_TRUE,
    KW_FALSE,
    IDENTIFIER,
    LBRACE,
    RBRACE,
    LPAREN,
    RPAREN,
    SEMICOLON,
    COMMA,
    COLON,
    EQUALS,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    EQEQ,
    BANGEQ,
    LT,
    GT,
    LTEQ,
    GTEQ,
    DOT,
    LBRACKET,
    RBRACKET,
    EOF_TOKEN,
    UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int col;

    Token() : type(TokenType::UNKNOWN), line(0), col(0) {}
    Token(TokenType t, const std::string& v, int l, int c)
        : type(t), value(v), line(l), col(c) {}
};
