#pragma once
#include <string>
#include <vector>
#include <memory>

// ── AST Nodes (v0.0.5: class + print + let + fn + operators + if + while + wait) ──

struct Node {
    int line = 0;
    virtual ~Node() = default;
};

using NodePtr = std::shared_ptr<Node>;

// ── Expressions ──

struct Expr : Node {};
using ExprPtr = std::shared_ptr<Expr>;

struct StringLiteral : Expr {
    std::string value;
};

struct IntLiteral : Expr {
    int value;
};

struct IdentifierExpr : Expr {
    std::string name;
};

struct BinaryExpr : Expr {
    ExprPtr left;
    std::string op;
    ExprPtr right;
};

struct CallExpr : Expr {
    std::string name;
    std::vector<ExprPtr> args;
};

struct ThisExpr : Expr {
    std::string field;
};

struct ArrayLiteral : Expr {
    std::vector<ExprPtr> elements;
};

struct IndexExpr : Expr {
    ExprPtr object;
    ExprPtr index;
};

// ── Statements ──

// let x = expr;
struct LetStmt : Node {
    std::string name;
    ExprPtr value;
};

// print(expr, expr);
struct PrintStmt : Node {
    std::vector<ExprPtr> args;
};

// return expr;
struct ReturnStmt : Node {
    ExprPtr value; // nullptr for bare return
};

// elif (condition) { body }
struct ElifClause {
    ExprPtr condition;
    std::vector<NodePtr> body;
};

// if (cond) { body } elif (cond) { body } else { body }
struct IfStmt : Node {
    ExprPtr condition;
    std::vector<NodePtr> thenBody;
    std::vector<ElifClause> elifs;
    std::vector<NodePtr> elseBody;
};

// while (cond) { body }
// @unsafe while (cond) { body } — disables loop speed protection
struct WhileStmt : Node {
    ExprPtr condition;
    std::vector<NodePtr> body;
    bool isUnsafe = false;
};

// wait(expr);
struct WaitStmt : Node {
    ExprPtr duration; // milliseconds
};

// name = expr; (variable reassignment)
struct AssignStmt : Node {
    std::string name;
    ExprPtr value;
};

// arr[index] = expr; (index assignment)
struct IndexAssignStmt : Node {
    std::string name;
    ExprPtr index;
    ExprPtr value;
};

// expression as statement (e.g., myFunc();)
struct ExprStmt : Node {
    ExprPtr expr;
};

// fn name(params) { body }
struct FnDecl : Node {
    std::string name;
    std::vector<std::string> params;
    std::vector<NodePtr> body;
};

// class Foo { ... }
struct ClassDecl : Node {
    std::string name;
    std::vector<NodePtr> body;
};

// Top-level program
struct Program {
    std::vector<NodePtr> declarations;
};
