/*
 * File:   action.cpp
 * Author: Francesco
 *
 * Created on April 4, 2019, 2:58 PM
 */
#include "action.h"

#include <utility>
#include "custom_event_models/cem_store.h"

/*********************************************************************
 Action implementation
 **********************************************************************/

action::action() = default;

//action::action(const fluent_set& fluents, const agent_set& agents) {
//    m_type = -1;
//    initialize_obs_table(fluents, agents);
//}

action::action(const std::string& name, const action_id& id, const fluent_set& fluents, const agent_set& agents) {
    set_name(name);
    set_id(id);
    m_type = -1;
    initialize_obs_table(fluents, agents);
}

void action::initialize_obs_table(const fluent_set& fluents, const agent_set& agents) {
    agent_set::const_iterator it_ag;

    belief_formula false_bf;
    fluent_formula false_ff;

    fluent f1 = *(fluents.begin());
    fluent f1_negated = helper::negate_fluent(f1);

    //The formula is "fluent_number_1 and -fluent_number_1" which is always false
    fluent_set true_fs;

    true_fs.insert(f1);
    true_fs.insert(f1_negated);

    false_ff.insert(true_fs);

    false_bf.set_formula_type(FLUENT_FORMULA);
    false_bf.set_fluent_formula(false_ff);
    false_bf.set_is_grounded(true);
    false_bf.deground();


    //std::map<agent, std::map<agent_group_id, belief_formula>> map_mid;
    std::map<agent_group_id, belief_formula> map_internal;

    for (it_ag = agents.begin(); it_ag != agents.end(); ++it_ag) {
        for (short it_ag_group = 0; it_ag_group != cem_store::get_instance().get_agent_group_number(); ++it_ag_group) {
            //Everyone set to false, then in the function that retrieves set the rules that if everything is false you get the last
            map_internal.insert(std::make_pair(it_ag_group, false_bf));
        }

        m_observants.insert(std::make_pair(*it_ag, map_internal));
        map_internal.clear();
    }
}

std::string action::get_name() const {
    return m_name;
}

void action::set_name(const std::string &name) {
    m_name = name;
}

action_id action::get_id() const {
    return m_id;
}

void action::set_id(action_id id) {
    m_id = id;
}

act_type action::get_type() const {
    return m_type;
}

void action::set_type(act_type type) {
    if (type != -1) {
        if (m_type == -1) {
            if (cem_store::get_instance().exist_cem(type)) {
                m_type = type;
                return;
            } else {
                std::cerr << "The specified action type: " << type << " does not match any cem specification."
                          << std::endl;
                exit(1);
            }
        }
    }
}

const formula_list &action::get_executability() const {
    return m_executability;
}

const effects_map &action::get_effects() const {
    return m_effects;
}

const observability_map &action::get_observants() const {
    return m_observants;
}

void action::add_executability(const belief_formula &exec) {
    ///Parameter Passing ok because push_back makes copy.
    m_executability.push_back(exec);
}

void action::add_effect(const fluent_formula &effect, const belief_formula &condition) {
    ///Parameter Passing ok because is map::value_type and it makes copy

    m_effects.insert(effects_map::value_type(effect, condition));
}

void action::add_observant(agent ag, agent_group_id ag_group, const belief_formula &condition) {
    //std::pair<agent_group_id, belief_formula> tmp_obs(ag_group, condition);

    m_observants[ag][ag_group] = condition; //.insert(observability_map::value_type(fully, condition));
}

void action::add_proposition(const grounder& grounder, proposition &prop) {
    ///Parameter Passing ok because its methods all make copies

    switch (prop.get_type()) {

        //Add action to the the list (name as identifier, then set id) then update the conditions and the awareness of the action so it's complete)
        case EFFECTS:
            add_effect(prop.get_action_effect(grounder), prop.get_grounded_conditions(grounder));
            break;

        case OBSERVABILITY:
            add_observant(prop.get_agent(grounder), prop.get_agent_group(), prop.get_grounded_conditions(grounder));
            break;

        case EXECUTABILITY:
            //@TODO:What if there is more than one? Then CNF or DNF
            add_executability(prop.get_grounded_conditions(grounder));
            break;
        case TYPE:
            //@TODO:What if there is more than one? Then CNF or DNF
            set_type(prop.get_action_type());
            break;
            /*******FOR MAL OPTIMIZATION******/
        case MAL_EFF:
            add_effect(prop.get_action_effect(grounder), prop.get_grounded_conditions(grounder));
            set_type(prop.get_action_type());
            break;
        default:
            break;
    }
}

bool action::operator<(const action &act) const {
    return m_id < act.get_id();
}

bool action::operator=(const action &act) {
    set_name(act.get_name());
    set_id(act.get_id());
    m_type = act.get_type();

    m_executability = act.get_executability();
    m_observants = act.get_observants();
    m_effects = act.get_effects();

    return true;
}

void action::print() const {
//    grounder grounder = domain::get_instance().get_grounder();
//    std::cout << "\nAction " << get_name() << ":" << std::endl;
//    std::cout << "	ID: " << get_id() << ":" << std::endl;
//    std::cout << "	Type: " << cem_store::get_instance().get_cem_name(get_type()) << std::endl;
//
//    std::cout << "	Executability:";
//    formula_list::const_iterator it_fl;
//    for (it_fl = m_executability.begin(); it_fl != m_executability.end(); ++it_fl) {
//        std::cout << " | ";
//        it_fl->print();
//        //std::cout << std::endl;
//    }
//
//    std::cout << "\n	Effects:";
//    effects_map::const_iterator it_effmap;
//    for (it_effmap = m_effects.begin(); it_effmap != m_effects.end(); ++it_effmap) {
//        std::cout << " | ";
//        printer::get_instance().print_list(it_effmap->first);
//        std::cout << " if ";
//        it_effmap->second.print();
//    }
//
//
//    std::cout << "\n	Observants:";
//    for (auto it_obsmap = m_observants.begin(); it_obsmap != m_observants.end(); ++it_obsmap) {
//        auto internal_map = it_obsmap->second;
//        auto ag_string = grounder.deground_agent(it_obsmap->first);
//        for (auto internal_it = internal_map.begin(); internal_it != internal_map.end(); ++internal_it) {
//            std::cout << " | " << ag_string << " belongs to "
//                      << cem_store::get_instance().get_agent_group_name(internal_it->first) << " if ";
//            //printer::get_instance().print_list(it_obsmap->second);
//            belief_formula cond_temp = internal_it->second;
//            if (cond_temp.get_formula_type() == FLUENT_FORMULA) {
//                auto ff_temp = cond_temp.get_fluent_formula();
//                auto init_fluent = domain::get_instance().get_fluents().begin();
//                auto neg_init_fluent = helper::negate_fluent(*init_fluent);
//                if (ff_temp.begin()->find(*init_fluent) != ff_temp.begin()->end() &&
//                    ff_temp.begin()->find(neg_init_fluent) != ff_temp.begin()->end()) {
//                    std::cout << "False";
//                } else {
//                    internal_it->second.print();
//                }
//            } else {
//                internal_it->second.print();
//            }
//        }
//    }
//    std::cout << std::endl;
}


/*******FOR MAL OPTIMIZATION******/

/*@TODO: Optimize when create a new class and compute just once and the return (with const &) the calculated set**/
void action::get_mal_observants(mal_observability_map &ret, agent_group_id id) const {
    for (auto it_obs = m_observants.begin(); it_obs != m_observants.end(); ++it_obs) {
        auto internal_map = it_obs->second;
        for (auto internal_it = internal_map.begin(); internal_it != internal_map.end(); ++internal_it) {
            if (internal_it->first == id) { //FULLY OBSERVANT
                ret.insert(mal_observability_map::value_type(it_obs->first, internal_it->second));
            }
        }
    }
}

mal_observability_map action::get_fully_observants() const {
    mal_observability_map ret;

    get_mal_observants(ret, 0); //0 is FULLY as the insertion order in the reader.cpp

    return ret;
}

/**\brief The partially_observant frame (each obsv is related to a condition that's why  map) of *this*.*/
mal_observability_map action::get_partially_observants() const {
    mal_observability_map ret;

    get_mal_observants(ret, 1); // is PARTIALLY as the insertion order in the reader.cpp

    return ret;
}
