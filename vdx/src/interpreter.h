#pragma once
#include "ast.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>

struct Value {
    enum Type { STRING, INT, BOOL, VOID, ARRAY };
    Type type;
    std::string strVal;
    int intVal;
    bool boolVal;
    std::vector<Value> arrVal;

    Value() : type(VOID), intVal(0), boolVal(false) {}
    static Value makeString(const std::string& s) { Value v; v.type = STRING; v.strVal = s; return v; }
    static Value makeInt(int i) { Value v; v.type = INT; v.intVal = i; return v; }
    static Value makeBool(bool b) { Value v; v.type = BOOL; v.boolVal = b; return v; }
    static Value makeVoid() { return Value(); }
    static Value makeArray(const std::vector<Value>& elems) { Value v; v.type = ARRAY; v.arrVal = elems; return v; }

    std::string toString() const {
        switch (type) {
            case STRING: return strVal;
            case INT: return std::to_string(intVal);
            case BOOL: return boolVal ? "true" : "false";
            case VOID: return "void";
            case ARRAY: {
                std::string s = "[";
                for (size_t i = 0; i < arrVal.size(); i++) {
                    if (i > 0) s += ", ";
                    if (arrVal[i].type == STRING) s += "\"" + arrVal[i].toString() + "\"";
                    else s += arrVal[i].toString();
                }
                s += "]";
                return s;
            }
        }
        return "";
    }
};

struct ReturnException {
    Value value;
};

class Interpreter {
public:
    void run(const Program& program);

private:
    // Scope stack: each scope is a map of variable names to values
    std::vector<std::unordered_map<std::string, Value>> scopes;
    std::unordered_map<std::string, const FnDecl*> functions;

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
    void execWait(const WaitStmt* stmt);
    Value execCall(const CallExpr* call);
    Value evalExpr(const Expr* expr);
    Value evalBinary(const BinaryExpr* expr);
    bool isTruthy(const Value& v);
};
