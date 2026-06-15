#include "fs.h"
#include <fstream>
#include <sstream>

// ── Filesystem Module Implementation (v0.0.12) ──

namespace FSModule {

Value readFile_builtin(const std::vector<Value>& args, int line) {
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] fs.readFile() expects 1 argument (path)");
    }
    if (args[0].type != Value::STRING) {
        throw std::runtime_error("[VDX] fs.readFile() expects string path at line " + std::to_string(line));
    }
    
    std::ifstream file(args[0].strVal);
    if (!file.is_open()) {
        throw std::runtime_error("[VDX] Cannot read file '" + args[0].strVal + "' at line " + std::to_string(line));
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return Value::makeString(buffer.str());
}

Value writeFile_builtin(const std::vector<Value>& args, int line) {
    if (args.size() != 2) {
        throw std::runtime_error("[VDX] fs.writeFile() expects 2 arguments (path, content)");
    }
    if (args[0].type != Value::STRING || args[1].type != Value::STRING) {
        throw std::runtime_error("[VDX] fs.writeFile() expects string arguments at line " + std::to_string(line));
    }
    
    std::ofstream file(args[0].strVal);
    if (!file.is_open()) {
        throw std::runtime_error("[VDX] Cannot write to file '" + args[0].strVal + "' at line " + std::to_string(line));
    }
    
    file << args[1].strVal;
    file.close();
    return Value::makeVoid();
}

void registerFS(Interpreter& interp) {
    interp.registerModuleFunc("fs.readFile", readFile_builtin);
    interp.registerModuleFunc("fs.writeFile", writeFile_builtin);
}

}
