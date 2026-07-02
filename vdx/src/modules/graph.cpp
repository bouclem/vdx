#include "graph.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <random>
#include <filesystem>

// ── Graph Module Implementation (v0.1.0) ──
// SVG-based plotting for VDX. No external dependencies.
// Plots are built in an internal SVG buffer and written to disk via graph.save()
// or opened in the default viewer via graph.show().
// TODO: Multiple series on the same plot
// TODO: Log-scale axis support
// TODO: Color/marker customization
// FIXME: Large datasets may produce very large SVG files — consider sampling

namespace GraphModule {

// ── Plot state ──
// Holds configuration and the current SVG content between graph.* calls.
struct PlotState {
    std::string svg;
    std::string title;
    std::string xlabel;
    std::string ylabel;
    std::string plotColor = "steelblue";
    std::string plotStroke = "navy";
    std::vector<std::string> legendLabels;
    bool showGrid = false;
    bool hasPlot = false;
};

// FIXME: PlotState is thread-local but still shared across multiple Interpreter
// instances on the same thread. A proper fix requires passing per-interpreter
// state through the ModuleFunc API, which is a larger architectural change.
static thread_local PlotState g_state;

// ── SVG helpers ──

static std::string escXml(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (char c : s) {
        switch (c) {
            case '<': out += "&lt;"; break;
            case '>': out += "&gt;"; break;
            case '&': out += "&amp;"; break;
            case '"': out += "&quot;"; break;
            case '\'': out += "&apos;"; break;
            case '\n': out += "&#10;"; break;
            case '\r': out += "&#13;"; break;
            case '\t': out += "&#9;"; break;
            default:
                if (static_cast<unsigned char>(c) < 0x20) {
                    out += "&#" + std::to_string(static_cast<unsigned char>(c)) + ";";
                } else {
                    out += c;
                }
                break;
        }
    }
    return out;
}

static std::string fmt(double v) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << v;
    return oss.str();
}

// SVG canvas dimensions
static const int SVG_W = 800;
static const int SVG_H = 600;
static const int MARGIN_L = 70;
static const int MARGIN_R = 30;
static const int MARGIN_T = 50;
static const int MARGIN_B = 60;
static const int PLOT_W = SVG_W - MARGIN_L - MARGIN_R;
static const int PLOT_H = SVG_H - MARGIN_T - MARGIN_B;

// Build SVG header + axes + title + labels, return the open body string position
// where plot-specific content should be inserted.
static std::string svgHeader(double xMin, double xMax, double yMin, double yMax) {
    std::ostringstream svg;

    svg << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << SVG_W
        << "\" height=\"" << SVG_H << "\" viewBox=\"0 0 " << SVG_W << " " << SVG_H << "\">\n";

    // Background
    svg << "  <rect width=\"" << SVG_W << "\" height=\"" << SVG_H
        << "\" fill=\"white\"/>\n";

    // Title
    if (!g_state.title.empty()) {
        svg << "  <text x=\"" << (SVG_W / 2) << "\" y=\"25\" text-anchor=\"middle\""
            << " font-family=\"sans-serif\" font-size=\"18\" font-weight=\"bold\">"
            << escXml(g_state.title) << "</text>\n";
    }

    // Plot area border
    svg << "  <rect x=\"" << MARGIN_L << "\" y=\"" << MARGIN_T
        << "\" width=\"" << PLOT_W << "\" height=\"" << PLOT_H
        << "\" fill=\"none\" stroke=\"black\" stroke-width=\"1\"/>\n";

    // X axis ticks and labels (5 ticks)
    for (int i = 0; i <= 5; i++) {
        double xVal = xMin + (xMax - xMin) * i / 5.0;
        int px = MARGIN_L + PLOT_W * i / 5;
        svg << "  <line x1=\"" << px << "\" y1=\"" << (MARGIN_T + PLOT_H)
            << "\" x2=\"" << px << "\" y2=\"" << (MARGIN_T + PLOT_H + 5)
            << "\" stroke=\"black\" stroke-width=\"1\"/>\n";
        svg << "  <text x=\"" << px << "\" y=\"" << (MARGIN_T + PLOT_H + 18)
            << "\" text-anchor=\"middle\" font-family=\"sans-serif\" font-size=\"11\">"
            << fmt(xVal) << "</text>\n";
    }

    // Y axis ticks and labels (5 ticks)
    for (int i = 0; i <= 5; i++) {
        double yVal = yMin + (yMax - yMin) * i / 5.0;
        int py = MARGIN_T + PLOT_H - PLOT_H * i / 5;
        svg << "  <line x1=\"" << (MARGIN_L - 5) << "\" y1=\"" << py
            << "\" x2=\"" << MARGIN_L << "\" y2=\"" << py
            << "\" stroke=\"black\" stroke-width=\"1\"/>\n";
        svg << "  <text x=\"" << (MARGIN_L - 8) << "\" y=\"" << (py + 4)
            << "\" text-anchor=\"end\" font-family=\"sans-serif\" font-size=\"11\">"
            << fmt(yVal) << "</text>\n";
    }

    // Grid lines
    if (g_state.showGrid) {
        for (int i = 1; i < 5; i++) {
            int px = MARGIN_L + PLOT_W * i / 5;
            svg << "  <line x1=\"" << px << "\" y1=\"" << MARGIN_T
                << "\" x2=\"" << px << "\" y2=\"" << (MARGIN_T + PLOT_H)
                << "\" stroke=\"#e0e0e0\" stroke-width=\"1\" stroke-dasharray=\"4,2\"/>\n";
        }
        for (int i = 1; i < 5; i++) {
            int py = MARGIN_T + PLOT_H - PLOT_H * i / 5;
            svg << "  <line x1=\"" << MARGIN_L << "\" y1=\"" << py
                << "\" x2=\"" << (MARGIN_L + PLOT_W) << "\" y2=\"" << py
                << "\" stroke=\"#e0e0e0\" stroke-width=\"1\" stroke-dasharray=\"4,2\"/>\n";
        }
    }

    // X label
    if (!g_state.xlabel.empty()) {
        svg << "  <text x=\"" << (MARGIN_L + PLOT_W / 2) << "\" y=\""
            << (SVG_H - 15) << "\" text-anchor=\"middle\""
            << " font-family=\"sans-serif\" font-size=\"13\">"
            << escXml(g_state.xlabel) << "</text>\n";
    }

    // Y label (rotated)
    if (!g_state.ylabel.empty()) {
        svg << "  <text x=\"15\" y=\"" << (MARGIN_T + PLOT_H / 2)
            << "\" text-anchor=\"middle\" transform=\"rotate(-90 15 "
            << (MARGIN_T + PLOT_H / 2) << ")\""
            << " font-family=\"sans-serif\" font-size=\"13\">"
            << escXml(g_state.ylabel) << "</text>\n";
    }

    // Legend
    if (!g_state.legendLabels.empty()) {
        int legendX = MARGIN_L + PLOT_W - 120;
        int legendY = MARGIN_T + 10;
        svg << "  <rect x=\"" << legendX << "\" y=\"" << legendY
            << "\" width=\"110\" height=\"" << (static_cast<int>(g_state.legendLabels.size()) * 18 + 8)
            << "\" fill=\"white\" fill-opacity=\"0.8\" stroke=\"#ccc\" stroke-width=\"1\"/>\n";
        for (size_t i = 0; i < g_state.legendLabels.size(); i++) {
            int ly = legendY + 14 + static_cast<int>(i) * 18;
            svg << "  <rect x=\"" << (legendX + 5) << "\" y=\"" << (ly - 8)
                << "\" width=\"10\" height=\"10\" fill=\"" << g_state.plotColor
                << "\" stroke=\"" << g_state.plotStroke << "\" stroke-width=\"0.5\"/>\n";
            std::string lbl = g_state.legendLabels[i];
            if (lbl.size() > 14) lbl = lbl.substr(0, 14);
            svg << "  <text x=\"" << (legendX + 20) << "\" y=\"" << ly
                << "\" font-family=\"sans-serif\" font-size=\"11\">"
                << escXml(lbl) << "</text>\n";
        }
    }

    return svg.str();
}

static std::string svgFooter() {
    return "</svg>\n";
}

// Map data coordinates to SVG pixel coordinates
static int mapX(double x, double xMin, double xMax) {
    return MARGIN_L + static_cast<int>((x - xMin) / (xMax - xMin) * PLOT_W);
}

static int mapY(double y, double yMin, double yMax) {
    return MARGIN_T + PLOT_H - static_cast<int>((y - yMin) / (yMax - yMin) * PLOT_H);
}

// ── Validation helpers ──

static void checkArray(const Value& arg, const std::string& funcName, int line) {
    if (arg.type != Value::ARRAY) {
        throw std::runtime_error("[VDX] graph." + funcName +
            "() expects an array argument at line " + std::to_string(line));
    }
}

static void checkNumericArray(const Value& arr, const std::string& funcName, int line) {
    checkArray(arr, funcName, line);
    for (const auto& v : arr.arrVal) {
        if (!v.isNumeric()) {
            throw std::runtime_error("[VDX] graph." + funcName +
                "() array must contain only numeric values at line " + std::to_string(line));
        }
    }
}

static std::vector<double> toDoubles(const Value& arr) {
    std::vector<double> result;
    result.reserve(arr.arrVal.size());
    for (const auto& v : arr.arrVal) {
        result.push_back(v.toDouble());
    }
    return result;
}

static std::pair<double, double> minMax(const std::vector<double>& data) {
    if (data.empty()) return {0.0, 0.0};
    double mn = data[0], mx = data[0];
    for (double v : data) {
        if (v < mn) mn = v;
        if (v > mx) mx = v;
    }
    return {mn, mx};
}

// ── Scatter plot ──
Value scatter_builtin(const std::vector<Value>& args, int line) {
    if (args.size() != 2) {
        throw std::runtime_error("[VDX] graph.scatter() expects 2 arguments (xs, ys) at line " +
            std::to_string(line));
    }
    checkNumericArray(args[0], "scatter", line);
    checkNumericArray(args[1], "scatter", line);

    if (args[0].arrVal.size() != args[1].arrVal.size()) {
        throw std::runtime_error("[VDX] graph.scatter() xs and ys must have the same length at line " +
            std::to_string(line));
    }

    auto xs = toDoubles(args[0]);
    auto ys = toDoubles(args[1]);

    if (xs.empty()) {
        throw std::runtime_error("[VDX] graph.scatter() arrays must not be empty at line " +
            std::to_string(line));
    }

    auto [xMin, xMax] = minMax(xs);
    auto [yMin, yMax] = minMax(ys);
    if (xMax == xMin) xMax = xMin + 1.0;
    if (yMax == yMin) yMax = yMin + 1.0;

    std::ostringstream svg;
    svg << svgHeader(xMin, xMax, yMin, yMax);

    // Plot points as circles
    for (size_t i = 0; i < xs.size(); i++) {
        int px = mapX(xs[i], xMin, xMax);
        int py = mapY(ys[i], yMin, yMax);
        svg << "  <circle cx=\"" << px << "\" cy=\"" << py
            << "\" r=\"3\" fill=\"" << g_state.plotColor
            << "\" stroke=\"" << g_state.plotStroke << "\" stroke-width=\"0.5\"/>\n";
    }

    svg << svgFooter();
    g_state.svg = svg.str();
    g_state.hasPlot = true;
    return Value::makeVoid();
}

// ── Line chart ──
Value line_builtin(const std::vector<Value>& args, int line) {
    if (args.size() != 2) {
        throw std::runtime_error("[VDX] graph.line() expects 2 arguments (xs, ys) at line " +
            std::to_string(line));
    }
    checkNumericArray(args[0], "line", line);
    checkNumericArray(args[1], "line", line);

    if (args[0].arrVal.size() != args[1].arrVal.size()) {
        throw std::runtime_error("[VDX] graph.line() xs and ys must have the same length at line " +
            std::to_string(line));
    }

    auto xs = toDoubles(args[0]);
    auto ys = toDoubles(args[1]);

    if (xs.empty()) {
        throw std::runtime_error("[VDX] graph.line() arrays must not be empty at line " +
            std::to_string(line));
    }

    auto [xMin, xMax] = minMax(xs);
    auto [yMin, yMax] = minMax(ys);
    if (xMax == xMin) xMax = xMin + 1.0;
    if (yMax == yMin) yMax = yMin + 1.0;

    std::ostringstream svg;
    svg << svgHeader(xMin, xMax, yMin, yMax);

    // Build polyline points
    std::ostringstream pts;
    for (size_t i = 0; i < xs.size(); i++) {
        int px = mapX(xs[i], xMin, xMax);
        int py = mapY(ys[i], yMin, yMax);
        if (i > 0) pts << " ";
        pts << px << "," << py;
    }

    svg << "  <polyline points=\"" << pts.str()
        << "\" fill=\"none\" stroke=\"" << g_state.plotColor
        << "\" stroke-width=\"2\"/>\n";

    // Also draw points
    for (size_t i = 0; i < xs.size(); i++) {
        int px = mapX(xs[i], xMin, xMax);
        int py = mapY(ys[i], yMin, yMax);
        svg << "  <circle cx=\"" << px << "\" cy=\"" << py
            << "\" r=\"2.5\" fill=\"" << g_state.plotColor << "\"/>\n";
    }

    svg << svgFooter();
    g_state.svg = svg.str();
    g_state.hasPlot = true;
    return Value::makeVoid();
}

// ── Bar chart ──
Value bar_builtin(const std::vector<Value>& args, int line) {
    if (args.size() != 2) {
        throw std::runtime_error("[VDX] graph.bar() expects 2 arguments (labels, values) at line " +
            std::to_string(line));
    }
    checkArray(args[0], "bar", line);
    checkNumericArray(args[1], "bar", line);

    if (args[0].arrVal.size() != args[1].arrVal.size()) {
        throw std::runtime_error("[VDX] graph.bar() labels and values must have the same length at line " +
            std::to_string(line));
    }

    auto values = toDoubles(args[1]);
    if (values.empty()) {
        throw std::runtime_error("[VDX] graph.bar() arrays must not be empty at line " +
            std::to_string(line));
    }

    double vMin = *std::min_element(values.begin(), values.end());
    double vMax = *std::max_element(values.begin(), values.end());
    // Ensure bars start from 0 when all values are positive
    if (vMin > 0.0) vMin = 0.0;
    if (vMax < 0.0) vMax = 0.0;
    if (vMax == vMin) vMax = vMin + 1.0;

    int n = static_cast<int>(values.size());
    double barWidth = static_cast<double>(PLOT_W) / n * 0.7;
    double barGap = static_cast<double>(PLOT_W) / n * 0.3;

    std::ostringstream svg;
    svg << svgHeader(0, n, vMin, vMax);

    // Compute the zero line position in pixel coordinates
    int zeroY = MARGIN_T + PLOT_H - static_cast<int>((0.0 - vMin) / (vMax - vMin) * PLOT_H);

    for (int i = 0; i < n; i++) {
        // Draw bars from the zero line, not from vMin
        int valY = MARGIN_T + PLOT_H - static_cast<int>((values[i] - vMin) / (vMax - vMin) * PLOT_H);
        int bx = MARGIN_L + static_cast<int>(i * (barWidth + barGap) + barGap / 2);
        int by, bh;
        if (values[i] >= 0.0) {
            by = valY;
            bh = zeroY - valY;
        } else {
            by = zeroY;
            bh = valY - zeroY;
        }
        if (bh < 1) bh = 1;

        svg << "  <rect x=\"" << bx << "\" y=\"" << by
            << "\" width=\"" << static_cast<int>(barWidth) << "\" height=\"" << bh
            << "\" fill=\"" << g_state.plotColor
            << "\" stroke=\"" << g_state.plotStroke << "\" stroke-width=\"0.5\"/>\n";

        // Value label above (for positive) or below (for negative) bar
        int labelY = (values[i] >= 0.0) ? (by - 5) : (by + bh + 12);
        svg << "  <text x=\"" << (bx + static_cast<int>(barWidth) / 2) << "\" y=\""
            << labelY << "\" text-anchor=\"middle\""
            << " font-family=\"sans-serif\" font-size=\"10\">"
            << fmt(values[i]) << "</text>\n";

        // Category label below bar
        std::string lbl = args[0].arrVal[i].toString();
        if (lbl.size() > 12) lbl = lbl.substr(0, 12);
        svg << "  <text x=\"" << (bx + static_cast<int>(barWidth) / 2) << "\" y=\""
            << (MARGIN_T + PLOT_H + 18) << "\" text-anchor=\"middle\""
            << " font-family=\"sans-serif\" font-size=\"10\">"
            << escXml(lbl) << "</text>\n";
    }

    svg << svgFooter();
    g_state.svg = svg.str();
    g_state.hasPlot = true;
    return Value::makeVoid();
}

// ── Histogram ──
Value hist_builtin(const std::vector<Value>& args, int line) {
    if (args.size() < 1 || args.size() > 2) {
        throw std::runtime_error("[VDX] graph.hist() expects 1 or 2 arguments (data, [bins]) at line " +
            std::to_string(line));
    }
    checkNumericArray(args[0], "hist", line);

    auto data = toDoubles(args[0]);
    if (data.empty()) {
        throw std::runtime_error("[VDX] graph.hist() data must not be empty at line " +
            std::to_string(line));
    }

    int numBins = 10;
    if (args.size() == 2) {
        if (!args[1].isNumeric()) {
            throw std::runtime_error("[VDX] graph.hist() bins must be an integer at line " +
                std::to_string(line));
        }
        numBins = static_cast<int>(args[1].toDouble());
        if (numBins <= 0) {
            throw std::runtime_error("[VDX] graph.hist() bins must be positive at line " +
                std::to_string(line));
        }
    }

    auto [dMin, dMax] = minMax(data);
    if (dMax == dMin) dMax = dMin + 1.0;

    double binWidth = (dMax - dMin) / numBins;

    std::vector<int> counts(numBins, 0);
    for (double v : data) {
        int bin = static_cast<int>((v - dMin) / binWidth);
        if (bin >= numBins) bin = numBins - 1;
        if (bin < 0) bin = 0;
        counts[bin]++;
    }

    int maxCount = *std::max_element(counts.begin(), counts.end());
    if (maxCount == 0) maxCount = 1;

    std::ostringstream svg;
    svg << svgHeader(dMin, dMax, 0, maxCount);

    double barW = static_cast<double>(PLOT_W) / numBins;

    for (int b = 0; b < numBins; b++) {
        double barH = static_cast<double>(counts[b]) / maxCount * PLOT_H;
        int bx = MARGIN_L + static_cast<int>(b * barW);
        int by = MARGIN_T + PLOT_H - static_cast<int>(barH);
        int bh = static_cast<int>(barH);

        svg << "  <rect x=\"" << bx << "\" y=\"" << by
            << "\" width=\"" << static_cast<int>(barW) << "\" height=\"" << bh
            << "\" fill=\"" << g_state.plotColor
            << "\" stroke=\"" << g_state.plotStroke << "\" stroke-width=\"0.5\"/>\n";
    }

    svg << svgFooter();
    g_state.svg = svg.str();
    g_state.hasPlot = true;
    return Value::makeVoid();
}

// ── Area chart ──
Value area_builtin(const std::vector<Value>& args, int line) {
    if (args.size() != 2) {
        throw std::runtime_error("[VDX] graph.area() expects 2 arguments (xs, ys) at line " +
            std::to_string(line));
    }
    checkNumericArray(args[0], "area", line);
    checkNumericArray(args[1], "area", line);

    if (args[0].arrVal.size() != args[1].arrVal.size()) {
        throw std::runtime_error("[VDX] graph.area() xs and ys must have the same length at line " +
            std::to_string(line));
    }

    auto xs = toDoubles(args[0]);
    auto ys = toDoubles(args[1]);

    if (xs.empty()) {
        throw std::runtime_error("[VDX] graph.area() arrays must not be empty at line " +
            std::to_string(line));
    }

    auto [xMin, xMax] = minMax(xs);
    auto [yMin, yMax] = minMax(ys);
    if (xMax == xMin) xMax = xMin + 1.0;
    // Ensure area starts from 0 baseline
    if (yMin > 0.0) yMin = 0.0;
    if (yMax < 0.0) yMax = 0.0;
    if (yMax == yMin) yMax = yMin + 1.0;

    std::ostringstream svg;
    svg << svgHeader(xMin, xMax, yMin, yMax);

    // Build polygon: line points + baseline closure
    std::ostringstream pts;
    int zeroY = mapY(0.0, yMin, yMax);
    for (size_t i = 0; i < xs.size(); i++) {
        int px = mapX(xs[i], xMin, xMax);
        int py = mapY(ys[i], yMin, yMax);
        if (i > 0) pts << " ";
        pts << px << "," << py;
    }
    // Close polygon to baseline
    int lastX = mapX(xs[xs.size() - 1], xMin, xMax);
    int firstX = mapX(xs[0], xMin, xMax);
    pts << " " << lastX << "," << zeroY;
    pts << " " << firstX << "," << zeroY;

    svg << "  <polygon points=\"" << pts.str()
        << "\" fill=\"" << g_state.plotColor
        << "\" fill-opacity=\"0.3\" stroke=\"" << g_state.plotColor
        << "\" stroke-width=\"2\"/>\n";

    svg << svgFooter();
    g_state.svg = svg.str();
    g_state.hasPlot = true;
    return Value::makeVoid();
}

// ── Plot configuration ──
// These store labels/title for the next plot created.

Value title_builtin(const std::vector<Value>& args, int line) {
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] graph.title() expects 1 argument at line " +
            std::to_string(line));
    }
    if (args[0].type != Value::STRING) {
        throw std::runtime_error("[VDX] graph.title() expects a string at line " +
            std::to_string(line));
    }
    g_state.title = args[0].strVal;
    return Value::makeVoid();
}

Value xlabel_builtin(const std::vector<Value>& args, int line) {
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] graph.xlabel() expects 1 argument at line " +
            std::to_string(line));
    }
    if (args[0].type != Value::STRING) {
        throw std::runtime_error("[VDX] graph.xlabel() expects a string at line " +
            std::to_string(line));
    }
    g_state.xlabel = args[0].strVal;
    return Value::makeVoid();
}

Value ylabel_builtin(const std::vector<Value>& args, int line) {
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] graph.ylabel() expects 1 argument at line " +
            std::to_string(line));
    }
    if (args[0].type != Value::STRING) {
        throw std::runtime_error("[VDX] graph.ylabel() expects a string at line " +
            std::to_string(line));
    }
    g_state.ylabel = args[0].strVal;
    return Value::makeVoid();
}

// ── v0.1.4 plot configuration ──

Value grid_builtin(const std::vector<Value>& args, int line) {
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] graph.grid() expects 1 argument (bool) at line " +
            std::to_string(line));
    }
    if (args[0].type != Value::BOOL) {
        throw std::runtime_error("[VDX] graph.grid() expects a boolean at line " +
            std::to_string(line));
    }
    g_state.showGrid = args[0].boolVal;
    return Value::makeVoid();
}

Value color_builtin(const std::vector<Value>& args, int line) {
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] graph.color() expects 1 argument (color name or hex) at line " +
            std::to_string(line));
    }
    if (args[0].type != Value::STRING) {
        throw std::runtime_error("[VDX] graph.color() expects a string at line " +
            std::to_string(line));
    }
    g_state.plotColor = args[0].strVal;
    // Auto-derive a darker stroke color
    if (args[0].strVal == "steelblue") g_state.plotStroke = "navy";
    else if (args[0].strVal == "red") g_state.plotStroke = "darkred";
    else if (args[0].strVal == "green") g_state.plotStroke = "darkgreen";
    else if (args[0].strVal == "blue") g_state.plotStroke = "darkblue";
    else if (args[0].strVal == "orange") g_state.plotStroke = "darkorange";
    else if (args[0].strVal == "purple") g_state.plotStroke = "indigo";
    else if (args[0].strVal == "black") g_state.plotStroke = "black";
    else g_state.plotStroke = args[0].strVal;
    return Value::makeVoid();
}

Value legend_builtin(const std::vector<Value>& args, int line) {
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] graph.legend() expects 1 argument (array of labels) at line " +
            std::to_string(line));
    }
    if (args[0].type != Value::ARRAY) {
        throw std::runtime_error("[VDX] graph.legend() expects an array of strings at line " +
            std::to_string(line));
    }
    g_state.legendLabels.clear();
    for (const auto& v : args[0].arrVal) {
        g_state.legendLabels.push_back(v.toString());
    }
    return Value::makeVoid();
}

// ── Output ──

Value save_builtin(const std::vector<Value>& args, int line) {
    if (args.size() != 1) {
        throw std::runtime_error("[VDX] graph.save() expects 1 argument (path) at line " +
            std::to_string(line));
    }
    if (args[0].type != Value::STRING) {
        throw std::runtime_error("[VDX] graph.save() expects a string path at line " +
            std::to_string(line));
    }
    if (!g_state.hasPlot) {
        throw std::runtime_error("[VDX] graph.save() no plot has been created yet at line " +
            std::to_string(line));
    }

    std::string path = args[0].strVal;
    // TODO: auto-append .svg if no extension
    std::ofstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("[VDX] graph.save() cannot open file '" + path + "' at line " +
            std::to_string(line));
    }
    file << g_state.svg;
    file.close();
    return Value::makeVoid();
}

Value show_builtin(const std::vector<Value>& args, int line) {
    (void)args;
    if (!g_state.hasPlot) {
        throw std::runtime_error("[VDX] graph.show() no plot has been created yet at line " +
            std::to_string(line));
    }

    // Write to a temp file and open it
    // TODO: cross-platform — currently Windows-only
#ifdef _WIN32
    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, 999999);
    std::string tmpPath = (std::filesystem::temp_directory_path() /
        ("vdx_plot_" + std::to_string(dist(rd)) + ".svg")).string();
#else
    std::string tmpPath = "/tmp/vdx_plot.svg";
#endif
    std::ofstream file(tmpPath);
    if (!file.is_open()) {
        throw std::runtime_error("[VDX] graph.show() cannot create temp file at line " +
            std::to_string(line));
    }
    file << g_state.svg;
    file.close();

#ifdef _WIN32
    std::string cmd = "start \"\" \"" + tmpPath + "\"";
    std::system(cmd.c_str());
#else
    // TODO: support macOS (open) and Linux (xdg-open)
    std::string cmd = "open \"" + tmpPath + "\" 2>/dev/null || xdg-open \"" + tmpPath + "\" 2>/dev/null &";
    std::system(cmd.c_str());
#endif

    return Value::makeVoid();
}

// ── Registration ──
void registerGraph(Interpreter& interp) {
    interp.registerModuleFunc("graph.scatter", scatter_builtin);
    interp.registerModuleFunc("graph.line", line_builtin);
    interp.registerModuleFunc("graph.bar", bar_builtin);
    interp.registerModuleFunc("graph.hist", hist_builtin);
    interp.registerModuleFunc("graph.area", area_builtin);
    interp.registerModuleFunc("graph.title", title_builtin);
    interp.registerModuleFunc("graph.xlabel", xlabel_builtin);
    interp.registerModuleFunc("graph.ylabel", ylabel_builtin);
    interp.registerModuleFunc("graph.grid", grid_builtin);
    interp.registerModuleFunc("graph.color", color_builtin);
    interp.registerModuleFunc("graph.legend", legend_builtin);
    interp.registerModuleFunc("graph.show", show_builtin);
    interp.registerModuleFunc("graph.save", save_builtin);
}

} // namespace GraphModule
