#pragma once
#include "../interpreter.h"

// ── Math Module (v0.0.9) ──
// Provides math functions for the VDX programming language

namespace MathModule {
    // Initialize and register all math functions with the interpreter
    void registerMath(Interpreter& interp);
    
    // Math functions
    Value sqrt_builtin(const std::vector<Value>& args);
    Value pow_builtin(const std::vector<Value>& args);
    Value abs_builtin(const std::vector<Value>& args);
    Value sin_builtin(const std::vector<Value>& args);
    Value cos_builtin(const std::vector<Value>& args);
    Value tan_builtin(const std::vector<Value>& args);
    Value floor_builtin(const std::vector<Value>& args);
    Value ceil_builtin(const std::vector<Value>& args);
    Value round_builtin(const std::vector<Value>& args);
    Value min_builtin(const std::vector<Value>& args);
    Value max_builtin(const std::vector<Value>& args);
    Value random_builtin(const std::vector<Value>& args);
    Value pi_builtin(const std::vector<Value>& args);
}
