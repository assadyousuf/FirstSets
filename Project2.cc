/*
 * Copyright (C) Mohsen Zohrevandi, 2017
 *               Rida Bazzi 2019
 * Do not share this file with anyone
 */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "lexer.h"
#include <algorithm>
#include <vector>
#include <map>
using namespace std;
struct grammerRule{
    int LHS;
    vector <int> RHS;
    bool isGen=false;
    bool isReachable=false;
};
using namespace std;

vector <grammerRule*> rules;
vector <string> symbol; //symbol used to store
 LexicalAnalyzer lexer;
 vector <int> nonterminals;
 vector <int> terminals;
 vector <bool> generating;
 vector <bool> reachable;
vector <grammerRule*> generatingRules;
vector <grammerRule*> reachableRules;
 vector <int> usefullSymbols;
vector < vector<string> > firstSets;


bool checkIfInVector(vector <string> *vect, Token *t){
    vector<string>::iterator it=find(vect->begin(),vect->end(),t->lexeme);
    if(vect->size()==0){
        return false;
    }
    if(it==vect->end()){
        return false;
    }
    else {
        return true;
    }
}


int findIndexInVector(vector <string> *vect,Token *t ){
    for(int i=0; i<vect->size(); i++){
        if(vect->at(i)==t->lexeme){
            return i;
        }
    }
    return 0;
    
}

bool searchSet(vector <bool> vect, int arg){
    if(vect[arg]==true){
        return true;
    }
    else {
        return false;}
    
}

void reachableCalc(){
    bool change;
    reachable.resize(symbol.size());
    reachable[0]=true; //adding start symbol to reachable set
    change=true;
    
    while(change){
        change=false;
        for(int k=0; k<generatingRules.size(); k++){
            if(searchSet(reachable, generatingRules[k]->LHS)){
                for(int x=0;x<generatingRules[k]->RHS.size();x++){
                    if(searchSet(reachable, generatingRules[k]->RHS[x]) == false ){
                        reachable[generatingRules[k]->RHS[x]]=true;
                        change=true;
                
                    }
                }
                
            }
            
        }
        
    }
        
        for(int t=0; t<generatingRules.size(); t++){
            if(reachable[generatingRules[t]->LHS] == true){
                cout<<symbol[generatingRules[t]->LHS] << "->";
                for(int m=0; m<generatingRules[t]->RHS.size(); m++){
                    if(symbol[generatingRules[t]->RHS[m]] == ""){
                        cout<< "#" << " ";
                    }
                            cout << symbol[generatingRules[t]->RHS[m]] << " ";
                        }
                            cout<< "\n";
            }
            
        }
        
        
        
    

    
    
    
    
    
}


void generatingSet(){
    vector <string> falseq;
    
    bool change;
    change=true;
    
    generating.resize(symbol.size());
    for(int s=0; s<terminals.size(); s++){
        generating[terminals[s]]=true;
    }
    
    falseq.resize(generating.size());
    
    
    
    while(change){
        change=false;
    for(int i=0; i<rules.size(); i++){
        
        int j=0;
        while(j<rules[i]->RHS.size() && searchSet(generating, rules[i]->RHS[j]) ){
            j++;
        }
        
        //cout << "1\n";
        if(j==rules[i]->RHS.size() && searchSet(generating, rules[i]->LHS) ==false){
            //cout << "2\n";
            generating[rules[i]->LHS]=true;
            change=true;
        }
    }
        
    }
    
    
    for(int z=0; z<terminals.size(); z++){
        if(generating[z]==false){
            falseq.insert(falseq.begin()+0,symbol[z]);
        }
     
    }
    
    
    bool gen=true;
    //grammer gen
   // for(int i=0; i<generating.size(); i++){
        //if(generating[i]==true){
            for(int q=0; q<rules.size(); q++ ){
                if(generating[rules[q]->LHS]==true){
                    for(int f=0; f<rules[q]->RHS.size(); f++){
                        if(symbol[rules[q]->RHS[f]] == falseq[f]  && symbol[rules[q]->RHS[f]]!=""){
                            gen=false;
                        }
                        
                    }
                    if(gen!=false && generatingRules.size()<= rules.size()){
                    generatingRules.push_back(rules[q]);}
                    gen=true;
                }
            } //loop through ruleset and check if the lHS of each rule matches the generating symbols
            
       // }
        
    //}
    
    reachableCalc();
    
}

// read grammar
void ReadGrammar()
{
    Token t=lexer.GetToken();
    lexer.UngetToken(t);
    while(t.token_type!=END_OF_FILE){
    t=lexer.GetToken();
    if(t.token_type==ID){
       //vector<string>::iterator it=find(symbol.begin(),symbol.end(),t.lexeme);
        grammerRule *rule=new grammerRule();
       
        if(checkIfInVector(&symbol, &t) == false){
        symbol.push_back(t.lexeme);
        }
        
        rule->LHS=findIndexInVector(&symbol, &t);
        if(find(nonterminals.begin(), nonterminals.end(), rule->LHS) == nonterminals.end()){
        nonterminals.push_back(rule->LHS);
        }
        
        t=lexer.GetToken();
        if(t.token_type==ARROW){
            t=lexer.GetToken();
            if(checkIfInVector(&symbol, &t) == false){
                symbol.push_back(t.lexeme);
            }
            if(t.token_type==HASH){
                rule->RHS.push_back(findIndexInVector(&symbol, &t));
            }
            while(t.token_type!=HASH){
                if(checkIfInVector(&symbol, &t) == false){
                       symbol.push_back(t.lexeme);
                      
                       }
                rule->RHS.push_back(findIndexInVector(&symbol, &t));
                t=lexer.GetToken();
            }
           
            rules.push_back(rule);
        }
    } if(t.token_type==DOUBLEHASH){
        symbol.push_back("END_OF_FILE");
        return;
    }
    
    }
        
    
    
    
    cout << "0\n";
}

// Task 1
void printTerminalsAndNoneTerminals()
{
    sort(nonterminals.begin(), nonterminals.end()); //sorts nonterminals vector
    //printing nonterimnals
    for(int i=0; i<nonterminals.size(); i++){
        int index=nonterminals.at(i);
        cout<<symbol[index]<<" ";
    }
    
   //printing terminals anything thats not a nonterminal is a terminal
    for(int j=0; j<symbol.size(); j++){
        if(find(nonterminals.begin(),nonterminals.end(),j) == nonterminals.end() && symbol[j]!="END_OF_FILE"){
            cout << symbol[j] << " ";
            terminals.push_back(j);
        }
    }
   
}

void setTerminals(){
    
    for(int j=0; j<symbol.size(); j++){
           if(find(nonterminals.begin(),nonterminals.end(),j) == nonterminals.end() && symbol[j]!="END_OF_FILE"){
               //cout << symbol[j] << " ";
               terminals.push_back(j);
           }
    }
    
}
    
    
    
    
  

// Task 2
void RemoveUselessSymbols()
{
    setTerminals();
    generatingSet();
    //cout << "2\n";
}

// Task 3
void CalculateFirstSets()
{
    bool change;
    setTerminals();
    firstSets.resize(symbol.size());
    //loop thorugh symbols and see which ones are terminals and add them to firstSet of each terminal
    for(int u=0; u<terminals.size(); u++){
        for(int v=0; v<symbol.size(); v++){
            if(v==terminals[u]){
                if(symbol[terminals[u]]==""){
                    firstSets[v].insert(firstSets[v].begin(),"#");
                } else {
                    firstSets[v].insert(firstSets[v].begin(),symbol[terminals[u]]);} //insert the symbol of the index given inside of terminal to first set of that index
            }
        }
    }
    
    change=true;
    while(change){
        change=false;
        for(grammerRule *rule:rules){
            for(int i:rule->RHS){
                if( find(firstSets[i].begin(),firstSets[i].end(),"#") !=  firstSets[i].end() ){
                    for(string s:firstSets[i]){
                        if(s=="#" && i+1>=rule->RHS.size() && find(firstSets[rule->LHS].begin(),firstSets[rule->LHS].end(),"#") == firstSets[rule->LHS].end()){
                            firstSets[rule->LHS].push_back("#");
                            change=true;
                        }
                    }
                }else{
                    for(string s:firstSets[i]){
                        if(find(firstSets[rule->LHS].begin(),firstSets[rule->LHS].end(),s) == firstSets[rule->LHS].end()){
                            firstSets[rule->LHS].push_back(s); change=true;}}
                    break;
                }
            
            }
            
}
    }
    
    sort(nonterminals.begin(), nonterminals.end());
    
    for(int m=0; m<nonterminals.size();m++){
        cout<<"FIRST("<<symbol[nonterminals[m]] << ") = { ";
        for(int n=0; n<firstSets[nonterminals[m]].size(); n++){
            if(n==0 && firstSets[nonterminals[m]].size()==1){
                sort(firstSets[nonterminals[m]].begin(),firstSets[nonterminals[m]].end());
                cout << firstSets[nonterminals[m]][n];
            }
            else if(n+1>=firstSets[nonterminals[m]].size()){
                    cout << firstSets[nonterminals[m]][n];
            }else {
            cout << firstSets[nonterminals[m]][n]<< ", ";}
                
                
        }
        cout << " }\n";
                                 }
}


// Task 4
void CalculateFollowSets()
{
   // cout << "4\n";
}

// Task 5
void CheckIfGrammarHasPredictiveParser()
{
    cout << "5\n";
}
    
int main (int argc, char* argv[])
{
    int task;
   
    
    
   if (argc < 2)
    {
        cout << "Error: missing argument\n";
        return 1;
    }

    /*
       Note that by convention argv[0] is the name of your executable,
       and the first argument to your program is stored in argv[1]
     */

    task = atoi(argv[1]);

    
    ReadGrammar();  // Reads the input grammar from standard input
                    // and represent it internally in data structures
                    // ad described in project 2 presentation file
    
    
    
    
    
    
    switch (task) {
        case 1: printTerminalsAndNoneTerminals();
            break;

        case 2: RemoveUselessSymbols();
            break;

        case 3: CalculateFirstSets();
            break;

        case 4: CalculateFollowSets();
            break;

        case 5: CheckIfGrammarHasPredictiveParser();
            break;

        default:
            cout << "Error: unrecognized task number " << task << "\n";
            break;
    }
    
    return 0;
}

