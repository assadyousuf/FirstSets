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
 vector <bool> generatingSymbols;
 vector <bool> reachable;
 vector <int> usefullSymbols;


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
    
    reachable.resize(generatingSymbols.size());
    //reachable[0]=true;
  
    
    for(int w=0;w<generatingSymbols.size();w++){
        reachable[w]=false;
        
    }
    
    reachable[0]=true;
    change=true;
    while(change){
    for(int j=0; j<rules.size(); j++){
        if(rules[j]->isGen){
            if(searchSet(reachable,rules[j]->LHS) ){
                for(int d=0; d<rules[j]->RHS.size(); d++){
                    if(searchSet(reachable,rules[j]->RHS[d]) ==false){
                        reachable[rules[j]->RHS[d]]=true;
                        change=true;
                    }
                    
                }
                
            }
            
            
        }
        
        
    }
    }
    
    
  
    
    
    
    
    
    
    
    
    for(int u=0; u<rules.size(); u++){
        if(rules[u]->LHS == reachable[u]){
            cout << rules[u]->LHS << "-> ";
            for(int y=0; y<rules[u]->RHS.size(); y++){
                cout << rules[u]->RHS[y];
            }
        }
    }
    
    
    
}


void generatingSet(){
 
    generatingSymbols.resize(symbol.size());
 //add epsilon and all terminals to generating set
    //generatingSymbols.resize(symbol.size());
    for(int i=0; i<terminals.size(); i++){
        generatingSymbols[terminals[i]]=true;
    }
    
    bool change=true;
    bool gen=false;
    int loops=0;
    
    while(change){
        loops++;
        change=false;
        for(int j=0; j<rules.size(); j++){
            for(int v=0; v<rules[j]->RHS.size(); v++){
                if( searchSet(generatingSymbols, rules[j]->RHS[v]) ){
                    gen=true;
                } else{
                    gen=false;
                }
                
            }
            if(gen==true && generatingSymbols[rules[j]->LHS]==false){
                generatingSymbols[rules[j]->LHS]=true;
                change=true;
            }
        }
    }
    
    //remove rules with nongenerating symbols
    for(int r=0; r<rules.size(); r++){
        for(int l=0; l<rules[r]->RHS.size(); l++){
            if(searchSet(generatingSymbols,rules[r]->RHS[l]) == false ){
                rules[r]->isGen=false;
            }
            else {
                rules[r]->isGen=true;
            }
        }
        
    }
    
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
        if(find(nonterminals.begin(),nonterminals.end(),j) == nonterminals.end() && symbol[j]!=""){
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
    
    cout << "2\n";
}

// Task 3
void CalculateFirstSets()
{
    cout << "3\n";
}

// Task 4
void CalculateFollowSets()
{
    cout << "4\n";
}

// Task 5
void CheckIfGrammarHasPredictiveParser()
{
    cout << "5\n";
}
    
int main ()
{
    //int task;
   
    
    
    /*if (argc < 2)
    {
        cout << "Error: missing argument\n";
        return 1;
    }*/

    /*
       Note that by convention argv[0] is the name of your executable,
       and the first argument to your program is stored in argv[1]
     */

    //task = atoi(argv[1]);

    
    ReadGrammar();  // Reads the input grammar from standard input
                    // and represent it internally in data structures
                    // ad described in project 2 presentation file
    
    
    //printTerminalsAndNoneTerminals();
    setTerminals();
    generatingSet();
    reachableCalc();
    
    for(int i=0; i<reachable.size(); i++){
        if(reachable[i]==true){
            cout<<symbol[reachable[i]];
        }
    }
    
    /*switch (task) {
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
    }*/
    
    return 0;
}

