/* 
 * File:   action.h
 * Author: Francesco
 *
 * Created on April 3, 2019, 11:56 AM
 */
#pragma once

#include <set>
#include <vector>
#include <map>


#include "../utilities/define.h"
#include "../formulae/belief_formula.h"
#include "../formulae/proposition.h"
#include "../domain/grounder.h"


//Associate each agent to the observability conditions for each action
typedef std::map<agent, fluent_formula> observability_map;
//Associate each effect the condition
typedef std::map<fluent_formula, belief_formula> effects_map;

class action
{
private:
    std::string m_name; // action name
    action_id m_id;
    proposition_type m_type = NOTSET;
    
    //@BeliefFormula
    formula_list m_executability;
    observability_map m_fully_observants;
    observability_map m_partially_observants;
    
    /*********************************
     * If ontic then <@FluentFormula, conditions(@list<BeliefFormula>)>
     * If sensing then <@Fluent, conditions(@list<BeliefFormula>)>
     * If announcement <then @Literal, conditions(@list<BeliefFormula>)>
     *********************************/
    effects_map m_effects;

public:
    /* constructor & destructor */
    action();
    action(const std::string &, action_id);

    /* get/set functions */
    std::string get_name() const;
    void set_name(const std::string &);
    
    action_id get_id() const;
    void set_id(action_id);
    
    const proposition_type get_type() const;
    void set_type(proposition_type);

    //@TODO: Check if is the best type of parameters ret. (Also the ones below)

    //The return type it's fine because actions survive with the domain
    const formula_list& get_executability() const;
    //The return type it's fine because actions survive with the domain
    const effects_map& get_effects() const;
    //The return type it's fine because actions survive with the domain
    const observability_map& get_fully_observants() const;
    //The return type it's fine because actions survive with the domain
    const observability_map& get_partially_observants() const;

    //Ok because is push_back and it makes copy
    void add_executability(const belief_formula &);
    
    //Ok because is map::value_type and it makes copy
    void add_effect(const fluent_formula&, const belief_formula&);
    //Ok because is map::value_type and it makes copy
    void add_fully_observant(agent, const fluent_formula&);
    //Ok because is map::value_type and it makes copy
    void add_partially_observant(agent, const fluent_formula&);
    
    //Ok because its method all makes copies
    void add_proposition(const proposition &, const grounder&);

    void print(const grounder&) const;
    
    //For set insertion
    bool operator<(const action&) const;
    bool operator=(const action&);

};

typedef std::set<action> action_set;
typedef std::vector<action> action_list;

typedef std::map<std::string, action_id> action_name_map;