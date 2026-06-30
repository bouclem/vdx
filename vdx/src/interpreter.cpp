#include "interpreter.h"
#include "lexer.h"
#include "parser.h"
#include "modules/fs.h"
#include "modules/math.h"
#include "modules/graph.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include <cmath>
#include <climits>
#include <fstream>
#include <filesystem>

// ── Value::toString ──

static std::string escapeString(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (char c : s) {
        if (c == '\\') out += "\\\\";
        else if (c == '"') out += "\\\"";
        else if (c == '\n') out += "\\n";
        else if (c == '\t') out += "\\t";
        else if (c == '\r') out += "\\r";
        else out += c;
    }
    return out;
}

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
                if (arrVal[i].type == STRING) s += "\"" + escapeString(arrVal[i].strVal) + "\"";
                else s += arrVal[i].toString();
            }
            s += "]";
            return s;
        }
        case OBJECT: {
            if (objVal) return "<" + objVal->className + " object>";
            return "<object>";
        }
        case DICT: {
            std::string s = "{";
            size_t i = 0;
            for (const auto& pair : dictVal) {
                if (i > 0) s += ", ";
                s += "\"" + pair.first + "\": ";
                if (pair.second.type == STRING) s += "\"" + escapeString(pair.second.strVal) + "\"";
                else s += pair.second.toString();
                i++;
            }
            s += "}";
            return s;
        }
    }
    return "";
}

// ── Module function registration ──

void Interpreter::registerModuleFunc(const std::string& name, ModuleFunc func) {
    moduleFunctions[name] = func;
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
        if (it != scopes[i].end()) return &it->second.value;
    }
    return nullptr;
}

bool Interpreter::isVarConst(const std::string& name) const {
    for (int i = (int)scopes.size() - 1; i >= 0; i--) {
        auto it = scopes[i].find(name);
        if (it != scopes[i].end()) return it->second.isConst;
    }
    return false;
}

void Interpreter::declareVar(const std::string& name, const Value& val, bool isConst) {
    ScopeEntry entry;
    entry.value = val;
    entry.isConst = isConst;
    scopes.back()[name] = entry;
}

// ── Type checking ──

void Interpreter::checkType(const std::string& annotation, const Value& val, int line) {
    if (annotation.empty()) return;
    bool ok = false;
    if (annotation == "int") ok = (val.type == Value::INT);
    else if (annotation == "float") ok = (val.type == Value::FLOAT || val.type == Value::INT);
    else if (annotation == "string") ok = (val.type == Value::STRING);
    else if (annotation == "bool") ok = (val.type == Value::BOOL);
    else if (annotation == "dict") ok = (val.type == Value::DICT);
    else if (annotation == "array" || (annotation.size() >= 2 && annotation.substr(annotation.size() - 2) == "[]")) ok = (val.type == Value::ARRAY);
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
            case Value::DICT: got = "dict"; break;
        }
        throw std::runtime_error("[VDX] Type mismatch: expected '" + annotation +
            "', got '" + got + "' at line " + std::to_string(line));
    }
}

// ── Execution ──

void Interpreter::run(const Program& program, const std::string& sourceDir) {
    sourceDirectory = sourceDir;

    // Register built-in modules (only once per Interpreter instance)
    if (!modulesRegistered) {
        FSModule::registerFS(*this);
        MathModule::registerMath(*this);
        GraphModule::registerGraph(*this);
        modulesRegistered = true;
    }

    // First pass: process imports
    for (auto& decl : program.declarations) {
        auto importStmt = dynamic_cast<ImportStmt*>(decl.get());
        if (importStmt) {
            execImport(importStmt);
        }
    }

    // Second pass: register all class declarations for 'new'
    bool hasClass = false;
    for (auto& decl : program.declarations) {
        auto cls = dynamic_cast<ClassDecl*>(decl.get());
        if (cls) {
            hasClass = true;
            if (classDecls.count(cls->name)) {
                throw std::runtime_error("[VDX] Class '" + cls->name + "' is already defined at line " + std::to_string(cls->line));
            }
            classDecls[cls->name] = cls;
        }
    }

    // Third pass: register top-level function declarations
    for (auto& decl : program.declarations) {
        auto fn = dynamic_cast<FnDecl*>(decl.get());
        if (fn) {
            if (functions.count(fn->name)) {
                throw std::runtime_error("[VDX] Function '" + fn->name + "' is already defined at line " + std::to_string(fn->line));
            }
            functions[fn->name] = fn;
        }
    }

    // Recommendation: suggest using class{} wrapper if none present
    if (!hasClass) {
        std::cerr << "[VDX] Tip: Wrapping code in class{} is recommended for better organization.\n\n";
    }

    // Fourth pass: execute top-level declarations in order
    pushScope();
    try {
        for (auto& decl : program.declarations) {
            if (dynamic_cast<ImportStmt*>(decl.get())) continue;
            if (dynamic_cast<FnDecl*>(decl.get())) continue;
            if (auto cls = dynamic_cast<ClassDecl*>(decl.get())) {
                execClass(cls);
                continue;
            }
            execStatement(decl);
        }
    } catch (ReturnException&) {
        // 'return' at top level outside a function — ignore
    } catch (BreakException&) {
        throw std::runtime_error("[VDX] 'break' used outside of a loop at line " + std::to_string(currentLine));
    } catch (ContinueException&) {
        throw std::runtime_error("[VDX] 'continue' used outside of a loop at line " + std::to_string(currentLine));
    }
    popScope();
}

void Interpreter::execImport(const ImportStmt* stmt) {
    // Resolve the import path
    std::filesystem::path importPath;
    if (std::filesystem::path(stmt->filename).is_absolute()) {
        importPath = stmt->filename;
    } else {
        importPath = std::filesystem::path(sourceDirectory) / stmt->filename;
    }

    // Normalize and check for circular imports
    if (!std::filesystem::exists(importPath)) {
        throw std::runtime_error("[VDX] Cannot import file '" + stmt->filename + "' at line " + std::to_string(stmt->line));
    }
    std::string canonicalPath = std::filesystem::canonical(importPath).string();
    if (importedFiles.count(canonicalPath)) {
        return; // Already imported
    }
    importedFiles.insert(canonicalPath);

    // Read and parse the imported file
    std::ifstream file(importPath);
    if (!file.is_open()) {
        throw std::runtime_error("[VDX] Cannot import file '" + stmt->filename + "' at line " + std::to_string(stmt->line));
    }

    std::stringstream buf;
    buf << file.rdbuf();
    std::string source = buf.str();

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto importedProgram = std::make_shared<Program>(parser.parse());
    
    // Store imported program to keep AST nodes alive
    importedPrograms.push_back(importedProgram);

    // Import classes and functions from the imported file
    std::string importDir = importPath.parent_path().string();

    // Process nested imports from the imported file first (depth-first)
    // Temporarily switch sourceDirectory so relative imports resolve correctly
    std::string savedSourceDir = sourceDirectory;
    sourceDirectory = importDir;
    for (auto& decl : importedProgram->declarations) {
        if (auto nestedImport = dynamic_cast<ImportStmt*>(decl.get())) {
            execImport(nestedImport);
        }
    }
    sourceDirectory = savedSourceDir;

    // Register imported classes
    for (auto& decl : importedProgram->declarations) {
        auto cls = dynamic_cast<ClassDecl*>(decl.get());
        if (cls) {
            if (classDecls.count(cls->name)) {
                throw std::runtime_error("[VDX] Class '" + cls->name + "' is already defined (imported at line " + std::to_string(stmt->line) + ")");
            }
            classDecls[cls->name] = cls;
        }
    }

    // Register imported file's functions (namespaced as ClassName::funcName)
    for (auto& decl : importedProgram->declarations) {
        auto cls = dynamic_cast<ClassDecl*>(decl.get());
        if (cls) {
            for (auto& node : cls->body) {
                if (auto fn = dynamic_cast<FnDecl*>(node.get())) {
                    std::string key = cls->name + "::" + fn->name;
                    if (functions.count(key)) {
                        throw std::runtime_error("[VDX] Function '" + fn->name + "' is already defined in class '" + cls->name + "' at line " + std::to_string(fn->line));
                    }
                    functions[key] = fn;
                }
            }
        }
    }

    // Register imported file's top-level functions (by plain name)
    for (auto& decl : importedProgram->declarations) {
        auto fn = dynamic_cast<FnDecl*>(decl.get());
        if (fn) {
            if (functions.count(fn->name)) {
                throw std::runtime_error("[VDX] Function '" + fn->name + "' is already defined (imported at line " + std::to_string(stmt->line) + ")");
            }
            functions[fn->name] = fn;
        }
    }
}

void Interpreter::execClass(const ClassDecl* cls) {
    pushScope();
    std::string savedClassName = currentClassName;
    currentClassName = cls->name;
    try {
        // First pass: register functions (namespaced as ClassName::funcName)
        for (auto& node : cls->body) {
            if (auto fn = dynamic_cast<FnDecl*>(node.get())) {
                std::string key = cls->name + "::" + fn->name;
                if (functions.count(key)) {
                    throw std::runtime_error("[VDX] Function '" + fn->name + "' is already defined in class '" + cls->name + "' at line " + std::to_string(fn->line));
                }
                functions[key] = fn;
            }
        }
    } catch (...) {
        currentClassName = savedClassName;
        popScope();
        throw;
    }
    currentClassName = savedClassName;
    popScope();
}

void Interpreter::execStatement(const NodePtr& node) {
    if (node->line > 0) currentLine = node->line;

    if (auto importStmt = dynamic_cast<ImportStmt*>(node.get())) {
        execImport(importStmt);
    } else if (auto let = dynamic_cast<LetStmt*>(node.get())) {
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
        if (isVarConst(assign->name)) {
            throw std::runtime_error("[VDX] Cannot assign to const variable '" + assign->name + "' at line " + std::to_string(currentLine));
        }
        Value* v = lookupVar(assign->name);
        if (!v) {
            throw std::runtime_error("[VDX] Undefined variable '" + assign->name + "' (use 'let' to declare) at line " + std::to_string(currentLine));
        }
        *v = evalExpr(assign->value.get());
    } else if (auto idxAssign = dynamic_cast<IndexAssignStmt*>(node.get())) {
        if (isVarConst(idxAssign->name)) {
            throw std::runtime_error("[VDX] Cannot modify const variable '" + idxAssign->name + "' at line " + std::to_string(currentLine));
        }
        Value* v = lookupVar(idxAssign->name);
        if (!v) {
            throw std::runtime_error("[VDX] Undefined variable '" + idxAssign->name + "' at line " + std::to_string(currentLine));
        }
        if (v->type == Value::ARRAY) {
            Value idx = evalExpr(idxAssign->index.get());
            if (idx.type != Value::INT) {
                throw std::runtime_error("[VDX] Array index must be an integer at line " + std::to_string(currentLine));
            }
            if (idx.intVal < 0 || idx.intVal >= (int)v->arrVal.size()) {
                throw std::runtime_error("[VDX] Array index " + std::to_string(idx.intVal) +
                    " out of bounds (size " + std::to_string(v->arrVal.size()) + ") at line " + std::to_string(currentLine));
            }
            v->arrVal[idx.intVal] = evalExpr(idxAssign->value.get());
        } else if (v->type == Value::DICT) {
            Value idx = evalExpr(idxAssign->index.get());
            if (idx.type != Value::STRING) {
                throw std::runtime_error("[VDX] Dictionary key must be a string at line " + std::to_string(currentLine));
            }
            v->dictVal[idx.strVal] = evalExpr(idxAssign->value.get());
        } else {
            throw std::runtime_error("[VDX] Cannot index into variable '" + idxAssign->name + "' at line " + std::to_string(currentLine));
        }
    } else if (auto dotAssign = dynamic_cast<DotAssignStmt*>(node.get())) {
        if (auto idExpr = dynamic_cast<const IdentifierExpr*>(dotAssign->object.get())) {
            if (isVarConst(idExpr->name)) {
                throw std::runtime_error("[VDX] Cannot modify field on const variable '" + idExpr->name + "' at line " + std::to_string(currentLine));
            }
        }
        Value obj = evalExpr(dotAssign->object.get());
        if (obj.type != Value::OBJECT || !obj.objVal) {
            throw std::runtime_error("[VDX] Cannot set field on non-object at line " + std::to_string(currentLine));
        }
        obj.objVal->fields[dotAssign->field] = evalExpr(dotAssign->value.get());
    } else if (auto es = dynamic_cast<ExprStmt*>(node.get())) {
        evalExpr(es->expr.get());
    } else if (auto brk = dynamic_cast<BreakStmt*>(node.get())) {
        execBreak(brk);
    } else if (auto cont = dynamic_cast<ContinueStmt*>(node.get())) {
        execContinue(cont);
    }
}

void Interpreter::execLet(const LetStmt* stmt) {
    Value val = evalExpr(stmt->value.get());
    checkType(stmt->typeAnnotation, val, stmt->line);
    declareVar(stmt->name, val, stmt->isConst);
}

void Interpreter::execBreak(const BreakStmt* stmt) {
    throw BreakException();
}

void Interpreter::execContinue(const ContinueStmt* stmt) {
    throw ContinueException();
}

void Interpreter::execPrint(const PrintStmt* stmt) {
    for (size_t i = 0; i < stmt->args.size(); i++) {
        if (i > 0) std::cout << " ";
        std::cout << evalExpr(stmt->args[i].get()).toString();
    }
    std::cout << '\n';
}

void Interpreter::execReturn(const ReturnStmt* stmt) {
    ReturnException ret;
    if (stmt->value) {
        ret.value = evalExpr(stmt->value.get());
    }
    throw ret;
}

void Interpreter::execBlock(const std::vector<NodePtr>& body) {
    pushScope();
    try {
        for (auto& s : body) execStatement(s);
    } catch (BreakException&) {
        popScope();
        throw;
    } catch (ContinueException&) {
        popScope();
        throw;
    } catch (ReturnException&) {
        popScope();
        throw;
    }
    popScope();
}

void Interpreter::execIf(const IfStmt* stmt) {
    if (isTruthy(evalExpr(stmt->condition.get()))) {
        execBlock(stmt->thenBody);
        return;
    }
    for (auto& elif : stmt->elifs) {
        if (isTruthy(evalExpr(elif.condition.get()))) {
            execBlock(elif.body);
            return;
        }
    }
    if (!stmt->elseBody.empty()) {
        execBlock(stmt->elseBody);
    }
}

void Interpreter::execWhile(const WhileStmt* stmt) {
    while (isTruthy(evalExpr(stmt->condition.get()))) {
        auto iterStart = std::chrono::steady_clock::now();

        pushScope();
        try {
            for (auto& s : stmt->body) execStatement(s);
            popScope();
        } catch (BreakException&) {
            popScope();
            break;
        } catch (ContinueException&) {
            popScope();
        } catch (ReturnException&) {
            popScope();
            throw;
        }

        if (!stmt->isUnsafe) {
            auto iterEnd = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(iterEnd - iterStart).count();
            if (elapsed > 2000) {
                throw std::runtime_error(
                    "[VDX] Loop safety: iteration took " + std::to_string(elapsed) +
                    "ms (> 2000ms maximum).\n"
                    "      This loop may be infinite or too slow.\n"
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
        try {
            for (auto& s : stmt->body) execStatement(s);
            popScope();
        } catch (BreakException&) {
            popScope();
            popScope(); // pop init scope too
            return;
        } catch (ContinueException&) {
            popScope();
        } catch (ReturnException&) {
            popScope();
            popScope(); // pop init scope too
            throw;
        }

        // Execute update
        execStatement(stmt->update);

        if (!stmt->isUnsafe) {
            auto iterEnd = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(iterEnd - iterStart).count();
            if (elapsed > 2000) {
                throw std::runtime_error(
                    "[VDX] Loop safety: iteration took " + std::to_string(elapsed) +
                    "ms (> 2000ms maximum).\n"
                    "      This loop may be infinite or too slow.\n"
                    "      Use @unsafe before 'for' to disable this protection:\n"
                    "      @unsafe for (let i = 0; i < n; i = i + 1) { ... }");
            }
        }
    }

    popScope(); // pop init scope
}

void Interpreter::execForIn(const ForInStmt* stmt) {
    // Try to get a pointer to the live array variable so modifications during iteration are reflected
    Value* arrPtr = nullptr;
    if (auto id = dynamic_cast<const IdentifierExpr*>(stmt->iterable.get())) {
        arrPtr = lookupVar(id->name);
    }
    if (arrPtr && arrPtr->type == Value::ARRAY) {
        for (size_t i = 0; i < arrPtr->arrVal.size(); i++) {
            pushScope();
            declareVar(stmt->varName, arrPtr->arrVal[i], false);
            try {
                for (auto& s : stmt->body) execStatement(s);
            } catch (BreakException&) {
                popScope();
                return;
            } catch (ContinueException&) {
                popScope();
                continue;
            } catch (ReturnException&) {
                popScope();
                throw;
            }
            popScope();
        }
        return;
    }
    // Fallback: evaluate the expression (may not be a simple variable)
    Value iterable = evalExpr(stmt->iterable.get());
    if (iterable.type != Value::ARRAY) {
        throw std::runtime_error("[VDX] for-in requires an array at line " + std::to_string(currentLine));
    }
    for (size_t i = 0; i < iterable.arrVal.size(); i++) {
        pushScope();
        declareVar(stmt->varName, iterable.arrVal[i], false);
        try {
            for (auto& s : stmt->body) execStatement(s);
        } catch (BreakException&) {
            popScope();
            return;
        } catch (ContinueException&) {
            popScope();
            continue;
        } catch (ReturnException&) {
            popScope();
            throw;
        }
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

bool Interpreter::isTruthy(const Value& v) const {
    switch (v.type) {
        case Value::BOOL: return v.boolVal;
        case Value::INT: return v.intVal != 0;
        case Value::FLOAT: return v.floatVal != 0.0;
        case Value::STRING: return !v.strVal.empty();
        case Value::VOID: return false;
        case Value::ARRAY: return !v.arrVal.empty();
        case Value::OBJECT: return v.objVal != nullptr;
        case Value::DICT: return !v.dictVal.empty();
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
    std::string savedClassName = currentClassName;
    std::shared_ptr<ObjectData> savedObject = currentObject;
    currentClassName = expr->className;
    currentObject = obj;

    try {
        // First pass: register methods
        for (auto& node : cls->body) {
            if (auto fn = dynamic_cast<FnDecl*>(node.get())) {
                obj->methods[fn->name] = fn;
            }
        }

        // Second pass: execute only LetStmt nodes (field initializers)
        // Skip side-effect statements like print, if, etc.
        for (auto& node : cls->body) {
            if (auto letStmt = dynamic_cast<LetStmt*>(node.get())) {
                execLet(letStmt);
            }
        }

        // Capture only variables declared via 'let' in the class body as fields
        // (excludes temporaries like loop counters from field initializers)
        for (auto& node : cls->body) {
            if (auto letStmt = dynamic_cast<LetStmt*>(node.get())) {
                Value* val = lookupVar(letStmt->name);
                if (val) {
                    obj->fields[letStmt->name] = *val;
                }
            }
        }
    } catch (...) {
        currentClassName = savedClassName;
        currentObject = savedObject;
        popScope();
        throw;
    }

    popScope();
    currentClassName = savedClassName;
    currentObject = savedObject;

    return Value::makeObject(obj);
}

Value Interpreter::execCall(const CallExpr* call) {
    // Built-in: len(array_or_string_or_object)
    if (call->name == "len") {
        if (call->args.size() != 1) {
            throw std::runtime_error("[VDX] len() expects 1 argument, got " +
                std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
        }
        Value arg = evalExpr(call->args[0].get());
        if (arg.type == Value::ARRAY) return Value::makeInt((int)arg.arrVal.size());
        if (arg.type == Value::STRING) return Value::makeInt((int)arg.strVal.size());
        if (arg.type == Value::OBJECT) {
            if (arg.objVal) {
                return Value::makeInt((int)arg.objVal->fields.size());
            }
            return Value::makeInt(0);
        }
        if (arg.type == Value::DICT) return Value::makeInt((int)arg.dictVal.size());
        throw std::runtime_error("[VDX] len() expects an array, string, object, or dict at line " + std::to_string(currentLine));
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
        if (isVarConst(id->name)) {
            throw std::runtime_error("[VDX] Cannot push to const array '" + id->name + "' at line " + std::to_string(currentLine));
        }
        Value* arr = lookupVar(id->name);
        if (!arr || arr->type != Value::ARRAY) {
            throw std::runtime_error("[VDX] push() first argument must be an array variable at line " + std::to_string(currentLine));
        }
        arr->arrVal.push_back(evalExpr(call->args[1].get()));
        return Value::makeVoid();
    }
    // Built-in: pop(array) - removes and returns last element
    if (call->name == "pop") {
        if (call->args.size() != 1) {
            throw std::runtime_error("[VDX] pop() expects 1 argument, got " +
                std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
        }
        auto id = dynamic_cast<const IdentifierExpr*>(call->args[0].get());
        if (!id) {
            throw std::runtime_error("[VDX] pop() argument must be a variable at line " + std::to_string(currentLine));
        }
        if (isVarConst(id->name)) {
            throw std::runtime_error("[VDX] Cannot pop from const array '" + id->name + "' at line " + std::to_string(currentLine));
        }
        Value* arr = lookupVar(id->name);
        if (!arr || arr->type != Value::ARRAY) {
            throw std::runtime_error("[VDX] pop() argument must be an array variable at line " + std::to_string(currentLine));
        }
        if (arr->arrVal.empty()) {
            throw std::runtime_error("[VDX] pop() cannot pop from empty array at line " + std::to_string(currentLine));
        }
        Value last = arr->arrVal.back();
        arr->arrVal.pop_back();
        return last;
    }
    // Built-in: type(value) - returns type as string
    if (call->name == "type") {
        if (call->args.size() != 1) {
            throw std::runtime_error("[VDX] type() expects 1 argument, got " +
                std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
        }
        Value arg = evalExpr(call->args[0].get());
        std::string typeName;
        switch (arg.type) {
            case Value::INT: typeName = "int"; break;
            case Value::FLOAT: typeName = "float"; break;
            case Value::STRING: typeName = "string"; break;
            case Value::BOOL: typeName = "bool"; break;
            case Value::VOID: typeName = "void"; break;
            case Value::ARRAY: typeName = "array"; break;
            case Value::OBJECT: typeName = "object"; break;
            case Value::DICT: typeName = "dict"; break;
        }
        return Value::makeString(typeName);
    }
    // Built-in: input() or input(prompt) - reads user input
    if (call->name == "input") {
        if (call->args.size() > 1) {
            throw std::runtime_error("[VDX] input() expects 0 or 1 arguments, got " +
                std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
        }
        // Print prompt if provided
        if (call->args.size() == 1) {
            Value prompt = evalExpr(call->args[0].get());
            std::cout << prompt.toString();
        }
        // Read input
        std::string input;
        std::getline(std::cin, input);
        return Value::makeString(input);
    }

    // Check module functions (C++ built-ins like math.sqrt, fs.readFile)
    auto modIt = moduleFunctions.find(call->name);
    if (modIt != moduleFunctions.end()) {
        std::vector<Value> argVals;
        for (size_t i = 0; i < call->args.size(); i++) {
            argVals.push_back(evalExpr(call->args[i].get()));
        }
        return modIt->second(argVals, currentLine);
    }

    // Try namespaced lookup first (currentClassName::funcName), then plain name
    const FnDecl* fn = nullptr;
    bool isMethod = false;
    if (!currentClassName.empty()) {
        auto it = functions.find(currentClassName + "::" + call->name);
        if (it != functions.end()) {
            fn = it->second;
            isMethod = true;
        }
    }
    if (!fn) {
        auto it = functions.find(call->name);
        if (it != functions.end()) {
            fn = it->second;
        }
    }
    if (!fn) {
        throw std::runtime_error("[VDX] Undefined function '" + call->name + "' at line " + std::to_string(currentLine));
    }
    if (call->args.size() != fn->params.size()) {
        throw std::runtime_error("[VDX] Function '" + fn->name + "' expects " +
            std::to_string(fn->params.size()) + " args, got " +
            std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
    }

    std::vector<Value> argVals;
    for (size_t i = 0; i < call->args.size(); i++) {
        argVals.push_back(evalExpr(call->args[i].get()));
    }

    // If calling a method via bare name from within a method, push object fields as scope
    bool pushedObjScope = false;
    if (isMethod && currentObject) {
        pushScope();
        for (auto& pair : currentObject->fields) {
            declareVar(pair.first, pair.second, false);
        }
        pushedObjScope = true;
    }

    pushScope();
    for (size_t i = 0; i < fn->params.size(); i++) {
        declareVar(fn->params[i], argVals[i], false);
    }

    Value result = Value::makeVoid();
    try {
        for (auto& stmt : fn->body) {
            execStatement(stmt);
        }
    } catch (ReturnException& e) {
        result = e.value;
    } catch (BreakException&) {
        if (pushedObjScope && scopes.size() >= 2) {
            auto& objScope = scopes[scopes.size() - 2];
            for (auto& pair : objScope) {
                currentObject->fields[pair.first] = pair.second.value;
            }
        }
        popScope();
        if (pushedObjScope) popScope();
        throw std::runtime_error("[VDX] 'break' used outside of a loop at line " + std::to_string(currentLine));
    } catch (ContinueException&) {
        if (pushedObjScope && scopes.size() >= 2) {
            auto& objScope = scopes[scopes.size() - 2];
            for (auto& pair : objScope) {
                currentObject->fields[pair.first] = pair.second.value;
            }
        }
        popScope();
        if (pushedObjScope) popScope();
        throw std::runtime_error("[VDX] 'continue' used outside of a loop at line " + std::to_string(currentLine));
    }

    // Sync object fields back from the object scope
    if (pushedObjScope && scopes.size() >= 2) {
        auto& objScope = scopes[scopes.size() - 2];
        for (auto& pair : objScope) {
            currentObject->fields[pair.first] = pair.second.value;
        }
    }

    popScope();
    if (pushedObjScope) popScope();
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
        if (!currentObject) {
            throw std::runtime_error("[VDX] 'this' used outside of object context at line " + std::to_string(currentLine));
        }
        return Value::makeObject(currentObject);
    }
    if (auto arr = dynamic_cast<const ArrayLiteral*>(expr)) {
        std::vector<Value> elems;
        for (auto& e : arr->elements) {
            elems.push_back(evalExpr(e.get()));
        }
        return Value::makeArray(elems);
    }
    if (auto dict = dynamic_cast<const DictLiteral*>(expr)) {
        std::unordered_map<std::string, Value> entries;
        for (auto& pair : dict->entries) {
            entries[pair.first] = evalExpr(pair.second.get());
        }
        return Value::makeDict(entries);
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
        if (obj.type == Value::DICT) {
            if (index.type != Value::STRING) {
                throw std::runtime_error("[VDX] Dictionary key must be a string at line " + std::to_string(currentLine));
            }
            auto it = obj.dictVal.find(index.strVal);
            if (it == obj.dictVal.end()) {
                throw std::runtime_error("[VDX] Key '" + index.strVal + "' not found in dictionary at line " + std::to_string(currentLine));
            }
            return it->second;
        }
        throw std::runtime_error("[VDX] Cannot index into this type at line " + std::to_string(currentLine));
    }
    if (auto ne = dynamic_cast<const NewExpr*>(expr)) {
        return execNew(ne);
    }
    if (auto dot = dynamic_cast<const DotExpr*>(expr)) {
        // Check if this is a module constant (e.g. math.pi)
        if (auto ident = dynamic_cast<const IdentifierExpr*>(dot->object.get())) {
            std::string fullFuncName = ident->name + "." + dot->field;
            auto modIt = moduleFunctions.find(fullFuncName);
            if (modIt != moduleFunctions.end()) {
                return modIt->second({}, currentLine);
            }
        }
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
        // Check if this is a module function call (e.g. math.sqrt, fs.readFile)
        if (auto ident = dynamic_cast<const IdentifierExpr*>(dc->object.get())) {
            std::string fullFuncName = ident->name + "." + dc->method;
            auto modIt = moduleFunctions.find(fullFuncName);
            if (modIt != moduleFunctions.end()) {
                std::vector<Value> argVals;
                for (size_t i = 0; i < dc->args.size(); i++) {
                    argVals.push_back(evalExpr(dc->args[i].get()));
                }
                return modIt->second(argVals, currentLine);
            }
        }
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
            declareVar(pair.first, pair.second, false);
        }
        pushScope();
        for (size_t i = 0; i < fn->params.size(); i++) {
            declareVar(fn->params[i], argVals[i], false);
        }

        std::string savedClassName = currentClassName;
        std::shared_ptr<ObjectData> savedObject = currentObject;
        currentClassName = obj.objVal->className;
        currentObject = obj.objVal;

        Value result = Value::makeVoid();
        try {
            for (auto& stmt : fn->body) {
                execStatement(stmt);
            }
        } catch (ReturnException& e) {
            result = e.value;
        } catch (BreakException&) {
            // Sync object fields before popping scopes (field mutations must not be lost)
            if (scopes.size() >= 2) {
                auto& objScope = scopes[scopes.size() - 2];
                for (auto& pair : objScope) {
                    obj.objVal->fields[pair.first] = pair.second.value;
                }
            }
            popScope(); // param scope
            popScope(); // object fields scope
            currentClassName = savedClassName;
            currentObject = savedObject;
            throw std::runtime_error("[VDX] 'break' used outside of a loop at line " + std::to_string(currentLine));
        } catch (ContinueException&) {
            // Sync object fields before popping scopes (field mutations must not be lost)
            if (scopes.size() >= 2) {
                auto& objScope = scopes[scopes.size() - 2];
                for (auto& pair : objScope) {
                    obj.objVal->fields[pair.first] = pair.second.value;
                }
            }
            popScope(); // param scope
            popScope(); // object fields scope
            currentClassName = savedClassName;
            currentObject = savedObject;
            throw std::runtime_error("[VDX] 'continue' used outside of a loop at line " + std::to_string(currentLine));
        }

        // Update object fields from the object scope (methods may modify fields)
        // The object scope is now at scopes[scopes.size() - 2] (under the param scope)
        if (scopes.size() >= 2) {
            auto& objScope = scopes[scopes.size() - 2];
            for (auto& pair : objScope) {
                obj.objVal->fields[pair.first] = pair.second.value;
            }
        }

        popScope(); // param scope
        popScope(); // object fields scope
        currentClassName = savedClassName;
        currentObject = savedObject;
        return result;
    }
    // Modulo: a % b
    if (auto mod = dynamic_cast<const ModuloExpr*>(expr)) {
        Value left = evalExpr(mod->left.get());
        Value right = evalExpr(mod->right.get());
        if (!left.isNumeric() || !right.isNumeric()) {
            throw std::runtime_error("[VDX] Modulo operands must be numeric at line " + std::to_string(currentLine));
        }
        // Use fmod for floats to allow both int and float modulo
        double l = left.toDouble();
        double r = right.toDouble();
        if (r == 0) {
            throw std::runtime_error("[VDX] Division by zero in modulo at line " + std::to_string(currentLine));
        }
        double result = std::fmod(l, r);
        // Return int if both operands were int, otherwise float
        if (left.type == Value::INT && right.type == Value::INT) {
            return Value::makeInt(static_cast<int64_t>(result));
        }
        return Value::makeFloat(result);
    }
    // Increment/decrement: ++x, x++, --x, x--
    if (auto incDec = dynamic_cast<const IncDecExpr*>(expr)) {
        if (isVarConst(incDec->name)) {
            throw std::runtime_error("[VDX] Cannot increment/decrement const variable '" + incDec->name + "' at line " + std::to_string(currentLine));
        }
        Value* var = lookupVar(incDec->name);
        if (!var) {
            throw std::runtime_error("[VDX] Undefined variable '" + incDec->name + "' at line " + std::to_string(currentLine));
        }
        if (!var->isNumeric()) {
            throw std::runtime_error("[VDX] Cannot increment/decrement non-numeric value at line " + std::to_string(currentLine));
        }

        // Get original value
        Value original = *var;
        double origVal = original.toDouble();

        // Calculate new value
        double delta = incDec->isIncrement ? 1.0 : -1.0;
        double newVal = origVal + delta;

        // Update the variable
        if (original.type == Value::INT) {
            int64_t result = static_cast<int64_t>(original.intVal) + static_cast<int64_t>(delta);
            if (result > INT_MAX || result < INT_MIN)
                throw std::runtime_error("[VDX] Integer overflow in increment/decrement at line " + std::to_string(currentLine));
            *var = Value::makeInt(result);
        } else {
            *var = Value::makeFloat(newVal);
        }

        // Return original value for postfix, new value for prefix
        if (incDec->isPrefix) {
            if (original.type == Value::INT) {
                int64_t result = static_cast<int64_t>(original.intVal) + static_cast<int64_t>(delta);
                if (result > INT_MAX || result < INT_MIN)
                    throw std::runtime_error("[VDX] Integer overflow in increment/decrement at line " + std::to_string(currentLine));
                return Value::makeInt(result);
            }
            return Value::makeFloat(newVal);
        } else {
            return original;
        }
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
        if (expr->op == "+") {
            long long result = static_cast<long long>(l) + static_cast<long long>(r);
            if (result > INT_MAX || result < INT_MIN)
                throw std::runtime_error("[VDX] Integer overflow in addition at line " + std::to_string(currentLine));
            return Value::makeInt(static_cast<int>(result));
        }
        if (expr->op == "-") {
            long long result = static_cast<long long>(l) - static_cast<long long>(r);
            if (result > INT_MAX || result < INT_MIN)
                throw std::runtime_error("[VDX] Integer overflow in subtraction at line " + std::to_string(currentLine));
            return Value::makeInt(static_cast<int>(result));
        }
        if (expr->op == "*") {
            long long result = static_cast<long long>(l) * static_cast<long long>(r);
            if (result > INT_MAX || result < INT_MIN)
                throw std::runtime_error("[VDX] Integer overflow in multiplication at line " + std::to_string(currentLine));
            return Value::makeInt(static_cast<int>(result));
        }
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
