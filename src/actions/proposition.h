/**
 * \class proposition
 * \brief Class used a support to elaborate the proposition of the \ref action(s) in the domain.
 *
 * A \ref proposition identifies and specifies an \ref action behavior(executability, effects, observability).
 * For details on the types of proposition look \ref proposition_type.
 * 
 * The content of a \ref proposition is not grounded as will be grounded once is inserted in an \ref action.
 *
 * \see action.
 * 
 * \todo public fields into private and getter and setter.
 * 
 * \todo move into the action package.
 * 
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 1, 2019
 */

#pragma once

#include <list>
#include <boost/lexical_cast.hpp>

#include "../../include/definitions/actions.h"
#include "../formulae/belief_formula.h"
#include "../utilities/printer.h"

/**
 * \brief The possible types of proposition founded in the domain description.
 */
enum proposition_type
{
    EXECUTABILITY, /**< \brief Used when the proposition specifies an action executability condition -- *act* **exec if** *phi* */
    EFFECTS, /**< \brief Used when the proposition specifies the effects of an action -- *act* **has_effects** *f*.*/
    OBSERVABILITY, /**< \brief Used when the proposition specifies the observability group of an agent wrt to an action -- *agent* **in_group group** *of act*.*/
    TYPE, /**< \brief Used when the proposition specifies the type of an action -- *act* **has_type** *type*.*/
    /*******FOR MAL OPTIMIZATION******/
    MAL_EFF /**< \brief Used to identify the mAL action definition (causes, determines, announces).*/
};

class proposition
{
private:
    /** The \ref proposition_type of this.*/
    proposition_type m_type;
    /** The name of this (used as id).*/
    std::string m_action_name;
    /** If *this* is \ref TYPE the action type is stored here.*/
    act_type m_act_type;
    /** If *this* is \ref EFFECTS the effects are stored here.*/
    string_set_set m_action_effect;
    /** If *this* is \ref OBSERVABILITY the relative \ref agent is stored here.*/
    std::string m_agent;
    /** If *this* is \ref OBSERVABILITY the relative \ref agent_group_id.*/
    agent_group_id m_agent_group;
    /** \brief If *this* is \ref OBSERVABILITY or \ref EFFECTS or \ref EXECUTABILITY the \ref fluent_formula condition for the observability is stored here (not grounded).*/
    belief_formula m_conditions;


public:

    /** Getter of the field \ref m_type.
     * 
     * @return the value of \ref m_type.*/
    proposition_type get_type() const;
    /** Getter of the field \ref m_action_name.
     * 
     * @return the value of \ref m_action_name.*/
    const std::string & get_action_name() const;

    /** Getter of the field \ref m_act_type.
     * 
     * @return the value of \ref m_action_name.*/
    act_type get_action_type() const;
    /** Getter of the field \ref m_action_effect grounded.
     * 
     * @return the grounded value of \ref m_action_effect.*/
    fluent_formula get_action_effect() const;
    /** Getter of the field \ref m_agent grounded.
     * 
     * @return the grounded value of \ref m_agent.*/
    agent get_agent() const;

    /** Getter of the field \ref m_agent_group.
     * 
     * @return the value of \ref m_agent_group.*/
    agent_group_id get_agent_group() const;

    /** Getter of the field \ref m_conditions.
     * 
     * @return the grounded value of \ref m_conditions.*/
    const belief_formula & get_conditions() const;

    /** Getter of the field \ref m_conditions grounded.
     * 
     * @return the grounded value of \ref m_conditions.*/
    const belief_formula & get_grounded_conditions();


    /** Setter for the field \ref m_type.
     * 
     * @param[in] to_set: the value to assign to \ref m_type.*/
    void set_type(proposition_type to_set);
    /** Setter for the field \ref m_action_name.
     * 
     * @param[in] to_set: the value to assign to \ref m_action_name.*/
    void set_action_name(const std::string & to_set);

    /** Setter for the field \ref m_act_type.
     * 
     * @param[in] to_set: the value to assign to \ref m_act_type.*/
    void set_action_type(const std::string & to_set);
    /** Setter for the field \ref m_action_effect.
     * 
     * @param[in] to_set: the value to assign to \ref m_action_effect.*/
    void set_action_effect(const string_set_set & to_set);
    /** Function that adds an action effect to \ref m_action_effect.
     * 
     * @param[in] to_add: the value to add to \ref m_action_effect.*/
    void add_action_effect(const string_set & to_add);

    /** Setter for the field \ref m_agent.
     * 
     * @param[in] to_set: the value to assign to \ref m_agent.*/
    void set_agent(const std::string & to_set);

    /** Setter for the field \ref m_agent_group.
     * 
     * @param[in] to_set: the value (retrived by the map in cem store) to assign to \ref m_agent_group.*/
    void set_agent_group(const std::string & to_set);

    /** Setter for the field \ref m_conditions.
     * 
     * @param[in] to_set: the value to assign to \ref m_conditions.*/
    void set_conditions(const belief_formula & to_set);


    /** \brief Function that print *this*.*/
    void print() const;
};

typedef std::list<proposition> proposition_list; /**< \brief A list of proposition to simplify the usage.*/