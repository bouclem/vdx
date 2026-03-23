#include "interpreter.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include <cmath>

// ── Value::toString ──

std::string Value::toString() const {
    switch (type) {
        case STRING: return strVal;
        case INT: return std::to_string(intVal);
        case FLOAT: {
            std::ostringstream oss;
            oss << floatVal;
            std::string s = oss.str();
            // Ensure floats always show a decimal point
            if (s.find('.') == std::string::npos) s += ".0";
            return s;
        }
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
        case OBJECT: {
            if (objVal) return "<" + objVal->className + " object>";
            return "<object>";
        }
    }
    return "";
}

// ── Scope management ──

void Interpreter::pushScope() {
    scopes.emplace_back();
}

void Interpreter::popScope() {
    scopes.pop_back();
}

Value* Interpreter::lookupVar(const std::string& name) {
    for (int i = (int)scopes.size() - 1; i >= 0; i--) {
        auto it = scopes[i].find(name);
        if (it != scopes[i].end()) return &it->second;
    }
    return nullptr;
}

void Interpreter::declareVar(const std::string& name, const Value& val) {
    scopes.back()[name] = val;
}

// ── Type checking ──

void Interpreter::checkType(const std::string& annotation, const Value& val, int line) {
    if (annotation.empty()) return;
    bool ok = false;
    if (annotation == "int") ok = (val.type == Value::INT);
    else if (annotation == "float") ok = (val.type == Value::FLOAT || val.type == Value::INT);
    else if (annotation == "string") ok = (val.type == Value::STRING);
    else if (annotation == "bool") ok = (val.type == Value::BOOL);
    else {
        throw std::runtime_error("[VDX] Unknown type '" + annotation + "' at line " + std::to_string(line));
    }
    if (!ok) {
        std::string got;
        switch (val.type) {
            case Value::INT: got = "int"; break;
            case Value::FLOAT: got = "float"; break;
            case Value::STRING: got = "string"; break;
            case Value::BOOL: got = "bool"; break;
            case Value::VOID: got = "void"; break;
            case Value::ARRAY: got = "array"; break;
            case Value::OBJECT: got = "object"; break;
        }
        throw std::runtime_error("[VDX] Type mismatch: expected '" + annotation +
            "', got '" + got + "' at line " + std::to_string(line));
    }
}

// ── Execution ──

void Interpreter::run(const Program& program) {
    // First pass: register all class declarations for 'new'
    for (auto& decl : program.declarations) {
        auto cls = dynamic_cast<ClassDecl*>(decl.get());
        if (cls) classDecls[cls->name] = cls;
    }
    // Second pass: execute top-level classes
    for (auto& decl : program.declarations) {
        auto cls = dynamic_cast<ClassDecl*>(decl.get());
        if (cls) execClass(cls);
    }
}

void Interpreter::execClass(const ClassDecl* cls) {
    pushScope();
    // First pass: register functions
    for (auto& node : cls->body) {
        if (auto fn = dynamic_cast<FnDecl*>(node.get())) {
            functions[fn->name] = fn;
        }
    }
    // Second pass: execute non-function statements
    for (auto& node : cls->body) {
        if (dynamic_cast<FnDecl*>(node.get())) continue;
        execStatement(node);
    }
    popScope();
}

void Interpreter::execStatement(const NodePtr& node) {
    if (node->line > 0) currentLine = node->line;

    if (auto let = dynamic_cast<LetStmt*>(node.get())) {
        execLet(let);
    } else if (auto print = dynamic_cast<PrintStmt*>(node.get())) {
        execPrint(print);
    } else if (auto ret = dynamic_cast<ReturnStmt*>(node.get())) {
        execReturn(ret);
    } else if (auto ifst = dynamic_cast<IfStmt*>(node.get())) {
        execIf(ifst);
    } else if (auto wh = dynamic_cast<WhileStmt*>(node.get())) {
        execWhile(wh);
    } else if (auto forst = dynamic_cast<ForStmt*>(node.get())) {
        execFor(forst);
    } else if (auto forin = dynamic_cast<ForInStmt*>(node.get())) {
        execForIn(forin);
    } else if (auto wt = dynamic_cast<WaitStmt*>(node.get())) {
        execWait(wt);
    } else if (auto assign = dynamic_cast<AssignStmt*>(node.get())) {
        Value* v = lookupVar(assign->name);
        if (!v) {
            throw std::runtime_error("[VDX] Undefined variable '" + assign->name + "' (use 'let' to declare) at line " + std::to_string(currentLine));
        }
        *v = evalExpr(assign->value.get());
    } else if (auto idxAssign = dynamic_cast<IndexAssignStmt*>(node.get())) {
        Value* v = lookupVar(idxAssign->name);
        if (!v) {
            throw std::runtime_error("[VDX] Undefined variable '" + idxAssign->name + "' at line " + std::to_string(currentLine));
        }
        if (v->type != Value::ARRAY) {
            throw std::runtime_error("[VDX] Cannot index into non-array variable '" + idxAssign->name + "' at line " + std::to_string(currentLine));
        }
        Value idx = evalExpr(idxAssign->index.get());
        if (idx.type != Value::INT) {
            throw std::runtime_error("[VDX] Array index must be an integer at line " + std::to_string(currentLine));
        }
        if (idx.intVal < 0 || idx.intVal >= (int)v->arrVal.size()) {
            throw std::runtime_error("[VDX] Array index " + std::to_string(idx.intVal) +
                " out of bounds (size " + std::to_string(v->arrVal.size()) + ") at line " + std::to_string(currentLine));
        }
        v->arrVal[idx.intVal] = evalExpr(idxAssign->value.get());
    } else if (auto dotAssign = dynamic_cast<DotAssignStmt*>(node.get())) {
        Value obj = evalExpr(dotAssign->object.get());
        if (obj.type != Value::OBJECT || !obj.objVal) {
            throw std::runtime_error("[VDX] Cannot set field on non-object at line " + std::to_string(currentLine));
        }
        obj.objVal->fields[dotAssign->field] = evalExpr(dotAssign->value.get());
    } else if (auto es = dynamic_cast<ExprStmt*>(node.get())) {
        evalExpr(es->expr.get());
    }
}

void Interpreter::execLet(const LetStmt* stmt) {
    Value val = evalExpr(stmt->value.get());
    checkType(stmt->typeAnnotation, val, stmt->line);
    declareVar(stmt->name, val);
}

void Interpreter::execPrint(const PrintStmt* stmt) {
    for (size_t i = 0; i < stmt->args.size(); i++) {
        if (i > 0) std::cout << " ";
        std::cout << evalExpr(stmt->args[i].get()).toString();
    }
    std::cout << std::endl;
}

void Interpreter::execReturn(const ReturnStmt* stmt) {
    ReturnException ret;
    if (stmt->value) {
        ret.value = evalExpr(stmt->value.get());
    }
    throw ret;
}

void Interpreter::execIf(const IfStmt* stmt) {
    if (isTruthy(evalExpr(stmt->condition.get()))) {
        pushScope();
        for (auto& s : stmt->thenBody) execStatement(s);
        popScope();
        return;
    }
    for (auto& elif : stmt->elifs) {
        if (isTruthy(evalExpr(elif.condition.get()))) {
            pushScope();
            for (auto& s : elif.body) execStatement(s);
            popScope();
            return;
        }
    }
    if (!stmt->elseBody.empty()) {
        pushScope();
        for (auto& s : stmt->elseBody) execStatement(s);
        popScope();
    }
}

void Interpreter::execWhile(const WhileStmt* stmt) {
    while (isTruthy(evalExpr(stmt->condition.get()))) {
        auto iterStart = std::chrono::steady_clock::now();

        pushScope();
        for (auto& s : stmt->body) execStatement(s);
        popScope();

        if (!stmt->isUnsafe) {
            auto iterEnd = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(iterEnd - iterStart).count();
            if (elapsed < 2000) {
                throw std::runtime_error(
                    "[VDX] Loop safety: iteration completed in " + std::to_string(elapsed) +
                    "ms (< 2000ms minimum).\n"
                    "      This loop may be infinite or too fast.\n"
                    "      Use @unsafe before 'while' to disable this protection:\n"
                    "      @unsafe while (condition) { ... }");
            }
        }
    }
}

void Interpreter::execFor(const ForStmt* stmt) {
    pushScope(); // scope for the init variable

    // Execute init
    execStatement(stmt->init);

    while (isTruthy(evalExpr(stmt->condition.get()))) {
        auto iterStart = std::chrono::steady_clock::now();

        pushScope(); // body scope
        for (auto& s : stmt->body) execStatement(s);
        popScope();

        // Execute update
        execStatement(stmt->update);

        if (!stmt->isUnsafe) {
            auto iterEnd = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(iterEnd - iterStart).count();
            if (elapsed < 2000) {
                throw std::runtime_error(
                    "[VDX] Loop safety: iteration completed in " + std::to_string(elapsed) +
                    "ms (< 2000ms minimum).\n"
                    "      This loop may be infinite or too fast.\n"
                    "      Use @unsafe before 'for' to disable this protection:\n"
                    "      @unsafe for (let i = 0; i < n; i = i + 1) { ... }");
            }
        }
    }

    popScope(); // pop init scope
}

void Interpreter::execForIn(const ForInStmt* stmt) {
    Value iterable = evalExpr(stmt->iterable.get());
    if (iterable.type != Value::ARRAY) {
        throw std::runtime_error("[VDX] for-in requires an array at line " + std::to_string(currentLine));
    }
    for (size_t i = 0; i < iterable.arrVal.size(); i++) {
        pushScope();
        declareVar(stmt->varName, iterable.arrVal[i]);
        for (auto& s : stmt->body) execStatement(s);
        popScope();
    }
}

void Interpreter::execWait(const WaitStmt* stmt) {
    Value dur = evalExpr(stmt->duration.get());
    if (dur.type != Value::INT) {
        throw std::runtime_error("[VDX] wait() expects an integer (milliseconds) at line " + std::to_string(currentLine));
    }
    if (dur.intVal < 0) {
        throw std::runtime_error("[VDX] wait() duration cannot be negative at line " + std::to_string(currentLine));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(dur.intVal));
}

bool Interpreter::isTruthy(const Value& v) {
    switch (v.type) {
        case Value::BOOL: return v.boolVal;
        case Value::INT: return v.intVal != 0;
        case Value::FLOAT: return v.floatVal != 0.0;
        case Value::STRING: return !v.strVal.empty();
        case Value::VOID: return false;
        case Value::ARRAY: return !v.arrVal.empty();
        case Value::OBJECT: return v.objVal != nullptr;
    }
    return false;
}

Value Interpreter::execNew(const NewExpr* expr) {
    auto it = classDecls.find(expr->className);
    if (it == classDecls.end()) {
        throw std::runtime_error("[VDX] Undefined class '" + expr->className + "' at line " + std::to_string(currentLine));
    }
    const ClassDecl* cls = it->second;

    auto obj = std::make_shared<ObjectData>();
    obj->className = expr->className;

    // Push a scope for the object construction
    pushScope();

    // First pass: register methods
    for (auto& node : cls->body) {
        if (auto fn = dynamic_cast<FnDecl*>(node.get())) {
            obj->methods[fn->name] = fn;
        }
    }

    // Second pass: execute non-function statements (to set up fields)
    for (auto& node : cls->body) {
        if (dynamic_cast<FnDecl*>(node.get())) continue;
        execStatement(node);
    }

    // Capture all variables from the current scope as fields
    if (!scopes.empty()) {
        for (auto& pair : scopes.back()) {
            obj->fields[pair.first] = pair.second;
        }
    }

    popScope();

    return Value::makeObject(obj);
}

Value Interpreter::execCall(const CallExpr* call) {
    // Built-in: len(array_or_string)
    if (call->name == "len") {
        if (call->args.size() != 1) {
            throw std::runtime_error("[VDX] len() expects 1 argument, got " +
                std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
        }
        Value arg = evalExpr(call->args[0].get());
        if (arg.type == Value::ARRAY) return Value::makeInt((int)arg.arrVal.size());
        if (arg.type == Value::STRING) return Value::makeInt((int)arg.strVal.size());
        throw std::runtime_error("[VDX] len() expects an array or string at line " + std::to_string(currentLine));
    }
    // Built-in: push(array, value)
    if (call->name == "push") {
        if (call->args.size() != 2) {
            throw std::runtime_error("[VDX] push() expects 2 arguments, got " +
                std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
        }
        auto id = dynamic_cast<const IdentifierExpr*>(call->args[0].get());
        if (!id) {
            throw std::runtime_error("[VDX] push() first argument must be a variable at line " + std::to_string(currentLine));
        }
        Value* arr = lookupVar(id->name);
        if (!arr || arr->type != Value::ARRAY) {
            throw std::runtime_error("[VDX] push() first argument must be an array variable at line " + std::to_string(currentLine));
        }
        arr->arrVal.push_back(evalExpr(call->args[1].get()));
        return Value::makeVoid();
    }

    auto it = functions.find(call->name);
    if (it == functions.end()) {
        throw std::runtime_error("[VDX] Undefined function '" + call->name + "' at line " + std::to_string(currentLine));
    }
    const FnDecl* fn = it->second;
    if (call->args.size() != fn->params.size()) {
        throw std::runtime_error("[VDX] Function '" + fn->name + "' expects " +
            std::to_string(fn->params.size()) + " args, got " +
            std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
    }

    std::vector<Value> argVals;
    for (size_t i = 0; i < call->args.size(); i++) {
        argVals.push_back(evalExpr(call->args[i].get()));
    }

    pushScope();
    for (size_t i = 0; i < fn->params.size(); i++) {
        declareVar(fn->params[i], argVals[i]);
    }

    Value result = Value::makeVoid();
    try {
        for (auto& stmt : fn->body) {
            execStatement(stmt);
        }
    } catch (ReturnException& e) {
        result = e.value;
    }

    popScope();
    return result;
}

Value Interpreter::evalExpr(const Expr* expr) {
    if (expr->line > 0) currentLine = expr->line;

    if (auto str = dynamic_cast<const StringLiteral*>(expr)) {
        return Value::makeString(str->value);
    }
    if (auto num = dynamic_cast<const IntLiteral*>(expr)) {
        return Value::makeInt(num->value);
    }
    if (auto flt = dynamic_cast<const FloatLiteral*>(expr)) {
        return Value::makeFloat(flt->value);
    }
    if (auto bl = dynamic_cast<const BoolLiteral*>(expr)) {
        return Value::makeBool(bl->value);
    }
    if (auto id = dynamic_cast<const IdentifierExpr*>(expr)) {
        Value* v = lookupVar(id->name);
        if (!v) {
            throw std::runtime_error("[VDX] Undefined variable '" + id->name + "' at line " + std::to_string(currentLine));
        }
        return *v;
    }
    if (auto bin = dynamic_cast<const BinaryExpr*>(expr)) {
        return evalBinary(bin);
    }
    if (auto call = dynamic_cast<const CallExpr*>(expr)) {
        return execCall(call);
    }
    if (auto te = dynamic_cast<const ThisExpr*>(expr)) {
        Value* v = lookupVar(te->field);
        if (!v) {
            throw std::runtime_error("[VDX] Undefined field 'this." + te->field + "' at line " + std::to_string(currentLine));
        }
        return *v;
    }
    if (auto arr = dynamic_cast<const ArrayLiteral*>(expr)) {
        std::vector<Value> elems;
        for (auto& e : arr->elements) {
            elems.push_back(evalExpr(e.get()));
        }
        return Value::makeArray(elems);
    }
    if (auto idx = dynamic_cast<const IndexExpr*>(expr)) {
        Value obj = evalExpr(idx->object.get());
        Value index = evalExpr(idx->index.get());
        if (obj.type == Value::ARRAY) {
            if (index.type != Value::INT) {
                throw std::runtime_error("[VDX] Array index must be an integer at line " + std::to_string(currentLine));
            }
            if (index.intVal < 0 || index.intVal >= (int)obj.arrVal.size()) {
                throw std::runtime_error("[VDX] Array index " + std::to_string(index.intVal) +
                    " out of bounds (size " + std::to_string(obj.arrVal.size()) + ") at line " + std::to_string(currentLine));
            }
            return obj.arrVal[index.intVal];
        }
        if (obj.type == Value::STRING) {
            if (index.type != Value::INT) {
                throw std::runtime_error("[VDX] String index must be an integer at line " + std::to_string(currentLine));
            }
            if (index.intVal < 0 || index.intVal >= (int)obj.strVal.size()) {
                throw std::runtime_error("[VDX] String index " + std::to_string(index.intVal) +
                    " out of bounds (length " + std::to_string(obj.strVal.size()) + ") at line " + std::to_string(currentLine));
            }
            return Value::makeString(std::string(1, obj.strVal[index.intVal]));
        }
        throw std::runtime_error("[VDX] Cannot index into this type at line " + std::to_string(currentLine));
    }
    if (auto ne = dynamic_cast<const NewExpr*>(expr)) {
        return execNew(ne);
    }
    if (auto dot = dynamic_cast<const DotExpr*>(expr)) {
        Value obj = evalExpr(dot->object.get());
        if (obj.type != Value::OBJECT || !obj.objVal) {
            throw std::runtime_error("[VDX] Cannot access field '" + dot->field + "' on non-object at line " + std::to_string(currentLine));
        }
        auto it = obj.objVal->fields.find(dot->field);
        if (it == obj.objVal->fields.end()) {
            throw std::runtime_error("[VDX] Undefined field '" + dot->field + "' on " + obj.objVal->className + " at line " + std::to_string(currentLine));
        }
        return it->second;
    }
    if (auto dc = dynamic_cast<const DotCallExpr*>(expr)) {
        Value obj = evalExpr(dc->object.get());
        if (obj.type != Value::OBJECT || !obj.objVal) {
            throw std::runtime_error("[VDX] Cannot call method '" + dc->method + "' on non-object at line " + std::to_string(currentLine));
        }
        auto it = obj.objVal->methods.find(dc->method);
        if (it == obj.objVal->methods.end()) {
            throw std::runtime_error("[VDX] Undefined method '" + dc->method + "' on " + obj.objVal->className + " at line " + std::to_string(currentLine));
        }
        const FnDecl* fn = it->second;
        if (dc->args.size() != fn->params.size()) {
            throw std::runtime_error("[VDX] Method '" + dc->method + "' expects " +
                std::to_string(fn->params.size()) + " args, got " +
                std::to_string(dc->args.size()) + " at line " + std::to_string(currentLine));
        }

        std::vector<Value> argVals;
        for (size_t i = 0; i < dc->args.size(); i++) {
            argVals.push_back(evalExpr(dc->args[i].get()));
        }

        // Push object fields as scope, then function params
        pushScope();
        for (auto& pair : obj.objVal->fields) {
            declareVar(pair.first, pair.second);
        }
        pushScope();
        for (size_t i = 0; i < fn->params.size(); i++) {
            declareVar(fn->params[i], argVals[i]);
        }

        Value result = Value::makeVoid();
        try {
            for (auto& stmt : fn->body) {
                execStatement(stmt);
            }
        } catch (ReturnException& e) {
            result = e.value;
        }

        // Update object fields from the object scope (methods may modify fields)
        // The object scope is now at scopes[scopes.size() - 2] (under the param scope)
        if (scopes.size() >= 2) {
            auto& objScope = scopes[scopes.size() - 2];
            for (auto& pair : objScope) {
                obj.objVal->fields[pair.first] = pair.second;
            }
        }

        popScope(); // param scope
        popScope(); // object fields scope
        return result;
    }
    throw std::runtime_error("[VDX] Unknown expression type at line " + std::to_string(currentLine));
}

Value Interpreter::evalBinary(const BinaryExpr* expr) {
    Value left = evalExpr(expr->left.get());
    Value right = evalExpr(expr->right.get());

    // String concatenation
    if (expr->op == "+" && left.type == Value::STRING && right.type == Value::STRING) {
        return Value::makeString(left.strVal + right.strVal);
    }

    // Mixed int/float arithmetic — promote to float
    if (left.isNumeric() && right.isNumeric() &&
        (left.type == Value::FLOAT || right.type == Value::FLOAT)) {
        double l = left.toDouble(), r = right.toDouble();
        if (expr->op == "+") return Value::makeFloat(l + r);
        if (expr->op == "-") return Value::makeFloat(l - r);
        if (expr->op == "*") return Value::makeFloat(l * r);
        if (expr->op == "/") {
            if (r == 0.0) throw std::runtime_error("[VDX] Division by zero at line " + std::to_string(currentLine));
            return Value::makeFloat(l / r);
        }
        if (expr->op == "==") return Value::makeBool(l == r);
        if (expr->op == "!=") return Value::makeBool(l != r);
        if (expr->op == "<") return Value::makeBool(l < r);
        if (expr->op == ">") return Value::makeBool(l > r);
        if (expr->op == "<=") return Value::makeBool(l <= r);
        if (expr->op == ">=") return Value::makeBool(l >= r);
    }

    // Integer arithmetic
    if (left.type == Value::INT && right.type == Value::INT) {
        int l = left.intVal, r = right.intVal;
        if (expr->op == "+") return Value::makeInt(l + r);
        if (expr->op == "-") return Value::makeInt(l - r);
        if (expr->op == "*") return Value::makeInt(l * r);
        if (expr->op == "/") {
            if (r == 0) throw std::runtime_error("[VDX] Division by zero at line " + std::to_string(currentLine));
            return Value::makeInt(l / r);
        }
        if (expr->op == "==") return Value::makeBool(l == r);
        if (expr->op == "!=") return Value::makeBool(l != r);
        if (expr->op == "<") return Value::makeBool(l < r);
        if (expr->op == ">") return Value::makeBool(l > r);
        if (expr->op == "<=") return Value::makeBool(l <= r);
        if (expr->op == ">=") return Value::makeBool(l >= r);
    }

    // String equality
    if (left.type == Value::STRING && right.type == Value::STRING) {
        if (expr->op == "==") return Value::makeBool(left.strVal == right.strVal);
        if (expr->op == "!=") return Value::makeBool(left.strVal != right.strVal);
    }

    // Bool equality
    if (left.type == Value::BOOL && right.type == Value::BOOL) {
        if (expr->op == "==") return Value::makeBool(left.boolVal == right.boolVal);
        if (expr->op == "!=") return Value::makeBool(left.boolVal != right.boolVal);
    }

    throw std::runtime_error("[VDX] Invalid operator '" + expr->op +
        "' for given types at line " + std::to_string(currentLine));
}
