#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include "math.h"
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <algorithm>
#include <random>
#include <climits>
#include <functional>

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

static void checkNumeric(const Value& arg, const std::string& funcName) {
    if (!arg.isNumeric()) {
        throw std::runtime_error("[VDX] math." + funcName + "() expects numeric argument");
    }
}

Value sqrt_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.sqrt() expects 1 argument");
    }
    checkNumeric(args[0], "sqrt");
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
    checkNumeric(args[0], "pow");
    checkNumeric(args[1], "pow");
    double base = args[0].toDouble();
    double exp = args[1].toDouble();
    return Value::makeFloat(std::pow(base, exp));
}

Value abs_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.abs() expects 1 argument");
    }
    checkNumeric(args[0], "abs");
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
    checkNumeric(args[0], "sin");
    return Value::makeFloat(std::sin(args[0].toDouble()));
}

Value cos_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.cos() expects 1 argument (radians)");
    }
    checkNumeric(args[0], "cos");
    return Value::makeFloat(std::cos(args[0].toDouble()));
}

Value tan_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.tan() expects 1 argument (radians)");
    }
    checkNumeric(args[0], "tan");
    return Value::makeFloat(std::tan(args[0].toDouble()));
}

Value floor_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.floor() expects 1 argument");
    }
    checkNumeric(args[0], "floor");
    if (args[0].type == Value::INT) {
        return Value::makeInt(args[0].intVal);
    }
    double d = std::floor(args[0].toDouble());
    if (d > static_cast<double>(INT_MAX) || d < static_cast<double>(INT_MIN))
        throw std::runtime_error("[VDX] math.floor() result out of int range");
    return Value::makeInt(static_cast<int>(d));
}

Value ceil_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.ceil() expects 1 argument");
    }
    checkNumeric(args[0], "ceil");
    if (args[0].type == Value::INT) {
        return Value::makeInt(args[0].intVal);
    }
    double d = std::ceil(args[0].toDouble());
    if (d > static_cast<double>(INT_MAX) || d < static_cast<double>(INT_MIN))
        throw std::runtime_error("[VDX] math.ceil() result out of int range");
    return Value::makeInt(static_cast<int>(d));
}

Value round_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.round() expects 1 argument");
    }
    checkNumeric(args[0], "round");
    if (args[0].type == Value::INT) {
        return Value::makeInt(args[0].intVal);
    }
    double d = std::round(args[0].toDouble());
    if (d > static_cast<double>(INT_MAX) || d < static_cast<double>(INT_MIN))
        throw std::runtime_error("[VDX] math.round() result out of int range");
    return Value::makeInt(static_cast<int>(d));
}

Value min_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() < 1) {
        throw std::runtime_error("[VDX] math.min() expects at least 1 argument");
    }
    double min_val = args[0].toDouble();
    bool anyFloat = false;
    for (size_t i = 0; i < args.size(); i++) {
        checkNumeric(args[i], "min");
        if (args[i].type == Value::FLOAT) anyFloat = true;
        double v = args[i].toDouble();
        if (v < min_val) min_val = v;
    }
    if (anyFloat) return Value::makeFloat(min_val);
    return Value::makeInt(static_cast<int>(min_val));
}

Value max_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() < 1) {
        throw std::runtime_error("[VDX] math.max() expects at least 1 argument");
    }
    double max_val = args[0].toDouble();
    bool anyFloat = false;
    for (size_t i = 0; i < args.size(); i++) {
        checkNumeric(args[i], "max");
        if (args[i].type == Value::FLOAT) anyFloat = true;
        double v = args[i].toDouble();
        if (v > max_val) max_val = v;
    }
    if (anyFloat) return Value::makeFloat(max_val);
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
        // Return random int between 0 and max (inclusive)
        checkNumeric(args[0], "random");
        int max_val = static_cast<int>(args[0].toDouble());
        if (max_val < 0) {
            throw std::runtime_error("[VDX] math.random() argument must be non-negative");
        }
        std::uniform_int_distribution<int> dist(0, max_val);
        return Value::makeInt(dist(rng));
    }
    if (args.size() == 2) {
        // Return random int between min and max (inclusive)
        checkNumeric(args[0], "random");
        checkNumeric(args[1], "random");
        int min_val = static_cast<int>(args[0].toDouble());
        int max_val = static_cast<int>(args[1].toDouble());
        if (max_val < min_val) {
            throw std::runtime_error("[VDX] math.random() max must be greater than or equal to min");
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

// ── v0.1.3 additions ──

Value log_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.log() expects 1 argument");
    }
    checkNumeric(args[0], "log");
    double x = args[0].toDouble();
    if (x <= 0) {
        throw std::runtime_error("[VDX] math.log() requires a positive number");
    }
    return Value::makeFloat(std::log(x));
}

Value log2_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.log2() expects 1 argument");
    }
    checkNumeric(args[0], "log2");
    double x = args[0].toDouble();
    if (x <= 0) {
        throw std::runtime_error("[VDX] math.log2() requires a positive number");
    }
    return Value::makeFloat(std::log2(x));
}

Value log10_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.log10() expects 1 argument");
    }
    checkNumeric(args[0], "log10");
    double x = args[0].toDouble();
    if (x <= 0) {
        throw std::runtime_error("[VDX] math.log10() requires a positive number");
    }
    return Value::makeFloat(std::log10(x));
}

Value exp_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.exp() expects 1 argument");
    }
    checkNumeric(args[0], "exp");
    return Value::makeFloat(std::exp(args[0].toDouble()));
}

Value cbrt_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.cbrt() expects 1 argument");
    }
    checkNumeric(args[0], "cbrt");
    return Value::makeFloat(std::cbrt(args[0].toDouble()));
}

Value asin_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.asin() expects 1 argument");
    }
    checkNumeric(args[0], "asin");
    double x = args[0].toDouble();
    if (x < -1.0 || x > 1.0) {
        throw std::runtime_error("[VDX] math.asin() requires argument in range [-1, 1]");
    }
    return Value::makeFloat(std::asin(x));
}

Value acos_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.acos() expects 1 argument");
    }
    checkNumeric(args[0], "acos");
    double x = args[0].toDouble();
    if (x < -1.0 || x > 1.0) {
        throw std::runtime_error("[VDX] math.acos() requires argument in range [-1, 1]");
    }
    return Value::makeFloat(std::acos(x));
}

Value atan_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.atan() expects 1 argument");
    }
    checkNumeric(args[0], "atan");
    return Value::makeFloat(std::atan(args[0].toDouble()));
}

Value atan2_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 2) {
        throw std::runtime_error("[VDX] math.atan2() expects 2 arguments (y, x)");
    }
    checkNumeric(args[0], "atan2");
    checkNumeric(args[1], "atan2");
    return Value::makeFloat(std::atan2(args[0].toDouble(), args[1].toDouble()));
}

Value degrees_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.degrees() expects 1 argument");
    }
    checkNumeric(args[0], "degrees");
    return Value::makeFloat(args[0].toDouble() * (180.0 / M_PI));
}

Value radians_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.radians() expects 1 argument");
    }
    checkNumeric(args[0], "radians");
    return Value::makeFloat(args[0].toDouble() * (M_PI / 180.0));
}

Value gcd_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 2) {
        throw std::runtime_error("[VDX] math.gcd() expects 2 arguments (a, b)");
    }
    checkNumeric(args[0], "gcd");
    checkNumeric(args[1], "gcd");
    long long a = static_cast<long long>(args[0].toDouble());
    long long b = static_cast<long long>(args[1].toDouble());
    if (a == 0 && b == 0) {
        throw std::runtime_error("[VDX] math.gcd() of 0 and 0 is undefined");
    }
    a = std::abs(a);
    b = std::abs(b);
    while (b != 0) {
        long long t = b;
        b = a % b;
        a = t;
    }
    if (a > static_cast<long long>(INT_MAX)) {
        throw std::runtime_error("[VDX] math.gcd() result out of int range");
    }
    return Value::makeInt(static_cast<int>(a));
}

Value sign_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.sign() expects 1 argument");
    }
    checkNumeric(args[0], "sign");
    double x = args[0].toDouble();
    if (x > 0) return Value::makeInt(1);
    if (x < 0) return Value::makeInt(-1);
    return Value::makeInt(0);
}

Value clamp_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 3) {
        throw std::runtime_error("[VDX] math.clamp() expects 3 arguments (value, min, max)");
    }
    checkNumeric(args[0], "clamp");
    checkNumeric(args[1], "clamp");
    checkNumeric(args[2], "clamp");
    double v = args[0].toDouble();
    double lo = args[1].toDouble();
    double hi = args[2].toDouble();
    if (lo > hi) {
        throw std::runtime_error("[VDX] math.clamp() min must be <= max");
    }
    double result = std::max(lo, std::min(v, hi));
    if (args[0].type == Value::INT && args[1].type == Value::INT && args[2].type == Value::INT) {
        return Value::makeInt(static_cast<int>(result));
    }
    return Value::makeFloat(result);
}

Value factorial_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.factorial() expects 1 argument");
    }
    checkNumeric(args[0], "factorial");
    long long n = static_cast<long long>(args[0].toDouble());
    if (n < 0) {
        throw std::runtime_error("[VDX] math.factorial() requires a non-negative integer");
    }
    long long result = 1;
    for (long long i = 2; i <= n; i++) {
        result *= i;
        if (result > static_cast<long long>(INT_MAX)) {
            throw std::runtime_error("[VDX] math.factorial() result out of int range (max is 12!)");
        }
    }
    return Value::makeInt(static_cast<int>(result));
}

Value fibonacci_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.fibonacci() expects 1 argument");
    }
    checkNumeric(args[0], "fibonacci");
    long long n = static_cast<long long>(args[0].toDouble());
    if (n < 0) {
        throw std::runtime_error("[VDX] math.fibonacci() requires a non-negative integer");
    }
    // Fast doubling: O(log n) instead of O(n) iterative or O(2^n) naive recursive
    // Returns F(n) and F(n+1) as a pair
    // Base: F(0)=0, F(1)=1
    std::function<std::pair<long long, long long>(long long)> fib =
        [&fib](long long k) -> std::pair<long long, long long> {
        if (k == 0) return {0, 1};
        auto [a, b] = fib(k / 2);  // a = F(k/2), b = F(k/2 + 1)
        // F(2n)   = F(n) * (2*F(n+1) - F(n))
        // F(2n+1) = F(n+1)^2 + F(n)^2
        long long c = a * (2 * b - a);
        long long d = a * a + b * b;
        if (k % 2 == 0) return {c, d};
        return {d, c + d};
    };
    auto [result, next] = fib(n);
    (void)next;
    if (result > static_cast<long long>(INT_MAX)) {
        throw std::runtime_error("[VDX] math.fibonacci() result out of int range (max is fib(46))");
    }
    return Value::makeInt(static_cast<int>(result));
}

// ── v0.1.4 additions ──

Value isPrime_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.isPrime() expects 1 argument");
    }
    checkNumeric(args[0], "isPrime");
    long long n = static_cast<long long>(args[0].toDouble());
    if (n < 2) return Value::makeBool(false);
    if (n < 4) return Value::makeBool(true);
    if (n % 2 == 0) return Value::makeBool(false);
    if (n % 3 == 0) return Value::makeBool(false);
    for (long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return Value::makeBool(false);
    }
    return Value::makeBool(true);
}

Value primes_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.primes() expects 1 argument");
    }
    checkNumeric(args[0], "primes");
    long long n = static_cast<long long>(args[0].toDouble());
    if (n < 2) return Value::makeArray({});

    std::vector<bool> sieve(static_cast<size_t>(n + 1), true);
    sieve[0] = sieve[1] = false;
    for (long long i = 2; i * i <= n; i++) {
        if (sieve[static_cast<size_t>(i)]) {
            for (long long j = i * i; j <= n; j += i) {
                sieve[static_cast<size_t>(j)] = false;
            }
        }
    }

    std::vector<Value> result;
    for (long long i = 2; i <= n; i++) {
        if (sieve[static_cast<size_t>(i)]) {
            result.push_back(Value::makeInt(static_cast<int>(i)));
        }
    }
    return Value::makeArray(result);
}

Value primeCount_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.primeCount() expects 1 argument");
    }
    checkNumeric(args[0], "primeCount");
    long long n = static_cast<long long>(args[0].toDouble());
    if (n < 2) return Value::makeInt(0);

    std::vector<bool> sieve(static_cast<size_t>(n + 1), true);
    sieve[0] = sieve[1] = false;
    int count = 0;
    for (long long i = 2; i * i <= n; i++) {
        if (sieve[static_cast<size_t>(i)]) {
            for (long long j = i * i; j <= n; j += i) {
                sieve[static_cast<size_t>(j)] = false;
            }
        }
    }
    for (long long i = 2; i <= n; i++) {
        if (sieve[static_cast<size_t>(i)]) count++;
    }
    return Value::makeInt(count);
}

Value sort_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.sort() expects 1 argument (array)");
    }
    if (args[0].type != Value::ARRAY) {
        throw std::runtime_error("[VDX] math.sort() expects an array argument");
    }
    std::vector<Value> result = args[0].arrVal;
    std::sort(result.begin(), result.end(), [](const Value& a, const Value& b) {
        return a.toDouble() < b.toDouble();
    });
    return Value::makeArray(result);
}

Value sortDesc_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.sortDesc() expects 1 argument (array)");
    }
    if (args[0].type != Value::ARRAY) {
        throw std::runtime_error("[VDX] math.sortDesc() expects an array argument");
    }
    std::vector<Value> result = args[0].arrVal;
    std::sort(result.begin(), result.end(), [](const Value& a, const Value& b) {
        return a.toDouble() > b.toDouble();
    });
    return Value::makeArray(result);
}

Value count_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 2) {
        throw std::runtime_error("[VDX] math.count() expects 2 arguments (array, value)");
    }
    if (args[0].type != Value::ARRAY) {
        throw std::runtime_error("[VDX] math.count() expects an array as first argument");
    }
    int count = 0;
    for (const auto& v : args[0].arrVal) {
        if (v.type == args[1].type) {
            if (v.type == Value::STRING && v.strVal == args[1].strVal) count++;
            else if (v.type == Value::INT && v.intVal == args[1].intVal) count++;
            else if (v.type == Value::FLOAT && v.floatVal == args[1].floatVal) count++;
            else if (v.type == Value::BOOL && v.boolVal == args[1].boolVal) count++;
        }
    }
    return Value::makeInt(count);
}

Value lcm_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 2) {
        throw std::runtime_error("[VDX] math.lcm() expects 2 arguments (a, b)");
    }
    checkNumeric(args[0], "lcm");
    checkNumeric(args[1], "lcm");
    long long a = static_cast<long long>(args[0].toDouble());
    long long b = static_cast<long long>(args[1].toDouble());
    if (a == 0 || b == 0) return Value::makeInt(0);
    a = std::abs(a);
    b = std::abs(b);
    long long g = a;
    long long t = b;
    while (t != 0) {
        long long tmp = g % t;
        g = t;
        t = tmp;
    }
    long long result = (a / g) * b;
    if (result > static_cast<long long>(INT_MAX)) {
        throw std::runtime_error("[VDX] math.lcm() result out of int range");
    }
    return Value::makeInt(static_cast<int>(result));
}

Value sum_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.sum() expects 1 argument (array)");
    }
    if (args[0].type != Value::ARRAY) {
        throw std::runtime_error("[VDX] math.sum() expects an array argument");
    }
    double total = 0.0;
    bool anyFloat = false;
    for (const auto& v : args[0].arrVal) {
        checkNumeric(v, "sum");
        if (v.type == Value::FLOAT) anyFloat = true;
        total += v.toDouble();
    }
    if (anyFloat) return Value::makeFloat(total);
    return Value::makeInt(static_cast<int>(total));
}

Value mean_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] math.mean() expects 1 argument (array)");
    }
    if (args[0].type != Value::ARRAY) {
        throw std::runtime_error("[VDX] math.mean() expects an array argument");
    }
    if (args[0].arrVal.empty()) {
        throw std::runtime_error("[VDX] math.mean() array must not be empty");
    }
    double total = 0.0;
    for (const auto& v : args[0].arrVal) {
        checkNumeric(v, "mean");
        total += v.toDouble();
    }
    return Value::makeFloat(total / static_cast<double>(args[0].arrVal.size()));
}

Value comb_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 2) {
        throw std::runtime_error("[VDX] math.comb() expects 2 arguments (n, k)");
    }
    checkNumeric(args[0], "comb");
    checkNumeric(args[1], "comb");
    long long n = static_cast<long long>(args[0].toDouble());
    long long k = static_cast<long long>(args[1].toDouble());
    if (n < 0 || k < 0) {
        throw std::runtime_error("[VDX] math.comb() requires non-negative integers");
    }
    if (k > n) return Value::makeInt(0);
    if (k == 0 || k == n) return Value::makeInt(1);
    if (k > n - k) k = n - k;
    long long result = 1;
    for (long long i = 0; i < k; i++) {
        result = result * (n - i) / (i + 1);
        if (result > static_cast<long long>(INT_MAX)) {
            throw std::runtime_error("[VDX] math.comb() result out of int range");
        }
    }
    return Value::makeInt(static_cast<int>(result));
}

Value hypot_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 2) {
        throw std::runtime_error("[VDX] math.hypot() expects 2 arguments (x, y)");
    }
    checkNumeric(args[0], "hypot");
    checkNumeric(args[1], "hypot");
    return Value::makeFloat(std::hypot(args[0].toDouble(), args[1].toDouble()));
}

Value lerp_builtin(const std::vector<Value>& args, int line) {
    (void)line;
    if (args.size() != 3) {
        throw std::runtime_error("[VDX] math.lerp() expects 3 arguments (a, b, t)");
    }
    checkNumeric(args[0], "lerp");
    checkNumeric(args[1], "lerp");
    checkNumeric(args[2], "lerp");
    double a = args[0].toDouble();
    double b = args[1].toDouble();
    double t = args[2].toDouble();
    return Value::makeFloat(a + (b - a) * t);
}

Value e_builtin(const std::vector<Value>& args, int line) {
    (void)args;
    (void)line;
    return Value::makeFloat(2.71828182845904523536);
}

Value tau_builtin(const std::vector<Value>& args, int line) {
    (void)args;
    (void)line;
    return Value::makeFloat(6.28318530717958647692);
}

void registerMath(Interpreter& interp) {
    interp.registerModuleFunc("math.sqrt", sqrt_builtin);
    interp.registerModuleFunc("math.pow", pow_builtin);
    interp.registerModuleFunc("math.abs", abs_builtin);
    interp.registerModuleFunc("math.sin", sin_builtin);
    interp.registerModuleFunc("math.cos", cos_builtin);
    interp.registerModuleFunc("math.tan", tan_builtin);
    interp.registerModuleFunc("math.floor", floor_builtin);
    interp.registerModuleFunc("math.ceil", ceil_builtin);
    interp.registerModuleFunc("math.round", round_builtin);
    interp.registerModuleFunc("math.min", min_builtin);
    interp.registerModuleFunc("math.max", max_builtin);
    interp.registerModuleFunc("math.random", random_builtin);
    interp.registerModuleFunc("math.pi", pi_builtin);
    // v0.1.3 additions
    interp.registerModuleFunc("math.log", log_builtin);
    interp.registerModuleFunc("math.log2", log2_builtin);
    interp.registerModuleFunc("math.log10", log10_builtin);
    interp.registerModuleFunc("math.exp", exp_builtin);
    interp.registerModuleFunc("math.cbrt", cbrt_builtin);
    interp.registerModuleFunc("math.asin", asin_builtin);
    interp.registerModuleFunc("math.acos", acos_builtin);
    interp.registerModuleFunc("math.atan", atan_builtin);
    interp.registerModuleFunc("math.atan2", atan2_builtin);
    interp.registerModuleFunc("math.degrees", degrees_builtin);
    interp.registerModuleFunc("math.radians", radians_builtin);
    interp.registerModuleFunc("math.gcd", gcd_builtin);
    interp.registerModuleFunc("math.sign", sign_builtin);
    interp.registerModuleFunc("math.clamp", clamp_builtin);
    interp.registerModuleFunc("math.factorial", factorial_builtin);
    interp.registerModuleFunc("math.fibonacci", fibonacci_builtin);
    // v0.1.4
    interp.registerModuleFunc("math.isPrime", isPrime_builtin);
    interp.registerModuleFunc("math.primes", primes_builtin);
    interp.registerModuleFunc("math.primeCount", primeCount_builtin);
    interp.registerModuleFunc("math.sort", sort_builtin);
    interp.registerModuleFunc("math.sortDesc", sortDesc_builtin);
    interp.registerModuleFunc("math.count", count_builtin);
    interp.registerModuleFunc("math.lcm", lcm_builtin);
    interp.registerModuleFunc("math.sum", sum_builtin);
    interp.registerModuleFunc("math.mean", mean_builtin);
    interp.registerModuleFunc("math.comb", comb_builtin);
    interp.registerModuleFunc("math.hypot", hypot_builtin);
    interp.registerModuleFunc("math.lerp", lerp_builtin);
    interp.registerModuleFunc("math.e", e_builtin);
    interp.registerModuleFunc("math.tau", tau_builtin);
}

} // namespace MathModule
