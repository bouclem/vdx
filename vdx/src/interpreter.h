#pragma once
#include "ast.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <stdexcept>

struct ObjectData;

struct Value {
    enum Type { STRING, INT, FLOAT, BOOL, VOID, ARRAY, OBJECT };
    Type type;
    std::string strVal;
    int intVal;
    double floatVal;
    bool boolVal;
    std::vector<Value> arrVal;
    std::shared_ptr<ObjectData> objVal;

    Value() : type(VOID), intVal(0), floatVal(0.0), boolVal(false) {}
    static Value makeString(const std::string& s) { Value v; v.type = STRING; v.strVal = s; return v; }
    static Value makeInt(int i) { Value v; v.type = INT; v.intVal = i; return v; }
    static Value makeFloat(double f) { Value v; v.type = FLOAT; v.floatVal = f; return v; }
    static Value makeBool(bool b) { Value v; v.type = BOOL; v.boolVal = b; return v; }
    static Value makeVoid() { return Value(); }
    static Value makeArray(const std::vector<Value>& elems) { Value v; v.type = ARRAY; v.arrVal = elems; return v; }
    static Value makeObject(std::shared_ptr<ObjectData> obj) { Value v; v.type = OBJECT; v.objVal = obj; return v; }

    // Get numeric value as double (for mixed int/float arithmetic)
    double toDouble() const {
        if (type == FLOAT) return floatVal;
        if (type == INT) return (double)intVal;
        return 0.0;
    }

    bool isNumeric() const { return type == INT || type == FLOAT; }

    std::string toString() const;
};

struct ObjectData {
    std::string className;
    std::unordered_map<std::string, Value> fields;
    std::unordered_map<std::string, const FnDecl*> methods;
};

struct ReturnException {
    Value value;
};

class Interpreter {
public:
    void run(const Program& program);
    int currentLine = 0;

private:
    std::vector<std::unordered_map<std::string, Value>> scopes;
    std::unordered_map<std::string, const FnDecl*> functions;
    std::unordered_map<std::string, const ClassDecl*> classDecls;

    void pushScope();
    void popScope();
    Value* lookupVar(const std::string& name);
    void declareVar(const std::string& name, const Value& val);

    void execClass(const ClassDecl* cls);
    void execStatement(const NodePtr& node);
    void execLet(const LetStmt* stmt);
    void execPrint(const PrintStmt* stmt);
    void execReturn(const ReturnStmt* stmt);
    void execIf(const IfStmt* stmt);
    void execWhile(const WhileStmt* stmt);
    void execFor(const ForStmt* stmt);
    void execForIn(const ForInStmt* stmt);
    void execWait(const WaitStmt* stmt);
    Value execCall(const CallExpr* call);
    Value evalExpr(const Expr* expr);
    Value evalBinary(const BinaryExpr* expr);
    Value execNew(const NewExpr* expr);
    bool isTruthy(const Value& v);
    void checkType(const std::string& annotation, const Value& val, int line);
};
