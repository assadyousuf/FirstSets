#include "project2.h"
#include <set>

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
    for(int s:terminals){
        generating[s]=true;
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
    //bool isFirstEpsilon=false;
     symbol.insert(symbol.begin(), "#");
    while(t.token_type!=END_OF_FILE){
        
       
    t=lexer.GetToken();
    if(t.token_type==ID){
       //vector<string>::iterator it=find(symbol.begin(),symbol.end(),t.lexeme);
        grammerRule *rule=new grammerRule();
       
        if(checkIfInVector(&symbol, &t) == false){ //checks if rhs is in symbol table otherwise adds it
        symbol.push_back(t.lexeme);
        }
        
        rule->LHS=findIndexInVector(&symbol, &t);
        if(find(nonterminals.begin(), nonterminals.end(), rule->LHS) == nonterminals.end()){
        nonterminals.insert(rule->LHS);
        }
        
        t=lexer.GetToken();
        if(t.token_type==ARROW){
            t=lexer.GetToken();
            if(checkIfInVector(&symbol, &t) == false && t.lexeme!=""){
                symbol.push_back(t.lexeme);
            }
            if(t.token_type==HASH){
                rule->isFirstEpsilon=true;
                rule->RHS.push_back(0);
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
        //symbol.push_back("END_OF_FILE");
        return;
    }
    
    }
        
    
    
    
    cout << "0\n";
}

// Task 1
void printTerminalsAndNoneTerminals()
{
    for(int u=1;u<symbol.size();u++){
                cout<<symbol[u]<<" ";
            
        }
    
    
   
}

void setTerminals(){
    
    for(int j=0; j<symbol.size(); j++){
           if(find(nonterminals.begin(),nonterminals.end(),j) == nonterminals.end() &&  symbol[j]!="#"){
               //cout << symbol[j] << " ";
            terminals.insert(j);
           }
    }
    
}

vector <string> containEpsilon(vector <string> set){
    for(int bf=0;bf<set.size();bf++){
        if(set[bf]=="#"){
            set.erase(set.begin()+bf);
            return set;
        }
    }
    return set;
    
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

    setTerminals();
    applyFirstSetRuleOne();
    applyFirstSetRuleTwo();
    applyFirstSetRule345();
    printFirstSets();
    
    
    
   
    
    //printOutFirstSets();
}
    
    
    
   
    
    


// Task 4
void CalculateFollowSets()
{
    applyFirstSetRuleOne();
    applyFirstSetRuleTwo();
    applyFirstSetRule345();
    printFirstSets();
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
        case 1:
            setTerminals();
            printTerminalsAndNoneTerminals();
            break;

        case 2: RemoveUselessSymbols();
            break;

        case 3: CalculateFirstSets();
                //printOutFirstSets();
                
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

bool ifDoesNotExis(vector<int> setfrom, int q){
    for(int y=0;y<setfrom.size();y++){
        if(setfrom[y]==q){
            return true;
        }
    }
    return false;
}


    
    
void applyFirstSetRuleOne(){
    if(symbol[0]=="#"){
        //Vector[0].insert("#");
        Vector.resize(symbol.size());
        Vector[0].insert(0);
    }
    
}

void applyFirstSetRuleTwo(){
    for(int t:terminals){
        Vector[t].insert(t);
    }
}


void applyFirstSetRule345(){
    change=true;
    int counter=1;
    while(change){
        change=false;
        cout<<counter<<"\n";
        for(int i=0;i<rules.size();i++){
            //cout<<counter<<"\n";
            for(int j=0; j<rules[i]->RHS.size(); j++){
                //std::vector<int>::iterator traverser=rules[i]->RHS.begin();
                //1)
                if(nonterminals.find(rules[i]->RHS[j]) != nonterminals.end() || terminals.find(rules[i]->RHS[j]) != terminals.end() ){
                    set <int> cpyset=Vector[rules[i]->RHS[j]];
                    set <int> lhsBeforeApplication= Vector[rules[i]->LHS];
                    if(cpyset.find(0)!=cpyset.end()){
                        cpyset.erase(cpyset.find(0));
                    }
                    
                    Vector[rules[i]->LHS].insert(cpyset.begin(),cpyset.end());
                    if(lhsBeforeApplication!= Vector[rules[i]->LHS]){
                        change=true;
                    }
                }
                
                vector<int>::iterator itRHS=rules[i]->RHS.begin()++;
                if(Vector[*itRHS].find(0) != Vector[*itRHS].end() ){
                while( *itRHS != rules[i]->RHS.back() &&Vector[*itRHS].find(0) != Vector[*itRHS].end()  ){
                    if(nonterminals.find(*itRHS) != nonterminals.end() || terminals.find(*itRHS) != terminals.end() ){
                        set <int> cpyset=Vector[*itRHS];
                        set <int> lhsBeforeApplication=Vector[*itRHS];
                        if(cpyset.find(0)!=cpyset.end()){
                            cpyset.erase(cpyset.find(0));
                        }
                        Vector[rules[i]->LHS].insert(cpyset.begin(),cpyset.end());
                        if(lhsBeforeApplication!= Vector[*itRHS]){
                            change=true;
                        }
                        
                        
                        
                    itRHS++;
                }
                }
                
                if(Vector[*itRHS].find(0) != Vector[*itRHS].end() ){
                     set <int> lhsBeforeApplication= Vector[rules[i]->LHS];
                    Vector[rules[i]->LHS].insert( * (Vector[*itRHS].find(0)) );
                    if(lhsBeforeApplication!= Vector[rules[i]->LHS]){
                        change=true;
                    }
                }
                    
                
                }
                
                
                else {
                    break;
                }
            }
            
        }
        printFirstSets();
        cout<<"\n";
        counter++;
    }
}



