#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <cctype>

using namespace std;

vector<pair<string, vector<string>>> Productions;
set<string> Terminals, NonTerminals;
map<string, set<string>> FIRST;
map<string, set<string>> FOLLOW;

void findFirst(){
    for(auto terminal : Terminals){
        FIRST[terminal].insert(terminal);
    }
    bool changed = true;
    while(changed){
        changed = false;
        for(auto prod : Productions){
            string LHS = prod.first;
            vector<string> RHS = prod.second;
            bool allNull = true;
            set<string> before = FIRST[LHS];
            for(int i = 0; i < RHS.size(); i++){
                if(!NonTerminals.count(RHS[i])){
                    if(RHS[i] != "eps"){
                        allNull = false;
                        FIRST[LHS].insert(RHS[i]);
                    }
                    break;
                }
                else{
                    for(auto first : FIRST[RHS[i]]){
                        if(first != "eps")
                            FIRST[LHS].insert(first);
                    }
                    if(FIRST[RHS[i]].find("eps") == FIRST[RHS[i]].end()){
                        allNull = false;
                        break;
                    }
                }
            }
            if(allNull){
                FIRST[LHS].insert("eps");
            }
            if(before.size() != FIRST[LHS].size()) changed = true;
        }
    }
}

void findFollow(string start){
    FOLLOW[start].insert("$");
    bool changed = true;
    while(changed){
        changed = false;
        for(auto prod:Productions){
            string LHS= prod.first;
            vector<string> RHS=prod.second;
            for(int i=0;i<RHS.size();i++){
                string curr=RHS[i];
                if(Terminals.count(curr)) continue;
                set<string>before=FOLLOW[curr];
                bool allnull=true;
                for(int j=i+1;j<RHS.size();j++){
                    if(Terminals.count(RHS[j])) {
                        if(RHS[j]!="eps") {
                            FOLLOW[curr].insert(RHS[j]);
                            allnull=false;
                        }
                        break;
                    }else{
                        for(auto first:FIRST[RHS[j]]){
                            if(first!="eps"){
                                FOLLOW[curr].insert(first);
                            }
                        }
                        if(!FIRST[RHS[j]].count("eps")) {
                            allnull=false;
                            break;
                        } 
                    }
                }
                if(allnull || i+1==RHS.size()) {
                    for(auto follow: FOLLOW[LHS]){
                        FOLLOW[curr].insert(follow);
                    }
                }
                if(before.size()!=FOLLOW[curr].size()){
                    changed=true;
                } 
            }
        }
    }
}

void printfirst(){
    cout<<"FIRST:"<<endl;
    for(auto firsts:FIRST){
        if(!isupper(firsts.first[0])) continue;
        cout<<firsts.first<<"={";
        for(auto terms:firsts.second){
            cout<<terms<<" ";
        }
        cout<<"}"<<endl;
    }
}

void printfollow(){
    cout<<"FOLLOW:"<<endl;
    for(auto firsts:FOLLOW){
        if(!isupper(firsts.first[0])) continue;
        cout<<firsts.first<<"={";
        for(auto terms:firsts.second){
            cout<<terms<<" ";
        }
        cout<<"}"<<endl;
    }
}

int main()
{
    cout << "Enter number of productions: ";
    int n;
    cin >> n;
    cin.ignore();
    for(int i = 0; i < n; i++){
        string s;
        getline(cin, s);
        int idx = s.find("->");
        string LHS = "";
        for(int j = 0; j < idx; j++){
            if(s[j] != ' '){
                LHS += s[j];
            }
            else{
                break;
            }
        }
        vector<string> RHS;
        vector<string> currProd;
        string token = "";
        for(int j = idx + 2; j < s.size(); j++){
            if(s[j] != ' '){
                token += s[j];
            } 
            else{
                if(token != "") RHS.push_back(token);
                token.clear();
            }
        }
        RHS.push_back(token);
        for(int j = 0; j < RHS.size(); j++){
            if(RHS[j] != "|"){
                currProd.push_back(RHS[j]);
            }
            else{
                Productions.push_back({LHS, currProd});
                currProd.clear();
            }
        }
        Productions.push_back({LHS, currProd});
        NonTerminals.insert(LHS);
    }
    for(auto prod : Productions){
        vector<string> RHS = prod.second;
        for(auto sym : RHS){
            if(!NonTerminals.count(sym)){
                Terminals.insert(sym);
            }
        }
    }
    findFirst();
    findFollow(Productions[0].first);
    printfirst();
    printfollow();
}