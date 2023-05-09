#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <unordered_set>
#include "lexer.h"

using namespace std;

bool print = false;
bool task5 = true;
bool DoesNotHaveUselessSymbols = false;

bool isInVector(const vector<string>& vec, const string& value) {
    return find(vec.begin(), vec.end(), value) != vec.end();
}
bool hasNonGeneratingSymbols(const pair<string, vector<string> > &rule, const set<string> &generatingSymbols, const vector<string> &nonTerminals) {
    for (const auto &symbol : rule.second) {
        if (isInVector(nonTerminals, symbol) && generatingSymbols.count(symbol) == 0) { // if it's a nonterminal and not generating
            
            return true;
        }
    }
    return false;
}

//Task 2 

void RemoveUselessSymbols(Token& current_token, LexicalAnalyzer& myLexer) {
    // Grammar rules in the format: {LHS, {RHS tokens}}
    //create grammarrules vector and nonterminals
    vector<pair<string, vector<string> > > grammarRules;
    vector<string> nonTerminals;

    while (current_token.token_type != HASH) {
        string lhs = current_token.lexeme;
        vector<string> rhs; // RHS tokens for current rule

        // Add lhs to nonTerminals if it's not already there
        if (!isInVector(nonTerminals, lhs)) {
            nonTerminals.push_back(lhs); 
        }

        current_token = myLexer.GetToken(); // go to arrow
        current_token = myLexer.GetToken(); // skip arrow

        while (current_token.token_type != STAR) {
            string rhsToken = current_token.lexeme;
            rhs.push_back(rhsToken);
            current_token = myLexer.GetToken();
        }
        grammarRules.push_back(make_pair(lhs, rhs));

        current_token = myLexer.GetToken(); // Skip STAR
    }
    //print grammar rules
        // cout << "OG Grammar Rules:" << endl;
        // for (const auto &rule : grammarRules) {
        //     const auto &lhs = rule.first;
        //     const auto &rhs = rule.second;

        //     cout << lhs << " -> ";
        //     if (rhs.empty()) {
        //         cout << "#";
        //     } else {
        //         for (const auto &symbol : rhs) {
        //             cout << symbol << " ";
        //         }
        //     }
        //     cout << endl;
        // }
        // cout << "\nend of grammar rules\n" << endl;
    vector<pair<string, vector<string> > > originalGrammarRules = grammarRules;

    // Step 1: Remove non-generating symbols
    set<string> generatingSymbols;

    bool changed = true;
    while (changed) { // While we're still making changes
        changed = false;
        for (const auto &rule : grammarRules) { // for each rule
            const auto &lhs = rule.first; // LHS of the rule
            const auto &rhs = rule.second; // RHS of the rule

            if (generatingSymbols.count(lhs) > 0) { // If the LHS is already generating, skip it
                continue;
            }

            bool allSymbolsGenerating = true;
            for (const auto &symbol : rhs) { // for each symbol in the RHS
                if (isInVector(nonTerminals, symbol) && generatingSymbols.count(symbol) == 0) { // if it's a nonterminal and not generating
                allSymbolsGenerating = false; // then the rule is not generating
                //DoesNotHaveUselessSymbols = false;
                //task5 = false;
                break; // break out of the loop and go to the next rule
                }
            }
            if (allSymbolsGenerating) {
                generatingSymbols.insert(lhs); // Add the LHS to the generating symbols
                changed = true;
            }
            //     DoesNotHaveUselessSymbols = false;}
        }
    }
    //if a nonterminal in the righthandside not generating, remove the entire rule from the grammar
    for (auto it = grammarRules.begin(); it != grammarRules.end(); ++it) {
        if (hasNonGeneratingSymbols(*it, generatingSymbols, nonTerminals)) {
            it = grammarRules.erase(it);
            --it;
        }
    }

    // Step 2: Remove non-reachable symbols
    set<string> reachableSymbols;
    reachableSymbols.insert(nonTerminals[0]); // Add the start symbol to the reachable symbols

    changed = true;
    while (changed) { // While we're still making changes
        changed = false;
        for (const auto &rule : grammarRules) { // for each rule
            const auto &lhs = rule.first; // LHS of the rule
            const auto &rhs = rule.second; // RHS of the rule

            if (reachableSymbols.count(lhs) == 0) { // If the LHS is not reachable, skip it
                //cout << "skip rule not reachable!\n";
                continue;
            }

            for (const auto &symbol : rhs) { // for each symbol in the RHS 
    if (isInVector(nonTerminals, symbol) && reachableSymbols.count(symbol) == 0) { // if it's a nonterminal and not reachable
        //DoesNotHaveUselessSymbols = false;
        reachableSymbols.insert(symbol); // then add it to the reachable symbols
        changed = true; // and set changed to true
            }
        }
    }
}

    vector<string> formattedRules;
    for (const auto &rule : grammarRules) {
        const auto &lhs = rule.first;
        const auto &rhs = rule.second;
        string formattedRule;

        if (generatingSymbols.count(lhs) == 0 || reachableSymbols.count(lhs) == 0) {
            continue;
        }

        formattedRule += lhs + " -> ";
        if (rhs.empty()) {
            formattedRule += "#";
        } else {
            for (const auto &symbol : rhs) {
                if (!isInVector(nonTerminals, symbol) || generatingSymbols.count(symbol) > 0) {
                    formattedRule += symbol + " ";
                }
            }
        }
        formattedRules.push_back(formattedRule);
    }


    if (print == true) {
        for (const auto &rule : formattedRules) {
            cout << rule << endl;
        }
    }
    //print grammarrules
    //  cout << "Grammar Rules: after gen" << endl;
    //  for (const auto &rule : grammarRules) {
    //         const auto &lhs = rule.first;
    //         const auto &rhs = rule.second;
    
    //         cout << lhs << " -> ";
    //         if (rhs.empty()) {
    //             cout << "#";
    //         } else {
    //             for (const auto &symbol : rhs) {
    //                 cout << symbol << " ";
    //             }
    //         }
    //         cout << endl;
    //     }
    //     cout << "\nend of grammar rules\n" << endl;
    //print formatted rules
    // cout << "Formatted Rules:" << endl;
    // for (const auto &rule : formattedRules) {
    //     cout << rule << endl;
    // }
    // cout << "\nend of formatted rules\n" << endl;
    // if (originalGrammarRules.size() == formattedRules.size()) {
    //     DoesNotHaveUselessSymbols = true;
    // }
}   

void PrintTerminalsAndNonTerminals(const vector<string>& terminals, const vector<string>& nonTerminals) {
    vector<string> printedNonTerminals;

    for (const auto& terminal : terminals) {
        cout << terminal << " ";
    }

    for (const auto& nonTerminal : nonTerminals) {
        if (!isInVector(printedNonTerminals, nonTerminal)) {
            cout << nonTerminal << " ";
            printedNonTerminals.push_back(nonTerminal);
        }
    }
}
// Task 1
std::pair<vector<string>, vector<string> > GetTerminalsAndNonTerminals(Token& current_token, LexicalAnalyzer& myLexer)
{
    vector<string> terminals;
    vector<string> nonTerminals;
    vector<string> orderedNonTerminals;
    bool previousStar = false; // Add a boolean variable to track if the previous token was a star

    while (current_token.token_type != HASH) {
        string lhs = current_token.lexeme;

        if (!isInVector(nonTerminals, lhs)) {
            nonTerminals.push_back(lhs);
        }

        if (isInVector(terminals, lhs)) {
            terminals.erase(remove(terminals.begin(), terminals.end(), lhs), terminals.end());
        }

        current_token = myLexer.GetToken(); // go to arrow
        //if not arrow, error
        if (current_token.token_type != ARROW) {
            cout << "SYNTAX ERROR !!!" << endl;
            print = false;
            break;
        } 
        current_token = myLexer.GetToken(); // skip arrow

        while (current_token.token_type != STAR && current_token.token_type != ARROW && current_token.token_type != HASH) {
            string rhs = current_token.lexeme;

            if (!isInVector(nonTerminals, rhs) && !isInVector(terminals, rhs)) {
                terminals.push_back(rhs);
            }
            nonTerminals.push_back(rhs);
            current_token = myLexer.GetToken();
        }
        if(current_token.token_type == ARROW || current_token.token_type == HASH){
            cout << "SYNTAX ERROR !!!" << endl;
            print = false;
            break;
        }

        // Check if the current token is a star and peek at the next token
        if (current_token.token_type == STAR && myLexer.peek(1).token_type == STAR) {
            cout << "SYNTAX ERROR !!!" << endl;
            print = false;
            break;
        }

        // Set previousStar to true if the current token is a star
        previousStar = (current_token.token_type == STAR);
        
        current_token = myLexer.GetToken(); // Skip STAR
    }

    // Remove terminals from nonterminals
    for (const auto& terminal : terminals) {
        nonTerminals.erase(remove(nonTerminals.begin(), nonTerminals.end(), terminal), nonTerminals.end());
    }

    if (print == true) {
        PrintTerminalsAndNonTerminals(terminals, nonTerminals);
    }

    return std::make_pair(terminals, orderedNonTerminals);
}

vector<pair<string, vector<string> > > GetGrammarRules(Token& current_token, LexicalAnalyzer& myLexer) {
    vector<pair<string, vector<string> > > grammarRules;
    vector<string> nonTerminals;
    vector<string> terminals;

    // Extract the grammar rules first
    while (current_token.token_type != HASH) {
        string lhs = current_token.lexeme;
        vector<string> rhs; // RHS tokens for current rule

        // Add lhs to nonTerminals if it's not already there
        if (!isInVector(nonTerminals, lhs)) {
            nonTerminals.push_back(lhs); 
        }

        current_token = myLexer.GetToken(); // go to arrow
        current_token = myLexer.GetToken(); // skip arrow

        while (current_token.token_type != STAR) {
            string rhsToken = current_token.lexeme;
            rhs.push_back(rhsToken);
            current_token = myLexer.GetToken();
        }
        grammarRules.push_back(make_pair(lhs, rhs));

        current_token = myLexer.GetToken(); // Skip STAR
    }
    return grammarRules;  
}

// Task 3
map<string, set<string> > ComputeFirstSets(const vector<pair<string, vector<string> > >& grammarRules, const vector<string>& nonTerminals) {
    map<string, set<string> > firstSets;

    // Initialize the first sets with epsilon for non-terminals that have epsilon production
    for (const auto& rule : grammarRules) {
        if (rule.second.size() == 1 && rule.second[0] == "#") {
            firstSets[rule.first].insert("#");
        }
    }

    bool changed = true;
    while (changed) {
        changed = false;

        for (const auto& rule : grammarRules) {
            const string& lhs = rule.first;
            const vector<string>& rhs = rule.second;

            // Save the old size of the FIRST set to compare later
            size_t oldSize = firstSets[lhs].size();

            bool allRhsProduceEmpty = true;
            for (const auto& symbol : rhs) { // For each symbol in the RHS
                if (symbol == "#") { // If the symbol is epsilon
                    continue; // Skip it
                }
                
                
                if (find(nonTerminals.begin(), nonTerminals.end(), symbol) == nonTerminals.end()) { // If the symbol is a terminal
                    firstSets[lhs].insert(symbol); // Add the terminal to the FIRST set
                    allRhsProduceEmpty = false; // The rule does not produce epsilon
                    break; // Stop processing the RHS
                } else {
                
                    for (const auto& firstElement : firstSets[symbol]) { // For each element in the FIRST set of the symbol
                        if (firstElement != "#") { // If the element is not epsilon
                            firstSets[lhs].insert(firstElement); // Add it to the FIRST set
                        }
                        
                    }

                    // If the current symbol does not produce epsilon
                    if (firstSets[symbol].find("#") == firstSets[symbol].end()) { 
                        allRhsProduceEmpty = false; // then the rule does not produce epsilon
                        break;
                    }
                }
            }

            if (allRhsProduceEmpty) { // If the rule produces epsilon
                firstSets[lhs].insert("#"); // Add epsilon to the FIRST set
            }

            // If the FIRST set has changed, set the changed flag to true
            if (firstSets[lhs].size() != oldSize) {
                changed = true;
            }
        }
    }

    return firstSets;
}

void CalculateFirstSets(Token& current_token, LexicalAnalyzer& myLexer)
{
    vector<string> terminals;
    vector<string> nonTerminals;
    vector<string> orderedNonTerminals;
    set<string> seenNonTerminals; // A set to check for duplicates

    auto grammarRules = GetGrammarRules(current_token, myLexer); 

    while (current_token.token_type != HASH) {
        string lhs = current_token.lexeme;

        if (!isInVector(nonTerminals, lhs)) {
            nonTerminals.push_back(lhs);
        }

        if (isInVector(terminals, lhs)) {
            terminals.erase(remove(terminals.begin(), terminals.end(), lhs), terminals.end());
        }

        current_token = myLexer.GetToken(); // go to arrow
        current_token = myLexer.GetToken(); // skip arrow

        while (current_token.token_type != STAR) {
            string rhs = current_token.lexeme;

            if (!isInVector(nonTerminals, rhs) && !isInVector(terminals, rhs)) {
                terminals.push_back(rhs);
            }
            nonTerminals.push_back(rhs);
            current_token = myLexer.GetToken();
        }
        current_token = myLexer.GetToken(); // Skip STAR
    }
    //remove terminals from nonterminals
    for (const auto& terminal : terminals) {
        nonTerminals.erase(remove(nonTerminals.begin(), nonTerminals.end(), terminal), nonTerminals.end());
    }
    //MIMIC HERE
    for (const auto& rule : grammarRules) {
        // Add the left side (non-terminal) to the nonTerminals vector if it's not already present
        if (find(nonTerminals.begin(), nonTerminals.end(), rule.first) == nonTerminals.end()) { // If the non-terminal is not in nonTerminals
            nonTerminals.push_back(rule.first);
            orderedNonTerminals.push_back(rule.first); // Add the non-terminal to orderedNonTerminals
        }

        // Iterate through the right side symbols
        for (const auto& symbol : rule.second) {
            // If the symbol is not in nonTerminals and not in terminals, it must be a terminal
            if (find(nonTerminals.begin(), nonTerminals.end(), symbol) == nonTerminals.end() &&
                find(terminals.begin(), terminals.end(), symbol) == terminals.end()) {
                terminals.push_back(symbol);
            }
            // If the symbol is in the nonTerminals vector and not in orderedNonTerminals, add it to orderedNonTerminals
            else if (find(nonTerminals.begin(), nonTerminals.end(), symbol) != nonTerminals.end() &&
                     find(orderedNonTerminals.begin(), orderedNonTerminals.end(), symbol) == orderedNonTerminals.end()) {
                orderedNonTerminals.push_back(symbol);
                     }
        }
    }

vector<string> printedNonTerminals; // To keep track of printed non-terminals
vector<string> mynewNonTerminals; // To keep track of printed non-terminals
for (const auto& nonTerminal : orderedNonTerminals) {
    if (isInVector(orderedNonTerminals, nonTerminal) && !isInVector(printedNonTerminals, nonTerminal)) { // If it's a non-terminal that hasn't been printed yet
        printedNonTerminals.push_back(nonTerminal);
        mynewNonTerminals.push_back(nonTerminal);
        //cout << nonTerminal << " here1" << endl;
        for (const auto& rule : grammarRules) {
            if (rule.first == nonTerminal) {
                vector<string> printedSymbols; // To keep track of printed symbols
                for (const auto& symbol : rule.second) {
                    if (isInVector(orderedNonTerminals, symbol) && !isInVector(printedNonTerminals, symbol) && !isInVector(printedSymbols, symbol)) {
                        printedSymbols.push_back(symbol);
                        mynewNonTerminals.push_back(symbol);
                        //cout << symbol << " here2" << endl; // If it's a terminal or a non-terminal that has already been printed, skip it
                    }
                }
                //cout << "here3" << endl;
            }
        }
    }
}
// Remove duplicates from mynewNonTerminals
mynewNonTerminals.erase(unique(mynewNonTerminals.begin(), mynewNonTerminals.end()), mynewNonTerminals.end());
//print mynewNonTerminals
vector <string> uniqueNonTerminals; // To keep track of unique non-terminals
for (const auto& nonTerminal : mynewNonTerminals) {
    // If the nonTerminal is not in seenNonTerminals, it's a unique element
    if (seenNonTerminals.find(nonTerminal) == seenNonTerminals.end()) {
        uniqueNonTerminals.push_back(nonTerminal); // Add the unique element to the uniqueNonTerminals vector
        seenNonTerminals.insert(nonTerminal); // Add the nonTerminal to the seenNonTerminals set
    }
}
mynewNonTerminals = uniqueNonTerminals;

     map<string, set<string> > firstSets = ComputeFirstSets(grammarRules, nonTerminals);
    //print grammarRules
    //SAME
     for (const auto& nonTerminal : mynewNonTerminals) {
    cout << "FIRST(" << nonTerminal << ") = { ";

    const set<string>& firstSet = firstSets[nonTerminal];
    bool firstElement = true;

    // If the first element of the RHS is empty, print #
    if (firstSet.find("#") != firstSet.end()) {
        cout << "#";
        firstElement = false;
    }

    for (const auto& terminal : terminals) {
        if (firstSet.find(terminal) != firstSet.end()) {
            if (!firstElement) {
                cout << ", ";
            }
            cout << terminal;
            firstElement = false;
        }
    }
    cout << " }\n";
}

}

// Task 4
map<string, set<string> > ComputeFollowSets(const vector<pair<string, vector<string> > >& grammarRules,
                                           const vector<string>& nonTerminals, const vector<string>& terminals,
                                           const map<string, set<string> >& firstSets) {
    map<string, set<string> > followSets;

    followSets[nonTerminals[0]].insert("$");  // Add EOF to the start symbol FOLLOW set

    bool changed = true;
    while (changed) {
        changed = false;

        for (const auto& rule : grammarRules) {
            const string& lhs = rule.first;
            const vector<string>& rhs = rule.second;

            for (size_t i = 0; i < rhs.size(); ++i) {
                const string& symbol = rhs[i];

                if (find(nonTerminals.begin(), nonTerminals.end(), symbol) != nonTerminals.end()) {
                    size_t oldSize = followSets[symbol].size();

                    // If this is not the last symbol in the RHS
                    if (i + 1 < rhs.size()) {
                        const string& nextSymbol = rhs[i + 1];
                        if (find(nonTerminals.begin(), nonTerminals.end(), nextSymbol) != nonTerminals.end()) {
                            for (const auto& firstElement : firstSets.at(nextSymbol)) {
                                if (firstElement != "#") {
                                    followSets[symbol].insert(firstElement);
                                }
                            }
                            if (firstSets.at(nextSymbol).count("#") > 0) {
                                bool allProduceEmpty = true;
                                for (size_t j = i + 1; j < rhs.size(); ++j) {
                                    const string& nextNextSymbol = rhs[j];
                                    if (find(nonTerminals.begin(), nonTerminals.end(), nextNextSymbol) != nonTerminals.end()) {
                                        if (firstSets.at(nextNextSymbol).count("#") == 0) {
                                            allProduceEmpty = false;
                                            break;
                                        }
                                    } else {
                                        allProduceEmpty = false;
                                        break;
                                    }
                                }
                                if (allProduceEmpty) {
                                    followSets[symbol].insert(followSets[lhs].begin(), followSets[lhs].end());
                                }
                            }
                        } else {
                            followSets[symbol].insert(nextSymbol);
                        }
                    } else {
                        followSets[symbol].insert(followSets[lhs].begin(), followSets[lhs].end());
                    }

                    if (followSets[symbol].size() != oldSize) {
                        changed = true;
                    }
                }
            }
        }
    }

    return followSets;
}




void CalculateFollowSets(Token& current_token, LexicalAnalyzer& myLexer) {
    vector<string> terminals;
    vector<string> nonTerminals;
    vector<string> orderedNonTerminals;
    set <string> seenNonTerminals; // To keep track of seen non-terminals
    auto grammarRules = GetGrammarRules(current_token, myLexer);
    //print grammarRules
    //cout << "Grammar Rules:\n";
    for (const auto& rule : grammarRules) {
        if (find(nonTerminals.begin(), nonTerminals.end(), rule.first) == nonTerminals.end()) {
            nonTerminals.push_back(rule.first);
            orderedNonTerminals.push_back(rule.first);
        }

        for (const auto& symbol : rule.second) {
            if (find(terminals.begin(), terminals.end(), symbol) == terminals.end() &&
                find(nonTerminals.begin(), nonTerminals.end(), symbol) == nonTerminals.end()) {
                terminals.push_back(symbol);
            }
            // If the symbol is in the nonTerminals vector and not in orderedNonTerminals, add it to orderedNonTerminals
            else if (find(nonTerminals.begin(), nonTerminals.end(), symbol) != nonTerminals.end() &&
                     find(orderedNonTerminals.begin(), orderedNonTerminals.end(), symbol) == orderedNonTerminals.end()) {
                orderedNonTerminals.push_back(symbol);
                     }
        }
    }
    vector<string> printedNonTerminals; // To keep track of printed non-terminals
vector<string> mynewNonTerminals; // To keep track of printed non-terminals
for (const auto& nonTerminal : orderedNonTerminals) {
    if (isInVector(orderedNonTerminals, nonTerminal) && !isInVector(printedNonTerminals, nonTerminal)) { // If it's a non-terminal that hasn't been printed yet
        printedNonTerminals.push_back(nonTerminal);
        mynewNonTerminals.push_back(nonTerminal);
        //cout << nonTerminal << " here1" << endl;
        for (const auto& rule : grammarRules) {
            if (rule.first == nonTerminal) {
                vector<string> printedSymbols; // To keep track of printed symbols
                for (const auto& symbol : rule.second) {
                    if (isInVector(orderedNonTerminals, symbol) && !isInVector(printedNonTerminals, symbol) && !isInVector(printedSymbols, symbol)) {
                        printedSymbols.push_back(symbol);
                        mynewNonTerminals.push_back(symbol);
                        //cout << symbol << " here2" << endl; // If it's a terminal or a non-terminal that has already been printed, skip it
                    }
                }
                //cout << "here3" << endl;
            }
        }
    }
}
// Remove duplicates from mynewNonTerminals
mynewNonTerminals.erase(unique(mynewNonTerminals.begin(), mynewNonTerminals.end()), mynewNonTerminals.end());
//print mynewNonTerminals
vector <string> uniqueNonTerminals; // To keep track of unique non-terminals
for (const auto& nonTerminal : mynewNonTerminals) {
    // If the nonTerminal is not in seenNonTerminals, it's a unique element
    if (seenNonTerminals.find(nonTerminal) == seenNonTerminals.end()) {
        uniqueNonTerminals.push_back(nonTerminal); // Add the unique element to the uniqueNonTerminals vector
        seenNonTerminals.insert(nonTerminal); // Add the nonTerminal to the seenNonTerminals set
    }
}
mynewNonTerminals = uniqueNonTerminals;
    //cout << "end of grammar rules\n";
    map<string, set<string> > firstSets = ComputeFirstSets(grammarRules, nonTerminals);
    map<string, set<string> > followSets = ComputeFollowSets(grammarRules, nonTerminals, terminals, firstSets);

    for (const auto& nonTerminal : mynewNonTerminals) {
        cout << "FOLLOW(" << nonTerminal << ") = { ";

        const set<string>& followSet = followSets[nonTerminal];
        bool firstElement = true;

        if (followSet.find("$") != followSet.end()) {
            cout << "$";
            firstElement = false;
        }

        for (const auto& terminal : terminals) {
            if (followSet.find(terminal) != followSet.end()) {
                if (!firstElement) {
                    cout << ", ";
                }
                cout << terminal;
                firstElement = false;
            }
        }
        cout << " }\n";
    }
}


// Task 5
void CheckIfGrammarHasPredictiveParser(Token& current_token, LexicalAnalyzer& MyLexer )
{
    RemoveUselessSymbols(current_token, MyLexer);
    if(DoesNotHaveUselessSymbols == true){
        cout << "YES";
    }
    else if(DoesNotHaveUselessSymbols == false){
        cout << "NO";
    }
    else{
        cout << "Error: missing argument\n";
    }
    
}


int main (int argc, char* argv[]){
    int task;
    LexicalAnalyzer myLexer;
   

    if(*argv[1] == '0'){
       
    }
    else if(*argv[1] == '1'){   
    }else if(*argv[1] == '2'){   
    }else if(*argv[1] == '3'){  
    }else if(*argv[1] == '4'){  
    }else if(*argv[1] == '5'){
    }else{
        cout << "Error: missing argument\n";
    }
    if (argc < 2){
        cout << "Error: missing argument\n";
        return 1;
    }
    task = atoi(argv[1]);
    
    Token current_token = myLexer.GetToken();
 
    switch (task) {
        case 1: print = true;
                GetTerminalsAndNonTerminals(current_token,myLexer);  
            break;

        case 2: print = true;
                RemoveUselessSymbols(current_token,myLexer);
            break;

        case 3: CalculateFirstSets(current_token,myLexer);
            break;

        case 4: CalculateFollowSets(current_token,myLexer);
            break;

        case 5: print = false;
            CheckIfGrammarHasPredictiveParser(current_token,myLexer);

        
            break;

        default:
            cout << "Error: unrecognized task number " << task << "\n";
            break;
    }
    return 0;
}

