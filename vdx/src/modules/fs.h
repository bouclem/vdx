#pragma once
#include "../interpreter.h"
#include <string>
#include <vector>

// ── Filesystem Module ──
// Provides file I/O operations for the VDX programming language.
// fs.setRoot(dir) enables an optional sandbox: once set, all fs.* paths are
// canonicalized and must stay under the root. Unset = unrestricted (default).

namespace FSModule {
    // Initialize and register all fs functions with the interpreter
    void registerFS(Interpreter& interp);

    // FS module functions
    Value readFile_builtin(const std::vector<Value>& args, int line);
    Value writeFile_builtin(const std::vector<Value>& args, int line);
    Value appendFile_builtin(const std::vector<Value>& args, int line);
    Value exists_builtin(const std::vector<Value>& args, int line);
    Value listDir_builtin(const std::vector<Value>& args, int line);
    Value setRoot_builtin(const std::vector<Value>& args, int line);
}
