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
#include <algorithm>
#include <cctype>
#include <system_error>

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
            // Ensure floats show a decimal point — but not for scientific
            // notation ("1e+20") or specials ("inf", "-inf", "nan")
            if (s.find('.') == std::string::npos && s.find('e') == std::string::npos &&
                s.find('E') == std::string::npos && s.find("inf") == std::string::npos &&
                s.find("nan") == std::string::npos) {
                s += ".0";
            }
            return s;
        }
        case BOOL: return boolVal ? "true" : "false";
        case VOID: return "void";
        case ARRAY: {
            std::string s = "[";
            if (arrVal) {
                for (size_t i = 0; i < arrVal->size(); i++) {
                    if (i > 0) s += ", ";
                    const Value& el = (*arrVal)[i];
                    if (el.type == STRING) s += "\"" + escapeString(el.strVal) + "\"";
                    else s += el.toString();
                }
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
            for (const auto& pair : *dictVal) {
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
    // Inside a method, bare names fall through to object fields — so 'x' and
    // 'this.x' are the same storage and can never diverge.
    if (currentObject) {
        auto it = currentObject->fields.find(name);
        if (it != currentObject->fields.end()) return &it->second;
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

// ── Lvalue resolution ──
// evalLValue/fieldLValue/indexLValue return pointers to live storage so
// assignments work through chains like a.b[i].f = v, this.arr[i] = v, arr[i][j] = v.

// Walks an lvalue chain to its root identifier and rejects writes to consts.
void Interpreter::checkConstTarget(const Expr* expr) {
    const Expr* e = expr;
    while (e) {
        if (auto id = dynamic_cast<const IdentifierExpr*>(e)) {
            if (isVarConst(id->name)) {
                throw std::runtime_error("[VDX] Cannot modify const variable '" + id->name +
                    "' at line " + std::to_string(currentLine));
            }
            return;
        }
        if (auto dot = dynamic_cast<const DotExpr*>(e)) { e = dot->object.get(); continue; }
        if (auto idx = dynamic_cast<const IndexExpr*>(e)) { e = idx->object.get(); continue; }
        return; // e.g. ThisExpr — 'this' itself is never const-checked
    }
}

Value* Interpreter::fieldLValue(const Expr* object, const std::string& field, bool forWrite) {
    std::shared_ptr<ObjectData> obj;
    if (dynamic_cast<const ThisExpr*>(object)) {
        if (!currentObject) {
            throw std::runtime_error("[VDX] 'this' used outside of object context at line " +
                std::to_string(currentLine));
        }
        obj = currentObject;
    } else {
        Value* ov = evalLValue(object, forWrite);
        if (ov->type != Value::OBJECT || !ov->objVal) {
            throw std::runtime_error("[VDX] Cannot access field '" + field + "' on " +
                ov->typeName() + " at line " + std::to_string(currentLine));
        }
        obj = ov->objVal;
    }
    auto it = obj->fields.find(field);
    if (it == obj->fields.end()) {
        if (forWrite) return &obj->fields[field]; // assignment creates new fields
        throw std::runtime_error("[VDX] Undefined field '" + field + "' on " + obj->className +
            " at line " + std::to_string(currentLine));
    }
    return &it->second;
}

Value* Interpreter::indexLValue(Value* container, const Value& index, bool create) {
    if (container->type == Value::ARRAY) {
        if (index.type != Value::INT) {
            throw std::runtime_error("[VDX] Array index must be an integer at line " +
                std::to_string(currentLine));
        }
        if (index.intVal < 0 || index.intVal >= (int)container->arrVal->size()) {
            throw std::runtime_error("[VDX] Array index " + std::to_string(index.intVal) +
                " out of bounds (size " + std::to_string(container->arrVal->size()) +
                ") at line " + std::to_string(currentLine));
        }
        return &(*container->arrVal)[index.intVal];
    }
    if (container->type == Value::DICT) {
        if (index.type != Value::STRING) {
            throw std::runtime_error("[VDX] Dictionary key must be a string at line " +
                std::to_string(currentLine));
        }
        auto& map = *container->dictVal;
        auto it = map.find(index.strVal);
        if (it == map.end()) {
            if (!create) {
                throw std::runtime_error("[VDX] Key '" + index.strVal +
                    "' not found in dictionary at line " + std::to_string(currentLine));
            }
            return &map[index.strVal]; // assignment creates the key
        }
        return &it->second;
    }
    throw std::runtime_error("[VDX] Cannot index into " + std::string(container->typeName()) +
        " at line " + std::to_string(currentLine));
}

Value* Interpreter::evalLValue(const Expr* expr, bool forWrite) {
    if (expr->line > 0) currentLine = expr->line;
    if (auto id = dynamic_cast<const IdentifierExpr*>(expr)) {
        Value* v = lookupVar(id->name);
        if (!v) {
            throw std::runtime_error("[VDX] Undefined variable '" + id->name +
                "' at line " + std::to_string(currentLine));
        }
        return v;
    }
    if (auto dot = dynamic_cast<const DotExpr*>(expr)) {
        return fieldLValue(dot->object.get(), dot->field, forWrite);
    }
    if (auto idx = dynamic_cast<const IndexExpr*>(expr)) {
        Value* container = evalLValue(idx->object.get(), false);
        if (container->type == Value::STRING) {
            throw std::runtime_error("[VDX] String element is not assignable here; "
                "use s[i] = \"c\" at line " + std::to_string(currentLine));
        }
        Value index = evalExpr(idx->index.get());
        return indexLValue(container, index, forWrite);
    }
    throw std::runtime_error("[VDX] Expression is not assignable at line " +
        std::to_string(currentLine));
}

// ── Error context for imported files ──

void Interpreter::recordErrorFile(const Node* decl) {
    if (!errorFile.empty()) return; // first (deepest) attribution wins
    auto it = declFiles.find(decl);
    if (it == declFiles.end()) return;
    errorFile = it->second;
    auto src = fileSources.find(it->second);
    if (src != fileSources.end()) errorSource = src->second;
}

// ── Loop safety ──

void Interpreter::checkLoopSafety(std::chrono::steady_clock::time_point iterStart,
                                  long long iteration, bool isUnsafe, const char* loopName) {
    if (isUnsafe) { ioExcludedMs = 0; return; }
    if (iteration > MAX_LOOP_ITERATIONS) {
        throw std::runtime_error(
            "[VDX] Loop safety: " + std::string(loopName) + " loop exceeded " +
            std::to_string(MAX_LOOP_ITERATIONS) + " iterations.\n"
            "      This loop may be infinite.\n"
            "      Use @unsafe before '" + loopName + "' to disable this protection.");
    }
    // Time blocked inside wait()/input() does not count toward the iteration budget
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - iterStart).count() - ioExcludedMs;
    ioExcludedMs = 0;
    if (elapsed > 2000) {
        throw std::runtime_error(
            "[VDX] Loop safety: iteration took " + std::to_string(elapsed) +
            "ms (> 2000ms maximum).\n"
            "      This loop may be infinite or too slow.\n"
            "      Use @unsafe before '" + loopName + "' to disable this protection.");
    }
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
        throw std::runtime_error("[VDX] Type mismatch: expected '" + annotation +
            "', got '" + val.typeName() + "' at line " + std::to_string(line));
    }
}

// ── Execution ──

void Interpreter::run(const Program& program, const std::string& sourceDir, const std::string& mainFile) {
    // Reset per-run state so an Interpreter instance is safely reusable
    scopes.clear();
    functions.clear();
    classDecls.clear();
    importedFiles.clear();
    importedPrograms.clear();
    declFiles.clear();
    fileSources.clear();
    currentObject.reset();
    currentClassName.clear();
    currentLine = 0;
    callDepth = 0;
    ioExcludedMs = 0;
    errorFile.clear();
    errorSource.clear();

    sourceDirectory = sourceDir;

    // Register built-in modules (only once per Interpreter instance)
    if (!modulesRegistered) {
        FSModule::registerFS(*this);
        MathModule::registerMath(*this);
        GraphModule::registerGraph(*this);
        modulesRegistered = true;
    }

    // Mark the entry file as imported so a circular import back into it is
    // skipped like any other duplicate import (instead of re-registering its
    // declarations and failing with a confusing duplicate-definition error).
    if (!mainFile.empty() && std::filesystem::exists(mainFile)) {
        std::error_code ec;
        auto canon = std::filesystem::canonical(mainFile, ec);
        if (!ec) {
            importedFiles.insert(canon.string());
            std::ifstream f(mainFile);
            if (f.is_open()) {
                std::stringstream buf;
                buf << f.rdbuf();
                fileSources[canon.string()] = buf.str();
            }
        }
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
        popScope();
        throw std::runtime_error("[VDX] 'break' used outside of a loop at line " + std::to_string(currentLine));
    } catch (ContinueException&) {
        popScope();
        throw std::runtime_error("[VDX] 'continue' used outside of a loop at line " + std::to_string(currentLine));
    } catch (...) {
        popScope();
        throw;
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
    fileSources[canonicalPath] = source;

    std::shared_ptr<Program> importedProgram;
    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();
        Parser parser(tokens);
        importedProgram = std::make_shared<Program>(parser.parse());
    } catch (const std::runtime_error&) {
        // Attribute the error to the imported file so the displayed source
        // context comes from that file, not the entry file
        errorFile = canonicalPath;
        errorSource = source;
        throw;
    }

    // Store imported program to keep AST nodes alive
    importedPrograms.push_back(importedProgram);

    // Remember which file each declaration came from (for runtime error context)
    for (auto& decl : importedProgram->declarations) {
        declFiles[decl.get()] = canonicalPath;
        if (auto cls = dynamic_cast<ClassDecl*>(decl.get())) {
            for (auto& member : cls->body) {
                declFiles[member.get()] = canonicalPath;
            }
        }
    }

    // Import classes and functions from the imported file
    std::string importDir = importPath.parent_path().string();

    // Process nested imports from the imported file first (depth-first)
    // Temporarily switch sourceDirectory so relative imports resolve correctly
    std::string savedSourceDir = sourceDirectory;
    sourceDirectory = importDir;
    try {
        for (auto& decl : importedProgram->declarations) {
            if (auto nestedImport = dynamic_cast<ImportStmt*>(decl.get())) {
                execImport(nestedImport);
            }
        }
    } catch (...) {
        sourceDirectory = savedSourceDir;
        throw;
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
        // First pass: register methods (namespaced as ClassName::funcName)
        for (auto& node : cls->body) {
            if (auto fn = dynamic_cast<FnDecl*>(node.get())) {
                std::string key = cls->name + "::" + fn->name;
                if (functions.count(key)) {
                    throw std::runtime_error("[VDX] Function '" + fn->name + "' is already defined in class '" + cls->name + "' at line " + std::to_string(fn->line));
                }
                functions[key] = fn;
            }
        }
        // Second pass: execute the class body as program code. Field
        // declarations (let/const) become variables in this scope; other
        // statements (print, if, loops, calls) run normally. This is what
        // makes 'class Main { <program> }' work as the recommended wrapper.
        for (auto& node : cls->body) {
            if (dynamic_cast<FnDecl*>(node.get())) continue;
            execStatement(node);
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
        checkConstTarget(idxAssign->object.get());
        Value* container = evalLValue(idxAssign->object.get(), false);
        Value idx = evalExpr(idxAssign->index.get());
        if (container->type == Value::STRING) {
            if (idx.type != Value::INT) {
                throw std::runtime_error("[VDX] String index must be an integer at line " + std::to_string(currentLine));
            }
            if (idx.intVal < 0 || idx.intVal >= (int)container->strVal.size()) {
                throw std::runtime_error("[VDX] String index " + std::to_string(idx.intVal) +
                    " out of bounds (length " + std::to_string(container->strVal.size()) + ") at line " + std::to_string(currentLine));
            }
            Value val = evalExpr(idxAssign->value.get());
            if (val.type != Value::STRING || val.strVal.size() != 1) {
                throw std::runtime_error("[VDX] String index assignment requires a single-character string at line " + std::to_string(currentLine));
            }
            container->strVal[idx.intVal] = val.strVal[0];
        } else {
            *indexLValue(container, idx, true) = evalExpr(idxAssign->value.get());
        }
    } else if (auto dotAssign = dynamic_cast<DotAssignStmt*>(node.get())) {
        checkConstTarget(dotAssign->object.get());
        *fieldLValue(dotAssign->object.get(), dotAssign->field, true) = evalExpr(dotAssign->value.get());
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
    (void)stmt;
    throw BreakException();
}

void Interpreter::execContinue(const ContinueStmt* stmt) {
    (void)stmt;
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
    long long iteration = 0;
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
        } catch (...) {
            popScope();
            throw;
        }

        checkLoopSafety(iterStart, ++iteration, stmt->isUnsafe, "while");
    }
}

void Interpreter::execFor(const ForStmt* stmt) {
    pushScope(); // scope for the init variable
    long long iteration = 0;

    try {
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
            } catch (...) {
                popScope();
                throw;
            }

            // Execute update
            execStatement(stmt->update);

            checkLoopSafety(iterStart, ++iteration, stmt->isUnsafe, "for");
        }
    } catch (...) {
        popScope(); // pop init scope
        throw;
    }

    popScope(); // pop init scope
}

void Interpreter::execForIn(const ForInStmt* stmt) {
    Value iterable = evalExpr(stmt->iterable.get());

    // Arrays share storage, so iterating a live view reflects push/pop during
    // iteration. Strings iterate over characters; dicts iterate over sorted keys.
    std::shared_ptr<std::vector<Value>> liveArr;
    std::vector<Value> items;
    if (iterable.type == Value::ARRAY) {
        liveArr = iterable.arrVal;
    } else if (iterable.type == Value::STRING) {
        for (char c : iterable.strVal) {
            items.push_back(Value::makeString(std::string(1, c)));
        }
    } else if (iterable.type == Value::DICT) {
        std::vector<std::string> keys;
        keys.reserve(iterable.dictVal->size());
        for (const auto& pair : *iterable.dictVal) keys.push_back(pair.first);
        std::sort(keys.begin(), keys.end());
        for (auto& k : keys) items.push_back(Value::makeString(k));
    } else {
        throw std::runtime_error("[VDX] for-in requires an array, string, or dictionary, got '" +
            std::string(iterable.typeName()) + "' at line " + std::to_string(currentLine));
    }

    auto count = [&]() -> size_t { return liveArr ? liveArr->size() : items.size(); };
    auto at = [&](size_t i) -> Value& { return liveArr ? (*liveArr)[i] : items[i]; };

    long long iteration = 0;
    for (size_t i = 0; i < count(); i++) {
        auto iterStart = std::chrono::steady_clock::now();

        pushScope();
        declareVar(stmt->varName, at(i), false);
        try {
            for (auto& s : stmt->body) execStatement(s);
            popScope();
        } catch (BreakException&) {
            popScope();
            return;
        } catch (ContinueException&) {
            popScope();
        } catch (...) {
            popScope();
            throw;
        }

        checkLoopSafety(iterStart, ++iteration, stmt->isUnsafe, "for-in");
    }
}

void Interpreter::execWait(const WaitStmt* stmt) {
    Value dur = evalExpr(stmt->duration.get());
    if (!dur.isNumeric()) {
        throw std::runtime_error("[VDX] wait() expects a number (milliseconds) at line " + std::to_string(currentLine));
    }
    double ms = dur.toDouble();
    if (ms < 0) {
        throw std::runtime_error("[VDX] wait() duration cannot be negative at line " + std::to_string(currentLine));
    }
    if (ms > 0) {
        // Time spent in wait() is excluded from loop-safety timing
        ioExcludedMs += static_cast<long long>(ms + 0.5);
        std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(ms));
    }
}

bool Interpreter::isTruthy(const Value& v) const {
    switch (v.type) {
        case Value::BOOL: return v.boolVal;
        case Value::INT: return v.intVal != 0;
        case Value::FLOAT: return v.floatVal != 0.0;
        case Value::STRING: return !v.strVal.empty();
        case Value::VOID: return false;
        case Value::ARRAY: return v.arrVal && !v.arrVal->empty();
        case Value::OBJECT: return v.objVal != nullptr;
        case Value::DICT: return v.dictVal && !v.dictVal->empty();
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
        recordErrorFile(cls);
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

// Shared function-call machinery used by bare calls and method calls.
// 'this' is bound only for methods — plain functions never see a stale object.
Value Interpreter::callFunction(const FnDecl* fn, const std::vector<Value>& args,
                                std::shared_ptr<ObjectData> obj, const std::string& clsName) {
    if (callDepth >= MAX_CALL_DEPTH) {
        throw std::runtime_error("[VDX] Maximum call depth exceeded (" +
            std::to_string(MAX_CALL_DEPTH) + ") — possible infinite recursion at line " +
            std::to_string(currentLine));
    }
    callDepth++;

    auto savedObject = currentObject;
    auto savedClassName = currentClassName;
    currentObject = obj;
    currentClassName = clsName;

    pushScope();
    for (size_t i = 0; i < fn->params.size(); i++) {
        declareVar(fn->params[i], args[i], false);
    }

    auto cleanup = [&]() {
        popScope();
        currentObject = savedObject;
        currentClassName = savedClassName;
        callDepth--;
    };

    Value result = Value::makeVoid();
    try {
        for (auto& stmt : fn->body) execStatement(stmt);
    } catch (ReturnException& e) {
        result = e.value;
    } catch (BreakException&) {
        cleanup();
        throw std::runtime_error("[VDX] 'break' used outside of a loop at line " + std::to_string(currentLine));
    } catch (ContinueException&) {
        cleanup();
        throw std::runtime_error("[VDX] 'continue' used outside of a loop at line " + std::to_string(currentLine));
    } catch (std::runtime_error&) {
        recordErrorFile(fn);
        cleanup();
        throw;
    } catch (...) {
        cleanup();
        throw;
    }
    cleanup();
    return result;
}

// Module-function wrapper: normalizes error format so every module error
// carries source context (some module errors omit "at line N").
Value Interpreter::callModule(const ModuleFunc& f, const std::vector<Value>& args, int line) {
    try {
        return f(args, line);
    } catch (const std::runtime_error& e) {
        std::string msg = e.what();
        if (line > 0 && msg.find("at line") == std::string::npos) {
            msg += " at line " + std::to_string(line);
        }
        throw std::runtime_error(msg);
    }
}

Value Interpreter::execCall(const CallExpr* call) {
    // User-defined functions resolve first — a user 'fn len(x)' shadows the
    // built-in rather than being silently unreachable.
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
        if (it != functions.end()) fn = it->second;
    }
    if (fn) {
        if (call->args.size() != fn->params.size()) {
            throw std::runtime_error("[VDX] Function '" + fn->name + "' expects " +
                std::to_string(fn->params.size()) + " args, got " +
                std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
        }
        std::vector<Value> argVals;
        for (size_t i = 0; i < call->args.size(); i++) {
            argVals.push_back(evalExpr(call->args[i].get()));
        }
        return callFunction(fn, argVals,
                            isMethod ? currentObject : nullptr,
                            isMethod ? currentClassName : "");
    }

    bool handled = false;
    Value builtinResult = execBuiltin(call, handled);
    if (handled) return builtinResult;

    // Module functions (e.g. namespaced C++ built-ins)
    auto modIt = moduleFunctions.find(call->name);
    if (modIt != moduleFunctions.end()) {
        std::vector<Value> argVals;
        for (size_t i = 0; i < call->args.size(); i++) {
            argVals.push_back(evalExpr(call->args[i].get()));
        }
        return callModule(modIt->second, argVals, currentLine);
    }

    throw std::runtime_error("[VDX] Undefined function '" + call->name + "' at line " + std::to_string(currentLine));
}

Value Interpreter::execBuiltin(const CallExpr* call, bool& handled) {
    handled = true;
    const std::string& n = call->name;

    // Built-in: len(array_or_string_or_object)
    if (n == "len") {
        if (call->args.size() != 1) {
            throw std::runtime_error("[VDX] len() expects 1 argument, got " +
                std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
        }
        Value arg = evalExpr(call->args[0].get());
        if (arg.type == Value::ARRAY) return Value::makeInt((int)arg.arrVal->size());
        if (arg.type == Value::STRING) return Value::makeInt((int)arg.strVal.size());
        if (arg.type == Value::OBJECT) {
            if (arg.objVal) return Value::makeInt((int)arg.objVal->fields.size());
            return Value::makeInt(0);
        }
        if (arg.type == Value::DICT) return Value::makeInt((int)arg.dictVal->size());
        throw std::runtime_error("[VDX] len() expects an array, string, object, or dict at line " + std::to_string(currentLine));
    }
    // Built-in: push(array, value) — works on any array lvalue (obj.field, arr[i], this.x)
    if (n == "push") {
        if (call->args.size() != 2) {
            throw std::runtime_error("[VDX] push() expects 2 arguments, got " +
                std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
        }
        checkConstTarget(call->args[0].get());
        Value* arr = evalLValue(call->args[0].get());
        if (arr->type != Value::ARRAY) {
            throw std::runtime_error("[VDX] push() first argument must be an array at line " + std::to_string(currentLine));
        }
        arr->arrVal->push_back(evalExpr(call->args[1].get()));
        return Value::makeVoid();
    }
    // Built-in: pop(array) — removes and returns last element
    if (n == "pop") {
        if (call->args.size() != 1) {
            throw std::runtime_error("[VDX] pop() expects 1 argument, got " +
                std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
        }
        checkConstTarget(call->args[0].get());
        Value* arr = evalLValue(call->args[0].get());
        if (arr->type != Value::ARRAY) {
            throw std::runtime_error("[VDX] pop() argument must be an array at line " + std::to_string(currentLine));
        }
        if (arr->arrVal->empty()) {
            throw std::runtime_error("[VDX] pop() cannot pop from empty array at line " + std::to_string(currentLine));
        }
        Value last = arr->arrVal->back();
        arr->arrVal->pop_back();
        return last;
    }
    // Built-in: type(value) — returns type as string
    if (n == "type") {
        if (call->args.size() != 1) {
            throw std::runtime_error("[VDX] type() expects 1 argument, got " +
                std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
        }
        return Value::makeString(evalExpr(call->args[0].get()).typeName());
    }
    // Built-in: input() or input(prompt) — reads user input
    if (n == "input") {
        if (call->args.size() > 1) {
            throw std::runtime_error("[VDX] input() expects 0 or 1 arguments, got " +
                std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
        }
        if (call->args.size() == 1) {
            std::cout << evalExpr(call->args[0].get()).toString();
        }
        auto t0 = std::chrono::steady_clock::now();
        std::string input;
        std::getline(std::cin, input);
        // Time blocked on input is excluded from loop-safety timing
        ioExcludedMs += std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - t0).count();
        return Value::makeString(input);
    }
    // Built-in: int(value) — numeric/string conversion
    if (n == "int") {
        if (call->args.size() != 1) {
            throw std::runtime_error("[VDX] int() expects 1 argument, got " +
                std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
        }
        Value v = evalExpr(call->args[0].get());
        if (v.type == Value::INT) return v;
        if (v.type == Value::BOOL) return Value::makeInt(v.boolVal ? 1 : 0);
        if (v.type == Value::FLOAT) {
            if (std::isnan(v.floatVal) || std::isinf(v.floatVal) ||
                v.floatVal > (double)INT_MAX || v.floatVal < (double)INT_MIN) {
                throw std::runtime_error("[VDX] int() cannot convert float out of int range at line " + std::to_string(currentLine));
            }
            return Value::makeInt(static_cast<int>(v.floatVal));
        }
        if (v.type == Value::STRING) {
            try {
                size_t pos = 0;
                int r = std::stoi(v.strVal, &pos);
                if (pos != v.strVal.size()) throw std::invalid_argument("trailing");
                return Value::makeInt(r);
            } catch (const std::exception&) {
                throw std::runtime_error("[VDX] int() cannot convert '" + v.strVal + "' at line " + std::to_string(currentLine));
            }
        }
        throw std::runtime_error("[VDX] int() cannot convert " + std::string(v.typeName()) + " at line " + std::to_string(currentLine));
    }
    // Built-in: float(value) — numeric/string conversion
    if (n == "float") {
        if (call->args.size() != 1) {
            throw std::runtime_error("[VDX] float() expects 1 argument, got " +
                std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
        }
        Value v = evalExpr(call->args[0].get());
        if (v.type == Value::FLOAT) return v;
        if (v.type == Value::INT) return Value::makeFloat((double)v.intVal);
        if (v.type == Value::BOOL) return Value::makeFloat(v.boolVal ? 1.0 : 0.0);
        if (v.type == Value::STRING) {
            try {
                size_t pos = 0;
                double r = std::stod(v.strVal, &pos);
                if (pos != v.strVal.size()) throw std::invalid_argument("trailing");
                return Value::makeFloat(r);
            } catch (const std::exception&) {
                throw std::runtime_error("[VDX] float() cannot convert '" + v.strVal + "' at line " + std::to_string(currentLine));
            }
        }
        throw std::runtime_error("[VDX] float() cannot convert " + std::string(v.typeName()) + " at line " + std::to_string(currentLine));
    }
    // Built-in: str(value) — string representation of any value
    if (n == "str") {
        if (call->args.size() != 1) {
            throw std::runtime_error("[VDX] str() expects 1 argument, got " +
                std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
        }
        return Value::makeString(evalExpr(call->args[0].get()).toString());
    }

    // ── String builtins ──
    auto strArg = [&](size_t i, const char* which) -> std::string {
        Value v = evalExpr(call->args[i].get());
        if (v.type != Value::STRING) {
            throw std::runtime_error("[VDX] " + n + "() argument " + which +
                " must be a string at line " + std::to_string(currentLine));
        }
        return v.strVal;
    };
    auto intArg = [&](size_t i, const char* which) -> int {
        Value v = evalExpr(call->args[i].get());
        if (v.type != Value::INT) {
            throw std::runtime_error("[VDX] " + n + "() argument " + which +
                " must be an integer at line " + std::to_string(currentLine));
        }
        return v.intVal;
    };

    // Built-in: split(str, sep) — returns array of parts
    if (n == "split") {
        if (call->args.size() != 2) {
            throw std::runtime_error("[VDX] split() expects 2 arguments, got " +
                std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
        }
        std::string s = strArg(0, "1"), sep = strArg(1, "2");
        if (sep.empty()) {
            throw std::runtime_error("[VDX] split() separator cannot be empty at line " + std::to_string(currentLine));
        }
        std::vector<Value> parts;
        size_t pos = 0;
        while (true) {
            size_t found = s.find(sep, pos);
            if (found == std::string::npos) {
                parts.push_back(Value::makeString(s.substr(pos)));
                break;
            }
            parts.push_back(Value::makeString(s.substr(pos, found - pos)));
            pos = found + sep.size();
        }
        return Value::makeArray(std::move(parts));
    }
    // Built-in: substr(str, start[, len])
    if (n == "substr") {
        if (call->args.size() < 2 || call->args.size() > 3) {
            throw std::runtime_error("[VDX] substr() expects 2 or 3 arguments, got " +
                std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
        }
        std::string s = strArg(0, "1");
        int start = intArg(1, "2");
        if (start < 0 || start > (int)s.size()) {
            throw std::runtime_error("[VDX] substr() start out of range at line " + std::to_string(currentLine));
        }
        size_t len = s.size() - (size_t)start;
        if (call->args.size() == 3) {
            int l = intArg(2, "3");
            if (l < 0) {
                throw std::runtime_error("[VDX] substr() length cannot be negative at line " + std::to_string(currentLine));
            }
            len = std::min(len, (size_t)l);
        }
        return Value::makeString(s.substr((size_t)start, len));
    }
    // Built-in: indexOf(str, sub) — position or -1
    if (n == "indexOf") {
        if (call->args.size() != 2) {
            throw std::runtime_error("[VDX] indexOf() expects 2 arguments, got " +
                std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
        }
        std::string s = strArg(0, "1"), sub = strArg(1, "2");
        size_t pos = s.find(sub);
        return Value::makeInt(pos == std::string::npos ? -1 : (int)pos);
    }
    // Built-in: upper(str) / lower(str)
    if (n == "upper" || n == "lower") {
        if (call->args.size() != 1) {
            throw std::runtime_error("[VDX] " + n + "() expects 1 argument, got " +
                std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
        }
        std::string s = strArg(0, "1");
        for (auto& c : s) {
            c = (char)(n == "upper" ? std::toupper((unsigned char)c) : std::tolower((unsigned char)c));
        }
        return Value::makeString(s);
    }
    // Built-in: trim(str) — strips leading/trailing whitespace
    if (n == "trim") {
        if (call->args.size() != 1) {
            throw std::runtime_error("[VDX] trim() expects 1 argument, got " +
                std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
        }
        std::string s = strArg(0, "1");
        size_t b = s.find_first_not_of(" \t\n\r");
        if (b == std::string::npos) return Value::makeString("");
        return Value::makeString(s.substr(b, s.find_last_not_of(" \t\n\r") - b + 1));
    }
    // Built-in: replace(str, from, to) — replaces all occurrences
    if (n == "replace") {
        if (call->args.size() != 3) {
            throw std::runtime_error("[VDX] replace() expects 3 arguments, got " +
                std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
        }
        std::string s = strArg(0, "1"), from = strArg(1, "2"), to = strArg(2, "3");
        if (from.empty()) {
            throw std::runtime_error("[VDX] replace() 'from' cannot be empty at line " + std::to_string(currentLine));
        }
        size_t pos = 0;
        while ((pos = s.find(from, pos)) != std::string::npos) {
            s.replace(pos, from.size(), to);
            pos += to.size();
        }
        return Value::makeString(s);
    }
    // Built-in: join(array, sep) — joins element strings
    if (n == "join") {
        if (call->args.size() != 2) {
            throw std::runtime_error("[VDX] join() expects 2 arguments, got " +
                std::to_string(call->args.size()) + " at line " + std::to_string(currentLine));
        }
        Value arr = evalExpr(call->args[0].get());
        if (arr.type != Value::ARRAY) {
            throw std::runtime_error("[VDX] join() first argument must be an array at line " + std::to_string(currentLine));
        }
        std::string sep = strArg(1, "2"), out;
        for (size_t i = 0; i < arr.arrVal->size(); i++) {
            if (i > 0) out += sep;
            out += (*arr.arrVal)[i].toString();
        }
        return Value::makeString(out);
    }

    handled = false;
    return Value::makeVoid();
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
    if (dynamic_cast<const ThisExpr*>(expr)) {
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
            if (index.intVal < 0 || index.intVal >= (int)obj.arrVal->size()) {
                throw std::runtime_error("[VDX] Array index " + std::to_string(index.intVal) +
                    " out of bounds (size " + std::to_string(obj.arrVal->size()) + ") at line " + std::to_string(currentLine));
            }
            return (*obj.arrVal)[index.intVal];
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
            auto it = obj.dictVal->find(index.strVal);
            if (it == obj.dictVal->end()) {
                throw std::runtime_error("[VDX] Key '" + index.strVal + "' not found in dictionary at line " + std::to_string(currentLine));
            }
            return it->second;
        }
        throw std::runtime_error("[VDX] Cannot index into " + std::string(obj.typeName()) + " at line " + std::to_string(currentLine));
    }
    if (auto ne = dynamic_cast<const NewExpr*>(expr)) {
        return execNew(ne);
    }
    if (auto dot = dynamic_cast<const DotExpr*>(expr)) {
        // Module constants (e.g. math.pi) only apply when no user variable
        // shadows the module name — a variable named 'math' wins.
        if (auto ident = dynamic_cast<const IdentifierExpr*>(dot->object.get())) {
            if (!lookupVar(ident->name)) {
                std::string fullFuncName = ident->name + "." + dot->field;
                auto modIt = moduleFunctions.find(fullFuncName);
                if (modIt != moduleFunctions.end()) {
                    return callModule(modIt->second, {}, currentLine);
                }
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
        // Module functions (e.g. math.sqrt) only apply when no user variable
        // shadows the module name — a variable named 'math' wins.
        if (auto ident = dynamic_cast<const IdentifierExpr*>(dc->object.get())) {
            if (!lookupVar(ident->name)) {
                std::string fullFuncName = ident->name + "." + dc->method;
                auto modIt = moduleFunctions.find(fullFuncName);
                if (modIt != moduleFunctions.end()) {
                    std::vector<Value> argVals;
                    for (size_t i = 0; i < dc->args.size(); i++) {
                        argVals.push_back(evalExpr(dc->args[i].get()));
                    }
                    return callModule(modIt->second, argVals, currentLine);
                }
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
        return callFunction(fn, argVals, obj.objVal, obj.objVal->className);
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
            return Value::makeInt(static_cast<int>(result));
        }
        return Value::makeFloat(result);
    }
    // Logical: a && b (short-circuit)
    if (auto log = dynamic_cast<const LogicalExpr*>(expr)) {
        Value left = evalExpr(log->left.get());
        if (log->op == "&&") {
            if (!isTruthy(left)) return Value::makeBool(false);
            return Value::makeBool(isTruthy(evalExpr(log->right.get())));
        }
        // "||"
        if (isTruthy(left)) return Value::makeBool(true);
        return Value::makeBool(isTruthy(evalExpr(log->right.get())));
    }
    // Logical not: !x
    if (auto ne = dynamic_cast<const NotExpr*>(expr)) {
        return Value::makeBool(!isTruthy(evalExpr(ne->operand.get())));
    }
    // Increment/decrement on any lvalue: ++x, x++, obj.n++, arr[i]++
    if (auto incDec = dynamic_cast<const IncDecExpr*>(expr)) {
        checkConstTarget(incDec->target.get());
        Value* var = evalLValue(incDec->target.get());
        if (!var->isNumeric()) {
            throw std::runtime_error("[VDX] Cannot increment/decrement non-numeric value at line " + std::to_string(currentLine));
        }

        Value original = *var;
        double origVal = original.toDouble();
        double delta = incDec->isIncrement ? 1.0 : -1.0;
        double newVal = origVal + delta;

        if (original.type == Value::INT) {
            int64_t result = static_cast<int64_t>(original.intVal) + static_cast<int64_t>(delta);
            if (result > INT_MAX || result < INT_MIN)
                throw std::runtime_error("[VDX] Integer overflow in increment/decrement at line " + std::to_string(currentLine));
            *var = Value::makeInt(static_cast<int>(result));
            if (incDec->isPrefix) return Value::makeInt(static_cast<int>(result));
        } else {
            *var = Value::makeFloat(newVal);
            if (incDec->isPrefix) return Value::makeFloat(newVal);
        }
        return original;
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
            if (l == INT_MIN && r == -1) {
                throw std::runtime_error("[VDX] Integer overflow in division (INT_MIN / -1) at line " + std::to_string(currentLine));
            }
            return Value::makeInt(l / r);
        }
        if (expr->op == "==") return Value::makeBool(l == r);
        if (expr->op == "!=") return Value::makeBool(l != r);
        if (expr->op == "<") return Value::makeBool(l < r);
        if (expr->op == ">") return Value::makeBool(l > r);
        if (expr->op == "<=") return Value::makeBool(l <= r);
        if (expr->op == ">=") return Value::makeBool(l >= r);
    }

    // String equality and lexicographic ordering
    if (left.type == Value::STRING && right.type == Value::STRING) {
        if (expr->op == "==") return Value::makeBool(left.strVal == right.strVal);
        if (expr->op == "!=") return Value::makeBool(left.strVal != right.strVal);
        if (expr->op == "<") return Value::makeBool(left.strVal < right.strVal);
        if (expr->op == ">") return Value::makeBool(left.strVal > right.strVal);
        if (expr->op == "<=") return Value::makeBool(left.strVal <= right.strVal);
        if (expr->op == ">=") return Value::makeBool(left.strVal >= right.strVal);
    }

    // Bool equality
    if (left.type == Value::BOOL && right.type == Value::BOOL) {
        if (expr->op == "==") return Value::makeBool(left.boolVal == right.boolVal);
        if (expr->op == "!=") return Value::makeBool(left.boolVal != right.boolVal);
    }

    throw std::runtime_error("[VDX] Invalid operator '" + expr->op +
        "' for types '" + left.typeName() + "' and '" + right.typeName() +
        "' at line " + std::to_string(currentLine));
}
