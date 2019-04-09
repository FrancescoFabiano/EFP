/* 
 * File:   kedge.h
 * Author: Francesco
 *
 * Created on March 31, 2019, 1:25 PM
 */

#pragma once

#include <string>
#include <vector>
#include <memory>


#include "kworld.h"
#include "kstore.h"

typedef std::string kedge_id;
typedef std::shared_ptr<kedge> kedge_ptr;
//@TODO: is maybe better set? Duplicate? if switch add < for pointer
typedef std::vector<kedge_ptr> kedge_ptr_list;

class kedge
{
private:
    kworld_ptr m_from;
    kworld_ptr m_to;
    agent m_label;
    kedge_id m_edge_id;

    kedge_id hash_info_into_id(kworld_ptr from, kworld_ptr to, agent label);
    kedge_id hash_info_into_id(); //generate an unique id given the state information -> the literal
    
    void set_from(const kworld &);
    void set_to(const kworld &);
    void set_label(agent);
    void set_id();

public:
    kedge();
    //@TODO:Also with pointers?
    kedge(const kworld & from, const kworld & to, agent label);
    
    kworld_ptr get_from();
    kworld_ptr get_to();
    agent get_label();
    kedge_id get_id();
    
    //For set
    bool operator<(const kedge&) const;
    //Not needed because @((a < b) && (b < a))
    //bool operator==(const kedge&) const;


    void print();
};