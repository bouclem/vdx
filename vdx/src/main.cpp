#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <cctype>

static const char* VDX_VERSION = "0.1.5";

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
    // Look for "at line X" pattern (more specific than just "line ")
    size_t pos = msg.find("at line ");
    if (pos != std::string::npos) {
        pos += 8;
        std::string num;
        while (pos < msg.size() && isdigit(static_cast<unsigned char>(msg[pos]))) {
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

static void printUsage() {
    std::cout << "VDX programming language interpreter v" << VDX_VERSION << "\n\n"
        << "Usage: vdx <file.vdx>\n"
        << "       vdx --help\n"
        << "       vdx --version\n\n"
        << "Options:\n"
        << "  --help, -h     Show this help message\n"
        << "  --version, -v  Show the interpreter version\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: vdx <file.vdx>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    if (filename == "--help" || filename == "-h") {
        printUsage();
        return 0;
    }
    if (filename == "--version" || filename == "-v") {
        std::cout << "vdx " << VDX_VERSION << "\n";
        return 0;
    }

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "\n  error: Cannot open file '" << filename << "'\n\n";
        return 1;
    }

    std::stringstream buf;
    buf << file.rdbuf();
    std::string source = buf.str();

    Interpreter interp;
    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto program = parser.parse();

        std::string sourceDir = std::filesystem::path(filename).parent_path().string();
        interp.run(program, sourceDir, filename);
    } catch (const std::runtime_error& e) {
        // If the error originated in an imported file, show that file's source
        const std::string& errSource = interp.errorSource.empty() ? source : interp.errorSource;
        const std::string& errFile = interp.errorFile.empty() ? filename : interp.errorFile;
        printError(errSource, errFile, e.what());
        return 1;
    } catch (const std::bad_alloc&) {
        std::cerr << "\n  error: Out of memory\n\n";
        return 1;
    }

    return 0;
}
