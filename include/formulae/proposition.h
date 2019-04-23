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

#include "belief_formula.h"
#include "../utilities/define.h"
#include "../utilities/printer.h"

/**
 * \brief The possible types of proposition founded in the domain description.
 */
enum proposition_type
{
    EXECUTABILITY, /**< \brief Used when the proposition specifies an action executability condition -- *act* **exec if** *phi* */
    DYNAMIC, /**< \brief Used when the proposition specifies the effects of an ontic action -- *act* **causes** *f* 
              *
              * \todo change with ONTIC.
              */
    DETERMINATION, /**< \brief Used when the proposition specifies the effects of a sensing action -- *act* **sensed** *f* 
                    *
                    * \todo change with SENSING.
                    */
    ANNOUNCEMENT, /**< \brief Used when the proposition specifies the effects of a announcement action -- *act* **annaunces** *ff* */
    OBSERVANCE, /**< \brief Used when the proposition specifies the full observability conditions of an action -- *ag* **observes** *act* */
    AWARENESS, /**< \brief Used when the proposition specifies the partial observability conditions of an action -- *ag* **aware** *act* */
    NOTSET, /**< \brief Default case.*/
};

class proposition
{
public:
    /**
     * The \ref proposition_type of this.
     */
    proposition_type m_type;
    /**
     * The name of this (serves as id).
     */
    std::string m_action_name;

    /*
     Ontic: string_set -> fluent
     Sensing: string_set -> fluent
     Announcement: string_set_set -> fluent formula
     */

    /**
     * If *this* is \ref DYNAMIC, \ref DETERMINATION or \ref ANNOUNCEMENT the effects are stored here.
     */
    string_set_set m_action_effect;

    /**
     * If *this* is \ref OBSERVANCE or \ref AWARENESS the relative \ref agent is stored here.
     */
    std::string m_agent;

    /**
     * If *this* is \ref OBSERVANCE or \ref AWARENESS the \ref fluent_formula condition for the observability is stored here (not grounded).
     */
    string_set_set m_observability_conditions;

    /**
     * If *this* is \ref DYNAMIC, \ref DETERMINATION or \ref ANNOUNCEMENT the \ref belief_formula condition for the act is stored here.
     */
    belief_formula m_executability_conditions;

    //const belief_formula* get_executability_conditions() const;
    //const string_set* get_effect() const;

    /**
     * \brief Function that print *this*.*/
    void print() const;
};

typedef std::list<proposition> proposition_list; /**< \brief A list of proposition to simplify the usage.*/