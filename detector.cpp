#include "detector.h"

#include <set>

using namespace std;

void detectDeadCode(vector<Statement>& statements) {
    set<string> liveVariables;

    // Analyze the program backward.
    for (int i = static_cast<int>(statements.size()) - 1;
         i >= 0; --i) {

        Statement& st = statements[i];
        st.isDead = false;
        st.reason = "";

        if (st.isAssignment) {

            bool valueIsNeeded =
                liveVariables.count(st.lhs) > 0;

            if (st.safeToEliminate && !valueIsNeeded) {
                st.isDead = true;
                st.reason =
                    "Assigned value is never used later.";

                // Do not mark RHS variables live:
                // the entire pure assignment is removed.
                continue;
            }

            // The assignment must be retained.
            liveVariables.erase(st.lhs);

            for (const string& variable : st.variablesUsed)
                liveVariables.insert(variable);
        }
        else {
            // Preserve observable and unknown statements.
            for (const string& variable : st.variablesUsed)
                liveVariables.insert(variable);
        }
    }
}
