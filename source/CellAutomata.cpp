#include "CellAutomata.h"

// CellAutomata.cpp: implementation of the CellAutomata class.
//
//////////////////////////////////////////////////////////////////////

void CellAutomata::destroyCA(){

    for(int i=0; i<sliceArray.size();i++)
        if(sliceArray[i]) delete sliceArray[i];
        
    sliceArray.clear();
    initSlice = (Slice*)0;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CellAutomata::CellAutomata
    (int E_rows, int E_columns, int E_height, 
    Rule* R,
    Slice* N, 
    string E_name):
    initSlice((Slice*)0),
    rule((Rule*)0),
    rows(E_rows), columns(E_columns), height(E_height),
    index_Of_Rewind_Finger(-1),index_Of_Current_Slice(-1),
    m_evolutionNumber(0),
    m_cellAutomataId(E_name){

    if(height<1) throw "Illegal Stack Capacity Creation Size";
    if(N) setInitialConfiguration(*N);
    if(R) rule = new Rule(*R);
}
    
void CellAutomata::setRule(const Rule& NS){
        if(rule) delete rule;
        rule = new Rule(NS);
}
    
void CellAutomata::setInitialConfiguration(const Slice& NS){
    destroyCA();
    initSlice = new Slice(NS);
    sliceArray.push_back(initSlice);
    index_Of_Rewind_Finger = 0;
    index_Of_Current_Slice = 0;
    m_evolutionNumber = 0;
}
void CellAutomata::backward(){

    this->index_Of_Rewind_Finger = 
    (this->index_Of_Rewind_Finger)? this->index_Of_Rewind_Finger-1:this->index_Of_Current_Slice;
    
}
void CellAutomata::forward(){
    this->index_Of_Rewind_Finger = 
        (this->index_Of_Rewind_Finger+1)%(this->index_Of_Current_Slice+1);
}
int CellAutomata::evolve(){
     if(rule)
        if(initSlice){
            Slice* Temporay_Slice = 
                                sliceArray[this->index_Of_Current_Slice]->evolve(*rule);
            
            this->index_Of_Rewind_Finger = 
            this->index_Of_Current_Slice =
            ((this->index_Of_Current_Slice + 1) % height);
            
            if(sliceArray.size()<height)
                sliceArray.push_back(Temporay_Slice);
            else{    
                if(sliceArray[this->index_Of_Current_Slice]) 
                    delete sliceArray[this->index_Of_Current_Slice];
                sliceArray[this->index_Of_Current_Slice] =  Temporay_Slice;
            }
                        return m_evolutionNumber++;
    }
        return 0;
}

int CellAutomata::evolveTo(int EvolNum){
    while(m_evolutionNumber < EvolNum)
        evolve();
        return m_evolutionNumber;
}
CellAutomata::~CellAutomata(){
    destroyCA();
    if(rule){ delete rule; rule = (Rule*)0; }
}

