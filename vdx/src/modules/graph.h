#pragma once
#include "../interpreter.h"
#include <string>
#include <vector>

// ── Graph Module (v0.1.0) ──
// Provides plotting and data visualization for the VDX programming language.
// Generates SVG vector graphics files — no external dependencies required.
// SVG files can be opened in any web browser or vector graphics editor.

namespace GraphModule {
    // Initialize and register all graph functions with the interpreter
    void registerGraph(Interpreter& interp);

    // ── Chart creation ──
    // Each function builds an SVG plot and stores it internally.
    // Use graph.save(path) to write the SVG to disk, or graph.show() to open it.

    // graph.scatter(xs, ys) — scatter plot from two numeric arrays
    // TODO: support marker style/size/color parameter
    Value scatter_builtin(const std::vector<Value>& args, int line);

    // graph.line(xs, ys) — line chart connecting (x, y) points
    // TODO: support line style/color parameter
    Value line_builtin(const std::vector<Value>& args, int line);

    // graph.bar(labels, values) — vertical bar chart
    // TODO: support horizontal bars and stacked bars
    Value bar_builtin(const std::vector<Value>& args, int line);

    // graph.hist(data, [bins]) — histogram with configurable bin count
    // FIXME: bin edge calculation is simplistic — need proper binning algorithm
    Value hist_builtin(const std::vector<Value>& args, int line);

    // graph.area(xs, ys) — area chart (line + filled region below)
    Value area_builtin(const std::vector<Value>& args, int line);

    // ── Plot configuration ──
    // These set labels/title for the next plot created.

    // graph.title(text) — set the title for the next plot
    Value title_builtin(const std::vector<Value>& args, int line);

    // graph.xlabel(text) — set the x-axis label for the next plot
    Value xlabel_builtin(const std::vector<Value>& args, int line);

    // graph.ylabel(text) — set the y-axis label for the next plot
    Value ylabel_builtin(const std::vector<Value>& args, int line);

    // graph.grid(bool) — toggle grid lines on/off for the next plot
    Value grid_builtin(const std::vector<Value>& args, int line);

    // graph.color(name) — set the primary color for the next plot
    Value color_builtin(const std::vector<Value>& args, int line);

    // graph.legend(labels) — add a legend to the next plot
    Value legend_builtin(const std::vector<Value>& args, int line);

    // ── Output ──

    // graph.save(path) — save the current plot as an SVG file
    Value save_builtin(const std::vector<Value>& args, int line);

    // graph.show() — open the current plot in the default SVG viewer
    // TODO: cross-platform open command (currently Windows-only)
    Value show_builtin(const std::vector<Value>& args, int line);
}
