#pragma once

#include <string>
#include <vector>
using namespace std;

#define RULE2DSIZE 512
#define RULEELTTYPE unsigned short

class Rule {

protected:
        unsigned short m_rule_size;
        vector <RULEELTTYPE> m_rule_vect;

protected:
        inline bool checkRange(const unsigned short i) const{
            return ((i >= 0)&&( i < m_rule_size));
        };

public:
        Rule(void (*) (vector<RULEELTTYPE>&) = NULL);
        Rule(const Rule&);
        
        inline bool subRule(const unsigned short ind) const{
            if(!checkRange(ind)) throw "Out of Range";  
            return (bool) m_rule_vect[ind];
        };

        inline void setSubRule(const unsigned short ind){
            if(!checkRange(ind)) throw "Out of Range";  
            m_rule_vect[ind] = (RULEELTTYPE)1;
        };

        inline void unsetSubRule(int ind){
            if(!checkRange(ind)) throw "Out of Range";  
            m_rule_vect[ind] = (RULEELTTYPE)0;
        };
		inline int getRuleSize() const {return m_rule_size;};
        virtual ~Rule();
};

