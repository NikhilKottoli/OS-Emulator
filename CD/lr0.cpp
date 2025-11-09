#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <sstream>
#include <algorithm>
#include <cctype>
using namespace std;

class Production {
    public:
    string lhs, rhs;
    Production(string l, string r){
        lhs = l;
        rhs = r;
    }
};

class Item {
    public:
    int prod;
    int dot;
    Item(int p, int d){
        prod = p;
        dot = d;
    }
    bool operator<(const Item& o) const {
        return prod != o.prod ? prod < o.prod : dot < o.dot;
    }
    bool operator==(const Item& o) const {
        return prod == o.prod && dot == o.dot;
    }
};

class State {
    public:
    set<Item> items;
    int id;
};

vector<Production> prods;
vector<State> states;
map<pair<int,char>, int> trans;

void closure(set<Item>& s) {
    bool changed = true;
    while (changed) {
        changed = false;
        set<Item> copy = s;
        for (const auto& it : copy) {
            if (it.dot >= (int)prods[it.prod].rhs.size()) continue; // Dot at end

            char c = prods[it.prod].rhs[it.dot];
            
            if (isupper(c)) {
                for (int i = 0; i < (int)prods.size(); i++) {
                    if (prods[i].lhs[0] == c) {
                        if (s.insert(Item(i, 0)).second) changed = true;
                    }
                }
            }
        }
    }
}

set<Item> go(const set<Item>& from, char sym) {
    set<Item> res;
    for (const auto& it : from) {
        if (it.dot < (int)prods[it.prod].rhs.size() && 
            prods[it.prod].rhs[it.dot] == sym) {
            res.insert(Item(it.prod, it.dot + 1));
        }
    }
    if (!res.empty()) closure(res);
    return res;
}

int findState(const set<Item>& s) {
    for (int i = 0; i < (int)states.size(); i++)
        if (states[i].items == s) return states[i].id;
    return -1;
}

void buildStates() {
    State s0;
    s0.items.insert(Item(0, 0));
    closure(s0.items);
    s0.id = 0;
    states.push_back(s0);
    
    set<char> syms;
    for (int i = 0; i < (int)prods.size(); i++) {
        if (prods[i].lhs != "S'") syms.insert(prods[i].lhs[0]);
        for (int j = 0; j < (int)prods[i].rhs.size(); j++)
            if (prods[i].rhs[j] != '#') syms.insert(prods[i].rhs[j]);
    }
    
    for (int i = 0; i < (int)states.size(); i++) {
        for (set<char>::iterator sit = syms.begin(); sit != syms.end(); ++sit) {
            set<Item> next = go(states[i].items, *sit);
            if (!next.empty()) {
                int id = findState(next);
                if (id == -1) {
                    State ns;
                    ns.items = next;
                    ns.id = states.size();
                    id = ns.id;
                    states.push_back(ns);
                }
                trans[make_pair(i, *sit)] = id;
            }
        }
    }
}

string trim(string s) {
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
    return s;
}

int main() {
    int n;
    cout << "Enter number of productions : ";
    cin >> n;
    cin.ignore();
    
    cout << "Enter productions :\n";
    bool first = true;
    
    for (int i = 0; i < n; i++) {
        string line, lhs, rhs;
        getline(cin, line);
        
        size_t pos = line.find('=');
        lhs = trim(line.substr(0, pos));
        rhs = trim(line.substr(pos + 1));
        
        // size_t start = 0, end;
        // while ((end = rhs.find('|', start)) != string::npos) {
        //     string alt = rhs.substr(start, end - start);
        //     if (first) {
        //         prods.push_back(Production("S'", string(1, lhs[0])));
        //         first = false;
        //     }
        //     prods.push_back(Production(lhs, alt));
        //     start = end + 1;
        // }
        if (first) {
            prods.push_back(Production("S'", string(1, lhs[0])));
            first = false;
        }
        prods.push_back(Production(lhs, rhs));
    }
    
    buildStates();
    
    cout << "\nAugmented Grammar :\n";
    for (int i = 0; i < (int)prods.size(); i++)
        cout << prods[i].lhs << " -> " << prods[i].rhs << "\n";
    
    cout << "\n----------------------------------------\n";
    
    for (int i = 0; i < (int)states.size(); i++) {
        cout << "I" << states[i].id << " :\n";
        for (set<Item>::iterator it = states[i].items.begin(); 
             it != states[i].items.end(); ++it) {
            cout << "    " << prods[it->prod].lhs << " -> ";
            for (int j = 0; j < (int)prods[it->prod].rhs.size(); j++) {
                if (j == it->dot) cout << ". ";
                cout << prods[it->prod].rhs[j] << " ";
            }
            if (it->dot == (int)prods[it->prod].rhs.size()) cout << ".";
            cout << "\n";
        }
        
        for (map<pair<int,char>, int>::iterator mit = trans.begin(); 
             mit != trans.end(); ++mit) {
            if (mit->first.first == i)
                cout << "GOTO ( I" << i << " , " << mit->first.second 
                     << " ) = I" << mit->second << "\n";
        }
        if (i < (int)states.size() - 1) cout << "\n";
    }
    
    cout << "----------------------------------------\n";
    cout << "Canonical Collection of LR(0) Item Sets :\n";
    for (int i = 0; i < (int)states.size(); i++) {
        cout << "I" << i;
        if (i < (int)states.size() - 1) cout << " , ";
    }
    cout << "\n";
    
    return 0;
}