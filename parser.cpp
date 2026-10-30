#include "parser.h"

#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <algorithm>
#include <cctype>

using namespace std;

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");

    if (start == string::npos)
        return "";

    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

bool isDeclaration(const string& s) {
    static const regex typePattern(
        R"(^\s*(const\s+)?(int|float|double|char|bool|long|short|string)\b)"
    );

    return regex_search(s, typePattern);
}

string getLastIdentifier(const string& s) {
    static const regex identifier(R"([A-Za-z_][A-Za-z0-9_]*)");

    string result;
    auto begin = sregex_iterator(s.begin(), s.end(), identifier);
    auto end = sregex_iterator();

    for (auto it = begin; it != end; ++it)
        result = it->str();

    return result;
}

int findAssignment(const string& s) {
    bool inSingle = false;
    bool inDouble = false;
    bool escaped = false;

    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        char c = s[i];

        if (escaped) {
            escaped = false;
            continue;
        }

        if ((inSingle || inDouble) && c == '\\') {
            escaped = true;
            continue;
        }

        if (!inDouble && c == '\'') {
            inSingle = !inSingle;
            continue;
        }

        if (!inSingle && c == '"') {
            inDouble = !inDouble;
            continue;
        }

        if (inSingle || inDouble || c != '=')
            continue;

        char prev = (i > 0) ? s[i - 1] : '\0';
        char next = (i + 1 < static_cast<int>(s.size()))
                        ? s[i + 1] : '\0';

        if (prev == '=' || prev == '!' ||
            prev == '<' || prev == '>' || next == '=')
            continue;

        return i;
    }

    return -1;
}

vector<string> extractIdentifiers(const string& expression) {
    string clean = regex_replace(
        expression,
        regex(R"("(\\.|[^"\\])*"|'(\\.|[^'\\])*')"),
        " "
    );

    static const regex identifier(R"([A-Za-z_][A-Za-z0-9_]*)");

    static const set<string> keywords = {
        "int", "float", "double", "char", "bool",
        "long", "short", "const", "string",
        "return", "printf", "void", "true", "false"
    };

    vector<string> result;

    auto begin = sregex_iterator(clean.begin(), clean.end(), identifier);
    auto end = sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        string name = it->str();

        if (keywords.count(name) == 0)
            result.push_back(name);
    }

    return result;
}

Statement parseStatement(const string& source, int lineNumber) {
    Statement st{};

    st.lineNumber = lineNumber;
    st.originalCode = trim(source);
    st.isAssignment = false;
    st.isObservable = false;
    st.safeToEliminate = false;
    st.isDead = false;

    string code = st.originalCode;

    if (!code.empty() && code.back() == ';')
        code.pop_back();

    code = trim(code);

    int eq = findAssignment(code);

    if (eq != -1) {
        string left = trim(code.substr(0, eq));
        st.rhs = trim(code.substr(eq + 1));

        st.lhs = getLastIdentifier(left);
        st.isAssignment = !st.lhs.empty();

        if (st.isAssignment) {
            st.variablesUsed = extractIdentifiers(st.rhs);

            // Remove type keywords from the assignment's left side.
            st.safeToEliminate =
                st.rhs.find('(') == string::npos &&
                st.rhs.find(')') == string::npos &&
                st.rhs.find("++") == string::npos &&
                st.rhs.find("--") == string::npos &&
                st.rhs.find('=') == string::npos;

            // Function calls and compound expressions are preserved.
            if (st.rhs.find('(') != string::npos ||
                st.rhs.find(')') != string::npos)
                st.safeToEliminate = false;

            return st;
        }
    }

    // Observable operations must be retained.
    if (code.find("printf") != string::npos ||
        code.find("cout") != string::npos ||
        code.find("return") == 0) {

        st.isObservable = true;
        st.variablesUsed = extractIdentifiers(code);
        return st;
    }

    // Support declarations without initial values.
    if (isDeclaration(code)) {
        st.lhs = getLastIdentifier(code);
        st.isAssignment = !st.lhs.empty();
        st.safeToEliminate = st.isAssignment;
        st.rhs = "";
        return st;
    }

    // Unknown statements are preserved conservatively.
    st.isObservable = true;
    st.variablesUsed = extractIdentifiers(code);

    return st;
}

vector<Statement> parseSourceFile(const string& filename) {
    vector<Statement> statements;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Cannot open input file: "
             << filename << '\n';
        return statements;
    }

    string line;
    int lineNumber = 0;

    while (getline(file, line)) {
        ++lineNumber;
        line = trim(line);

        if (line.empty() ||
            line.rfind("//", 0) == 0)
            continue;

        statements.push_back(parseStatement(line, lineNumber));
    }

    return statements;
}
