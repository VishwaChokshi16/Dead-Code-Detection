#include "optimizer.h"

#include <fstream>
#include <iostream>

using namespace std;

bool writeOptimizedFile(
    const vector<Statement>& statements,
    const string& filename) {

    ofstream output(filename);

    if (!output.is_open()) {
        cerr << "Error: Cannot create output file: "
             << filename << '\n';
        return false;
    }

    for (const Statement& st : statements) {
        if (!st.isDead)
            output << st.originalCode << '\n';
    }

    output.close();

    return true;
}
