#include "math.h"
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <algorithm>
#include <random>

// ── Math Module Implementation (v0.0.9) ──

namespace MathModule {

static std::mt19937 rng;
static bool random_seeded = false;

static void seed_random() {
    if (!random_seeded) {
        std::random_device rd;
        rng.seed(rd());
        random_seeded = true;
    }
}

Value sqrt_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.sqrt() expects 1 argument");
    }
    double x = args[0].toDouble();
    if (x < 0) {
        throw std::runtime_error("[VDX] math.sqrt() cannot calculate square root of negative number");
    }
    return Value::makeFloat(std::sqrt(x));
}

Value pow_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 2) {
        throw std::runtime_error("[VDX] math.pow() expects 2 arguments (base, exponent)");
    }
    double base = args[0].toDouble();
    double exp = args[1].toDouble();
    return Value::makeFloat(std::pow(base, exp));
}

Value abs_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.abs() expects 1 argument");
    }
    if (args[0].type == Value::INT) {
        return Value::makeInt(std::abs(args[0].intVal));
    }
    return Value::makeFloat(std::abs(args[0].toDouble()));
}

Value sin_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.sin() expects 1 argument (radians)");
    }
    return Value::makeFloat(std::sin(args[0].toDouble()));
}

Value cos_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.cos() expects 1 argument (radians)");
    }
    return Value::makeFloat(std::cos(args[0].toDouble()));
}

Value tan_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.tan() expects 1 argument (radians)");
    }
    return Value::makeFloat(std::tan(args[0].toDouble()));
}

Value floor_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.floor() expects 1 argument");
    }
    return Value::makeFloat(std::floor(args[0].toDouble()));
}

Value ceil_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.ceil() expects 1 argument");
    }
    return Value::makeFloat(std::ceil(args[0].toDouble()));
}

Value round_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.round() expects 1 argument");
    }
    double x = args[0].toDouble();
    if (args[0].type == Value::INT) {
        return Value::makeInt(args[0].intVal);
    }
    return Value::makeFloat(std::round(x));
}

Value min_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() < 1) {
        throw std::runtime_error("[VDX] math.min() expects at least 1 argument");
    }
    double min_val = args[0].toDouble();
    for (size_t i = 1; i < args.size(); i++) {
        min_val = std::min(min_val, args[i].toDouble());
    }
    // Return int if all args are int
    for (size_t i = 0; i < args.size(); i++) {
        if (args[i].type == Value::FLOAT) {
            return Value::makeFloat(min_val);
        }
    }
    return Value::makeInt(static_cast<int>(min_val));
}

Value max_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() < 1) {
        throw std::runtime_error("[VDX] math.max() expects at least 1 argument");
    }
    double max_val = args[0].toDouble();
    for (size_t i = 1; i < args.size(); i++) {
        max_val = std::max(max_val, args[i].toDouble());
    }
    // Return int if all args are int
    for (size_t i = 0; i < args.size(); i++) {
        if (args[i].type == Value::FLOAT) {
            return Value::makeFloat(max_val);
        }
    }
    return Value::makeInt(static_cast<int>(max_val));
}

Value random_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    seed_random();
    if (args.size() == 0) {
        // Return random float between 0 and 1
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        return Value::makeFloat(dist(rng));
    }
    if (args.size() == 1) {
        // Return random int between 0 and max (exclusive)
        int max_val = static_cast<int>(args[0].toDouble());
        if (max_val <= 0) {
            throw std::runtime_error("[VDX] math.random() argument must be positive");
        }
        std::uniform_int_distribution<int> dist(0, max_val - 1);
        return Value::makeInt(dist(rng));
    }
    if (args.size() == 2) {
        // Return random int between min and max (inclusive)
        int min_val = static_cast<int>(args[0].toDouble());
        int max_val = static_cast<int>(args[1].toDouble());
        if (max_val <= min_val) {
            throw std::runtime_error("[VDX] math.random() max must be greater than min");
        }
        std::uniform_int_distribution<int> dist(min_val, max_val);
        return Value::makeInt(dist(rng));
    }
    throw std::runtime_error("[VDX] math.random() expects 0, 1, or 2 arguments");
}

Value pi_builtin(const std::vector<Value>& args, int line) {
    (void)args; // unused
    (void)line;
    return Value::makeFloat(M_PI);
}

} // namespace MathModule
