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
#include <memory>


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

typedef unsigned short action_id;
typedef std::vector<action_id> action_id_list;

typedef std::map<std::string, fluent> fluent_map;
typedef std::map<std::string, agent> agent_map;


enum domain_restriction
{
    S5, //Knowledge, S5 finitary
    K45, //Belief
    NONE, //sensing action (peek_a)
    NONEG, //Goal
    FAIL,
};

enum state_type
{
    KRIPKE,
    POSSIBILITIES,
    OBDD
};
/*****************Kripke States*****************/
class kedge;
typedef std::string kedge_id;
typedef std::shared_ptr<const kedge> kedge_ptr;
//@TODO: is maybe better set? Duplicate? if switch add < for pointer
typedef std::set<kedge_ptr> kedge_ptr_set;

class kworld;
//Use a unique key that identifies the world??? The hash of the literal list
typedef std::string kworld_id;
typedef std::shared_ptr<const kworld> kworld_ptr;
//@TODO: is maybe better set? Duplicate? if switch add < for pointer
typedef std::set<kworld_ptr> kworld_ptr_set;