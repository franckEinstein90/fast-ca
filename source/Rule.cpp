#include "Rule.h"


Rule::Rule(void (*RuleBuilder) (vector<RULEELTTYPE>&)):
m_rule_size(RULE2DSIZE){
	while(m_rule_vect.size() < m_rule_size)
		m_rule_vect.push_back((RULEELTTYPE)0);		

    if(RuleBuilder)
         (*RuleBuilder)(m_rule_vect);
         while(m_rule_vect.size() < m_rule_size)
                m_rule_vect.push_back((RULEELTTYPE)0);
}
Rule::Rule(const Rule& r):
m_rule_size(r.m_rule_size), m_rule_vect(r.m_rule_vect){
}
Rule::~Rule(void)
{
}
