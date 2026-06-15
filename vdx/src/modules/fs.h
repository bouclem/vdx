#pragma once
#include "../interpreter.h"
#include <string>
#include <vector>

// ── Filesystem Module (v0.0.12) ──
// Provides file I/O operations for the VDX programming language

namespace FSModule {
    // Initialize and register all fs functions with the interpreter
    void registerFS(Interpreter& interp);
    
    // FS module functions
    Value readFile_builtin(const std::vector<Value>& args, int line);
    Value writeFile_builtin(const std::vector<Value>& args, int line);
}
