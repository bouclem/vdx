#pragma once
#include "../interpreter.h"

// ── Math Module (v0.0.9) ──
// Provides math functions for the VDX programming language

namespace MathModule {
    // Initialize and register all math functions with the interpreter
    void registerMath(Interpreter& interp);
    
    // Math functions
    Value sqrt_builtin(const std::vector<Value>& args, int line);
    Value pow_builtin(const std::vector<Value>& args, int line);
    Value abs_builtin(const std::vector<Value>& args, int line);
    Value sin_builtin(const std::vector<Value>& args, int line);
    Value cos_builtin(const std::vector<Value>& args, int line);
    Value tan_builtin(const std::vector<Value>& args, int line);
    Value floor_builtin(const std::vector<Value>& args, int line);
    Value ceil_builtin(const std::vector<Value>& args, int line);
    Value round_builtin(const std::vector<Value>& args, int line);
    Value min_builtin(const std::vector<Value>& args, int line);
    Value max_builtin(const std::vector<Value>& args, int line);
    Value random_builtin(const std::vector<Value>& args, int line);
    Value pi_builtin(const std::vector<Value>& args, int line);
}
