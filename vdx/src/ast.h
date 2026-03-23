#pragma once
#include <string>
#include <vector>
#include <memory>

// ── AST Nodes (v0.0.8: types + new/objects + for loop) ──

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

struct FloatLiteral : Expr {
    double value;
};

struct BoolLiteral : Expr {
    bool value;
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

// new ClassName()
struct NewExpr : Expr {
    std::string className;
};

// obj.field (read)
struct DotExpr : Expr {
    ExprPtr object;
    std::string field;
};

// obj.method(args)
struct DotCallExpr : Expr {
    ExprPtr object;
    std::string method;
    std::vector<ExprPtr> args;
};

// ── Statements ──

// let x = expr;  OR  let x: type = expr;
struct LetStmt : Node {
    std::string name;
    std::string typeAnnotation; // "" if no annotation
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
struct WhileStmt : Node {
    ExprPtr condition;
    std::vector<NodePtr> body;
    bool isUnsafe = false;
};

// for (init; cond; update) { body }
struct ForStmt : Node {
    NodePtr init;       // LetStmt or AssignStmt
    ExprPtr condition;
    NodePtr update;     // AssignStmt
    std::vector<NodePtr> body;
    bool isUnsafe = false;
};

// for (name in expr) { body }
struct ForInStmt : Node {
    std::string varName;
    ExprPtr iterable;
    std::vector<NodePtr> body;
};

// wait(expr);
struct WaitStmt : Node {
    ExprPtr duration;
};

// name = expr;
struct AssignStmt : Node {
    std::string name;
    ExprPtr value;
};

// arr[index] = expr;
struct IndexAssignStmt : Node {
    std::string name;
    ExprPtr index;
    ExprPtr value;
};

// obj.field = expr;
struct DotAssignStmt : Node {
    ExprPtr object;
    std::string field;
    ExprPtr value;
};

// expression as statement
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
