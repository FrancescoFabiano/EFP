/* 
 * File:   proposition.h
 * Author: Francesco
 *
 * Created on March 31, 2019, 6:55 PM
 */

#pragma once

#include <list>

#include "belief_formula.h"
#include "../utilities/define.h"
#include "../utilities/printer.h"

enum proposition_type
{
    EXECUTABILITY,
    DYNAMIC, //ontic action (open_a)
    DETERMINATION, //sensing action (peek_a)
    ANNOUNCEMENT, //announcement action (shout_tail_a)
    OBSERVANCE,
    AWARENESS,
    NOTSET, //Special case
};

class proposition
{
public:
    proposition_type m_type;
    std::string m_action_name;

    /*
     Ontic: string_list -> fluent
     Sensing: string_list -> fluent
     Announcement: string_list_list -> fluent formula
     */
    string_set_set m_action_effect;
    std::string m_agent;
    //string fluentPro;
    string_set_set m_observability_conditions;
    belief_formula m_executability_conditions;

    //const belief_formula* get_executability_conditions() const;
    //const string_list* get_effect() const;

    void print() const;
};

typedef std::list<proposition> proposition_list;