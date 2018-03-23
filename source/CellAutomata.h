#pragma once

// CellAutomata.h: interface for the CellAutomata class.
//
//////////////////////////////////////////////////////////////////////

 
#include "CASlice.h" 





class CellAutomata  {
public:
	static const int defaultGridWidth = 100;
	static const int defaultGridHeight = 80;

protected: 
        int   ListNum;
        string   m_cellAutomataId;
        long int m_evolutionNumber;


        //Circular Stack
        vector<Slice *> sliceArray;
        Slice* initSlice;
        Rule* rule;
        
        void drawSlice(Slice*, float=(float)defaultGridWidth, float=(float)defaultGridHeight);
        void destroyCA();
        void insertSlice(Slice*);
        
        int index_Of_Current_Slice;
        int index_Of_Rewind_Finger;
      
    
        int rows;
        int columns;
        int height;
        
    public:
        CellAutomata(){};
		CellAutomata(int E_rows, int E_columns, int E_height,
                     Rule* R = (Rule*)0,
                     Slice* E_initSlice = (Slice*)0,
                     string E_name = (string)"");

        
        Slice* getSliceOnRewindIndex(){return this->sliceArray[this->index_Of_Rewind_Finger];};
        inline string  getName() const {return m_cellAutomataId;};
        
        void setRule(const Rule& NewRule);
        void setInitialConfiguration(const Slice& NewSlice);
        int evolve();
        int evolveTo(int EvolNum);
        void forward();
        void backward();
        virtual ~CellAutomata();
};


