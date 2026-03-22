#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

// Split source into lines for error display
static std::vector<std::string> splitLines(const std::string& src) {
    std::vector<std::string> lines;
    std::istringstream stream(src);
    std::string line;
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }
    return lines;
}

// Try to extract line number from error message like "at line 5"
static int extractLine(const std::string& msg) {
    // Look for "line X" pattern
    size_t pos = msg.find("line ");
    if (pos != std::string::npos) {
        pos += 5;
        std::string num;
        while (pos < msg.size() && isdigit(msg[pos])) {
            num += msg[pos++];
        }
        if (!num.empty()) return std::stoi(num);
    }
    return -1;
}

static void printError(const std::string& source, const std::string& filename, const std::string& msg) {
    auto lines = splitLines(source);
    int errLine = extractLine(msg);

    std::cerr << "\n";
    std::cerr << "  error: " << msg << "\n";

    if (errLine > 0 && errLine <= (int)lines.size()) {
        std::cerr << "   --> " << filename << ":" << errLine << "\n";
        std::cerr << "    |" << "\n";

        // Show context: line before, error line, line after
        int start = std::max(1, errLine - 1);
        int end = std::min((int)lines.size(), errLine + 1);
        for (int i = start; i <= end; i++) {
            if (i == errLine) {
                std::cerr << "  " << i << " |  " << lines[i - 1] << "  <--" << "\n";
            } else {
                std::cerr << "  " << i << " |  " << lines[i - 1] << "\n";
            }
        }
        std::cerr << "    |" << "\n";
    } else {
        std::cerr << "   --> " << filename << "\n";
    }
    std::cerr << "\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: vdx <file.vdx>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "\n  error: Cannot open file '" << filename << "'\n\n";
        return 1;
    }

    std::stringstream buf;
    buf << file.rdbuf();
    std::string source = buf.str();

    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto program = parser.parse();

        Interpreter interp;
        interp.run(program);
    } catch (const std::runtime_error& e) {
        printError(source, filename, e.what());
        return 1;
    }

    return 0;
}
