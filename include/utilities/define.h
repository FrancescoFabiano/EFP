/* 
 * File:   define.h
 * Author: Francesco
 *
 * Created on March 31, 2019, 6:44 PM
 */

#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <map>

/*****************************************************************
 * Constants
 ****************************************************************/
#define NEGATION_SYMBOL "-"

/****************************************************************
 * Types definition to simplify - Not class specific
 ****************************************************************/
typedef std::set<std::string> string_list;
typedef std::set<string_list> string_list_list;

//Plan related
//typedef unsigned short literal;
//typedef std::set<literal> literal_set;
typedef unsigned short fluent;
typedef std::set<fluent> fluent_list;
typedef std::set<fluent_list> fluent_formula;//To represent the fluent formula in DNF format

typedef unsigned short agent;
typedef std::vector<agent> agent_list;
typedef std::set<agent> agent_set;

typedef std::map<std::string, fluent> fluent_map;
typedef std::map<std::string, agent> agent_map;