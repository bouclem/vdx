#include "interpreter.h"
#include <iostream>
#include <thread>
#include <chrono>

// ── Scope management ──

void Interpreter::pushScope() {
    scopes.emplace_back();
}

void Interpreter::popScope() {
    scopes.pop_back();
}

Value* Interpreter::lookupVar(const std::string& name) {
    // Walk from innermost scope outward
    for (int i = (int)scopes.size() - 1; i >= 0; i--) {
        auto it = scopes[i].find(name);
        if (it != scopes[i].end()) return &it->second;
    }
    return nullptr;
}

void Interpreter::declareVar(const std::string& name, const Value& val) {
    scopes.back()[name] = val;
}

// ── Execution ──

void Interpreter::run(const Program& program) {
    for (auto& decl : program.declarations) {
        auto cls = dynamic_cast<ClassDecl*>(decl.get());
        if (cls) execClass(cls);
    }
}

void Interpreter::execClass(const ClassDecl* cls) {
    pushScope(); // class-level scope
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
    } else if (auto es = dynamic_cast<ExprStmt*>(node.get())) {
        evalExpr(es->expr.get());
    }
}

void Interpreter::execLet(const LetStmt* stmt) {
    declareVar(stmt->name, evalExpr(stmt->value.get()));
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

        // Loop speed protection: if iteration completed in < 2000ms and not @unsafe, block it
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
        case Value::STRING: return !v.strVal.empty();
        case Value::VOID: return false;
        case Value::ARRAY: return !v.arrVal.empty();
    }
    return false;
}

Value Interpreter::execCall(const CallExpr* call) {
    // Built-in: len(array)
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
        // First arg must be an identifier so we can modify the variable
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

    // Evaluate args in current scope before pushing function scope
    std::vector<Value> argVals;
    for (size_t i = 0; i < call->args.size(); i++) {
        argVals.push_back(evalExpr(call->args[i].get()));
    }

    // Push a new scope for the function, bind params
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
    throw std::runtime_error("[VDX] Unknown expression type at line " + std::to_string(currentLine));
}

Value Interpreter::evalBinary(const BinaryExpr* expr) {
    Value left = evalExpr(expr->left.get());
    Value right = evalExpr(expr->right.get());

    // String concatenation
    if (expr->op == "+" && left.type == Value::STRING && right.type == Value::STRING) {
        return Value::makeString(left.strVal + right.strVal);
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
