#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

struct Production {
    string lhs;
    vector<string> rhs;
};

vector<Production> prods;
vector<string> nonTerminals;
vector<string> terminals;
map<string, vector<string>> FIRST, FOLLOW;
map<string, map<string, string>> parseTable;
string startSymbol;

bool isNonTerminal(const string &s) {
    if (s.empty()) return false;
    char c = s[0];
    return (c >= 'A' && c <= 'Z');
}

bool contains(const vector<string> &v, const string &x) {
    for (auto &a : v) if (a == x) return true;
    return false;
}

void addUnique(vector<string> &v, const string &x) {
    if (!contains(v, x)) v.push_back(x);
}

// Split RHS by '|', manual parsing (no stringstream)
vector<vector<string>> splitRhsAlternatives(const string &rhsLine) {
    vector<string> tokens;
    string cur;
    for (size_t i = 0; i < rhsLine.size(); ++i) {
        char c = rhsLine[i];
        if (c == ' ') {
            if (!cur.empty()) { tokens.push_back(cur); cur.clear(); }
        } else cur.push_back(c);
    }
    if (!cur.empty()) tokens.push_back(cur);

    vector<vector<string>> alts;
    vector<string> curAlt;
    for (auto &t : tokens) {
        if (t == "|") {
            if (!curAlt.empty()) { alts.push_back(curAlt); curAlt.clear(); }
        } else curAlt.push_back(t);
    }
    if (!curAlt.empty()) alts.push_back(curAlt);
    return alts;
}

void computeFIRST() {
    for (auto &t : terminals) FIRST[t] = { t };
    for (auto &A : nonTerminals) if (FIRST.find(A) == FIRST.end()) FIRST[A] = {};

    bool changed = true;
    while (changed) {
        changed = false;
        for (auto &p : prods) {
            string A = p.lhs;
            vector<string> before = FIRST[A];

            if (p.rhs.empty()) {
                addUnique(FIRST[A], "eps");
                if (FIRST[A].size() != before.size()) changed = true;
                continue;
            }

            bool allNullable = true;
            for (auto &sym : p.rhs) {
                if (!isNonTerminal(sym)) {
                    if (sym != "eps") addUnique(FIRST[A], sym);
                    if (sym != "eps") allNullable = false;
                    break;
                } else {
                    for (auto &f : FIRST[sym]) if (f != "eps") addUnique(FIRST[A], f);
                    if (!contains(FIRST[sym], "eps")) { allNullable = false; break; }
                }
            }
            if (allNullable) addUnique(FIRST[A], "eps");

            if (FIRST[A].size() != before.size()) changed = true;
        }
    }
}

void computeFOLLOW() {
    for (auto &A : nonTerminals) if (FOLLOW.find(A) == FOLLOW.end()) FOLLOW[A] = {};
    addUnique(FOLLOW[startSymbol], "$");

    bool changed = true;
    while (changed) {
        changed = false;
        for (auto &p : prods) {
            string A = p.lhs;
            for (size_t i = 0; i < p.rhs.size(); ++i) {
                string B = p.rhs[i];
                if (!isNonTerminal(B)) continue;

                vector<string> before = FOLLOW[B];

                bool allNullable = true;
                for (size_t j = i + 1; j < p.rhs.size(); ++j) {
                    string sym = p.rhs[j];
                    if (!isNonTerminal(sym)) {
                        if (sym != "eps") addUnique(FOLLOW[B], sym);
                        allNullable = false;
                        break;
                    } else {
                        for (auto &f : FIRST[sym]) if (f != "eps") addUnique(FOLLOW[B], f);
                        if (!contains(FIRST[sym], "eps")) { allNullable = false; break; }
                    }
                }
                if (i + 1 == p.rhs.size() || allNullable) {
                    for (auto &x : FOLLOW[A]) addUnique(FOLLOW[B], x);
                }

                if (FOLLOW[B].size() != before.size()) changed = true;
            }
        }
    }
}

void buildParseTable() {
    bool conflict = false;

    for (auto &p : prods) {
        vector<string> firstAlpha;
        bool allNullable = true;

        if (p.rhs.empty()) firstAlpha.push_back("eps");
        else {
            for (auto &sym : p.rhs) {
                if (!isNonTerminal(sym)) {
                    if (sym != "eps") addUnique(firstAlpha, sym);
                    if (sym != "eps") allNullable = false;
                    break;
                } else {
                    for (auto &f : FIRST[sym]) if (f != "eps") addUnique(firstAlpha, f);
                    if (!contains(FIRST[sym], "eps")) { allNullable = false; break; }
                }
            }
            if (allNullable) addUnique(firstAlpha, "eps");
        }

        string prodStr = p.lhs + " -> ";
        if (p.rhs.empty()) prodStr += "eps";
        else for (auto &x : p.rhs) prodStr += x + " ";

        for (auto &t : firstAlpha) {
            if (t != "eps") {
                if (parseTable[p.lhs][t] != "") {
                    conflict = true;
                    parseTable[p.lhs][t] += " | " + prodStr;
                } else parseTable[p.lhs][t] = prodStr;
            }
        }

        if (contains(firstAlpha, "eps")) {
            for (auto &b : FOLLOW[p.lhs]) {
                if (parseTable[p.lhs][b] != "") {
                    conflict = true;
                    parseTable[p.lhs][b] += " | " + prodStr;
                } else parseTable[p.lhs][b] = prodStr;
            }
        }
    }

    if (conflict)
        cout << "\n  Grammar is NOT LL(1) (some cells have multiple productions)\n";
    else
        cout << "\n Grammar is LL(1)\n";
}

int main() {
    cout << "Enter grammar productions (use 'eps' for epsilon). End with #:\n";

    while (true) {
        string line;
        getline(cin, line);
        if (line == "#") break;
        if (line.empty()) continue;

        size_t pos = line.find("->");
        if (pos == string::npos) continue;

        string lhs = "";
        for (char c : line.substr(0, pos))
            if (c != ' ') lhs += c;

        string rhsPart = line.substr(pos + 2);
        vector<vector<string>> alts = splitRhsAlternatives(rhsPart);

        for (auto &alt : alts) {
            Production p; p.lhs = lhs; p.rhs = alt;
            prods.push_back(p);
        }
        if (!contains(nonTerminals, lhs)) nonTerminals.push_back(lhs);
    }

    if (prods.empty()) return 0;
    startSymbol = prods[0].lhs;

    for (auto &p : prods)
        for (auto &sym : p.rhs)
            if (!isNonTerminal(sym) && sym != "eps" && !contains(terminals, sym))
                terminals.push_back(sym);
    terminals.push_back("$");

    computeFIRST();
    computeFOLLOW();
    buildParseTable();

    cout << "\nFIRST sets:\n";
    for (auto &A : nonTerminals) {
        cout << "FIRST(" << A << ") = { ";
        for (auto &x : FIRST[A]) cout << x << " ";
        cout << "}\n";
    }

    cout << "\nFOLLOW sets:\n";
    for (auto &A : nonTerminals) {
        cout << "FOLLOW(" << A << ") = { ";
        for (auto &x : FOLLOW[A]) cout << x << " ";
        cout << "}\n";
    }

    cout << "\n--- LL(1) Parsing Table ---\n\n\t";
    for (auto &t : terminals) cout << t << "\t";
    cout << "\n";

    for (auto &A : nonTerminals) {
        cout << A << "\t";
        for (auto &t : terminals) {
            if (parseTable[A][t] != "")
                cout << parseTable[A][t] << "\t";
            else
                cout << "-\t";
        }
        cout << "\n";
    }

    return 0;
}
