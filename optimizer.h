#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "parser.h"

#include <string>
#include <vector>

bool writeOptimizedFile(
    const vector<Statement>& statements,
    const string& filename
);

#endif
