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

    // v0.1.3 additions
    Value log_builtin(const std::vector<Value>& args, int line);
    Value log2_builtin(const std::vector<Value>& args, int line);
    Value log10_builtin(const std::vector<Value>& args, int line);
    Value exp_builtin(const std::vector<Value>& args, int line);
    Value cbrt_builtin(const std::vector<Value>& args, int line);
    Value asin_builtin(const std::vector<Value>& args, int line);
    Value acos_builtin(const std::vector<Value>& args, int line);
    Value atan_builtin(const std::vector<Value>& args, int line);
    Value atan2_builtin(const std::vector<Value>& args, int line);
    Value degrees_builtin(const std::vector<Value>& args, int line);
    Value radians_builtin(const std::vector<Value>& args, int line);
    Value gcd_builtin(const std::vector<Value>& args, int line);
    Value sign_builtin(const std::vector<Value>& args, int line);
    Value clamp_builtin(const std::vector<Value>& args, int line);
    Value factorial_builtin(const std::vector<Value>& args, int line);
    Value fibonacci_builtin(const std::vector<Value>& args, int line);

    // v0.1.4 additions — primes, sort, count, fast math
    Value isPrime_builtin(const std::vector<Value>& args, int line);
    Value primes_builtin(const std::vector<Value>& args, int line);
    Value primeCount_builtin(const std::vector<Value>& args, int line);
    Value sort_builtin(const std::vector<Value>& args, int line);
    Value sortDesc_builtin(const std::vector<Value>& args, int line);
    Value count_builtin(const std::vector<Value>& args, int line);
    Value lcm_builtin(const std::vector<Value>& args, int line);
    Value sum_builtin(const std::vector<Value>& args, int line);
    Value mean_builtin(const std::vector<Value>& args, int line);
    Value comb_builtin(const std::vector<Value>& args, int line);
    Value hypot_builtin(const std::vector<Value>& args, int line);
    Value lerp_builtin(const std::vector<Value>& args, int line);
    Value e_builtin(const std::vector<Value>& args, int line);
    Value tau_builtin(const std::vector<Value>& args, int line);
}
