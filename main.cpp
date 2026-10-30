#include <iostream>
#include <iomanip>

#include "parser.h"
#include "detector.h"
#include "optimizer.h"

using namespace std;

int main() {
    const string inputFile = "input.txt";
    const string outputFile = "optimized.txt";

    cout << "\nVishwa Chokshi\n";
    cout << "\nHarshil Dhamat\n";
    cout << "\nMitanshu Dholkiya\n";
    cout << " DEAD CODE DETECTION AND ELIMINATION\n";

    // Step 1: Parse the input.
    vector<Statement> statements =
        parseSourceFile(inputFile);

    if (statements.empty()) {
        cout << "\nNo statements found, or input file is missing.\n";
        return 1;
    }

    cout << "\nOriginal Source Code:\n";

    for (const Statement& st : statements)
        cout << st.originalCode << '\n';

    // Step 2: Detect dead assignments.
    detectDeadCode(statements);

    cout << "\nDead Code Detection Report:\n";

    int deadCount = 0;

    for (const Statement& st : statements) {
        if (st.isDead) {
            ++deadCount;

            cout << "Line " << st.lineNumber << ": "
                 << st.originalCode << '\n';

            cout << "Reason: " << st.reason << "\n\n";
        }
    }

    if (deadCount == 0)
        cout << "No dead assignments detected.\n";

    // Step 3: Eliminate dead code.
    if (!writeOptimizedFile(statements, outputFile))
        return 1;

    cout << "Optimization Summary:\n";

    cout << "Statements analyzed : "
         << statements.size() << '\n';

    cout << "Dead statements     : "
         << deadCount << '\n';

    cout << "Statements retained : "
         << statements.size() - deadCount << '\n';

    cout << "\nOptimized Source Code:\n";

    for (const Statement& st : statements) {
        if (!st.isDead)
            cout << st.originalCode << '\n';
    }

    cout << "\nOptimized code saved to "
         << outputFile << '\n';

    cout << "\nProject completed successfully.\n";

    return 0;
}
