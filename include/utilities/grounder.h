/* 
 * File:   grounder.h
 * Author: Francesco
 *
 * Created on April 5, 2019, 12:13 PM
 */

#pragma once

#include "define.h"
#include <string>


//@TODO: Double definition watch out
typedef unsigned short action_id;
typedef std::map<std::string, action_id> action_name_map;

typedef std::map<fluent, std::string> reverse_fluent_map;
typedef std::map<agent, std::string> reverse_agent_map;
typedef std::map<action_id, std::string> reverse_action_name_map;


class grounder
{
private:
    //@TODO: The only copy of the maps is here, deleted from domain
    fluent_map m_fluent_map;
    agent_map m_agent_map;
    action_name_map m_action_name_map;
    
    //@TODO: Creation reverse map for printing, maybe is better to use bimap? We just use for print so Maybe not
    reverse_fluent_map r_fluent_map;
    reverse_agent_map r_agent_map;
    reverse_action_name_map r_action_name_map;
    
    bool m_reversed_fl = false;
    bool m_reversed_ag = false;
    bool m_reversed_ac = false;
    
    void reverse();
    
    void create_reverse_fl(const fluent_map&);
    void create_reverse_ag(const agent_map&);
    void create_reverse_ac(const action_name_map&);    
public:

    grounder();
    grounder(fluent_map, agent_map, action_name_map);

    void set_fluent_map(fluent_map);
    void set_agent_map(agent_map);
    void set_action_name_map(action_name_map);


    //@TODO: Check if is the best type of parameters pass. and return
    fluent ground_fluent(const std::string&) const;
    fluent_list ground_fluent(const string_list&) const;
    fluent_formula ground_fluent(const string_list_list&) const;

    agent ground_agent(const std::string&) const;

    action_id ground_action(const std::string&) const;
        
    
    std::string deground_fluent(fluent) const;
    string_list deground_fluent(const fluent_list&) const;
    string_list_list deground_fluent(const fluent_formula&) const;
   
    std::string deground_agent(agent) const;
    agent_set ground_agent(const string_list&) const;

  
    std::string deground_action(action_id) const;
    
    void print_ff(const fluent_list&) const;            
    void print_ff(const fluent_formula&) const;        
};
