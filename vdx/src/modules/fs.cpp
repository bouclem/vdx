#include "fs.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <system_error>
#include <algorithm>

// ── Filesystem Module Implementation ──

namespace FSModule {

// Optional sandbox root: when set via fs.setRoot(dir), every fs.* path is
// canonicalized and must resolve inside the root. Off by default.
static std::filesystem::path g_sandboxRoot;
static bool g_hasRoot = false;

// Resolve a user-supplied path against the sandbox (if any).
static std::string resolvePath(const std::string& path, int line) {
    if (!g_hasRoot) return path;
    std::error_code ec;
    std::filesystem::path full = std::filesystem::weakly_canonical(
        std::filesystem::path(path).is_absolute()
            ? std::filesystem::path(path)
            : g_sandboxRoot / path,
        ec);
    if (ec) {
        throw std::runtime_error("[VDX] fs: cannot resolve path '" + path +
            "' at line " + std::to_string(line));
    }
    std::string rootStr = g_sandboxRoot.generic_string();
    std::string fullStr = full.generic_string();
    if (fullStr.size() < rootStr.size() ||
        fullStr.compare(0, rootStr.size(), rootStr) != 0 ||
        (fullStr.size() > rootStr.size() && fullStr[rootStr.size()] != '/')) {
        throw std::runtime_error("[VDX] fs: path '" + path +
            "' is outside the sandbox root at line " + std::to_string(line));
    }
    return fullStr;
}

static void checkStringArgs(const std::vector<Value>& args, size_t count,
                            const char* name, int line) {
    for (size_t i = 0; i < count; i++) {
        if (args[i].type != Value::STRING) {
            throw std::runtime_error(std::string("[VDX] fs.") + name +
                "() expects string arguments at line " + std::to_string(line));
        }
    }
}

Value readFile_builtin(const std::vector<Value>& args, int line) {
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] fs.readFile() expects 1 argument (path) at line " + std::to_string(line));
    }
    checkStringArgs(args, 1, "readFile", line);

    std::ifstream file(resolvePath(args[0].strVal, line));
    if (!file.is_open()) {
        throw std::runtime_error("[VDX] Cannot read file '" + args[0].strVal + "' at line " + std::to_string(line));
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return Value::makeString(buffer.str());
}

Value writeFile_builtin(const std::vector<Value>& args, int line) {
    if (args.size() != 2) {
        throw std::runtime_error("[VDX] fs.writeFile() expects 2 arguments (path, content) at line " + std::to_string(line));
    }
    checkStringArgs(args, 2, "writeFile", line);

    std::ofstream file(resolvePath(args[0].strVal, line));
    if (!file.is_open()) {
        throw std::runtime_error("[VDX] Cannot write to file '" + args[0].strVal + "' at line " + std::to_string(line));
    }

    file << args[1].strVal;
    file.close();
    return Value::makeVoid();
}

Value appendFile_builtin(const std::vector<Value>& args, int line) {
    if (args.size() != 2) {
        throw std::runtime_error("[VDX] fs.append() expects 2 arguments (path, content) at line " + std::to_string(line));
    }
    checkStringArgs(args, 2, "append", line);

    std::ofstream file(resolvePath(args[0].strVal, line), std::ios::app);
    if (!file.is_open()) {
        throw std::runtime_error("[VDX] Cannot append to file '" + args[0].strVal + "' at line " + std::to_string(line));
    }

    file << args[1].strVal;
    file.close();
    return Value::makeVoid();
}

Value exists_builtin(const std::vector<Value>& args, int line) {
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] fs.exists() expects 1 argument (path) at line " + std::to_string(line));
    }
    checkStringArgs(args, 1, "exists", line);

    std::error_code ec;
    return Value::makeBool(std::filesystem::exists(resolvePath(args[0].strVal, line), ec));
}

Value listDir_builtin(const std::vector<Value>& args, int line) {
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] fs.listDir() expects 1 argument (dir) at line " + std::to_string(line));
    }
    checkStringArgs(args, 1, "listDir", line);

    std::string dir = resolvePath(args[0].strVal, line);
    std::error_code ec;
    if (!std::filesystem::is_directory(dir, ec)) {
        throw std::runtime_error("[VDX] fs.listDir() '" + args[0].strVal +
            "' is not a directory at line " + std::to_string(line));
    }

    std::vector<std::string> names;
    std::filesystem::directory_iterator it(dir, ec);
    if (ec) {
        throw std::runtime_error("[VDX] fs.listDir() cannot read '" + args[0].strVal +
            "' at line " + std::to_string(line));
    }
    for (const auto& entry : it) {
        names.push_back(entry.path().filename().string());
    }
    std::sort(names.begin(), names.end());

    std::vector<Value> result;
    result.reserve(names.size());
    for (auto& n : names) result.push_back(Value::makeString(n));
    return Value::makeArray(std::move(result));
}

Value setRoot_builtin(const std::vector<Value>& args, int line) {
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] fs.setRoot() expects 1 argument (dir, or \"\" to clear) at line " + std::to_string(line));
    }
    checkStringArgs(args, 1, "setRoot", line);

    if (args[0].strVal.empty()) {
        g_hasRoot = false;
        g_sandboxRoot.clear();
        return Value::makeVoid();
    }

    std::error_code ec;
    std::filesystem::path canon = std::filesystem::weakly_canonical(args[0].strVal, ec);
    if (ec || !std::filesystem::exists(canon) || !std::filesystem::is_directory(canon)) {
        throw std::runtime_error("[VDX] fs.setRoot() requires an existing directory at line " +
            std::to_string(line));
    }
    g_sandboxRoot = canon;
    g_hasRoot = true;
    return Value::makeVoid();
}

void registerFS(Interpreter& interp) {
    interp.registerModuleFunc("fs.readFile", readFile_builtin);
    interp.registerModuleFunc("fs.writeFile", writeFile_builtin);
    interp.registerModuleFunc("fs.append", appendFile_builtin);
    interp.registerModuleFunc("fs.exists", exists_builtin);
    interp.registerModuleFunc("fs.listDir", listDir_builtin);
    interp.registerModuleFunc("fs.setRoot", setRoot_builtin);
}

}
