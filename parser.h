#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

using namespace std;

struct Statement {
    int lineNumber;
    string originalCode;
    string lhs;
    string rhs;
    vector<string> variablesUsed;

    bool isAssignment;
    bool isObservable;
    bool safeToEliminate;
    bool isDead;
    string reason;
};

vector<Statement> parseSourceFile(const string& filename);

#endif
