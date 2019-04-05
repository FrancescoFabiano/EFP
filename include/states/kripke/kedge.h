/* 
 * File:   kedge.h
 * Author: Francesco
 *
 * Created on March 31, 2019, 1:25 PM
 */

#pragma once

#include "kworld.h"

typedef unsigned short kedge_id;
typedef std::vector<kedge_id> kedge_id_list;

class kedge
{
private:
    kworld_id m_from;
    kworld_id m_to;
    agent m_label;
    kedge_id m_edge_id;
    bool m_exists;

public:
    kedge();
    kedge(kedge*);

    kworld_id get_from();
    kworld_id get_to();
    agent get_label();
    kedge_id get_id();
    bool get_exist();
    
    void set_from(kworld_id);
    void set_to(kworld_id);
    void set_label(agent);
    void set_id(kedge_id);
    void set_exists(bool);

    void print();
};