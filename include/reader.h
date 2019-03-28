#ifndef READER_H
#define READER_H

#include "define.h"

using namespace std;

class Reader {
public:
    /* domain name */
    string m_domain_name;

    /* reading from the input file */
    StringList m_fluents;
    StringList m_actions;
    StringList sensing_actions; //Ben
    StringList ontic_actions; //Ben
    StringList ann_actions; //Ben
    StringList m_agents; //Me: set of agents
    PropositionList m_propositions;
    StringList2 m_init;
    StringList m_goal; // set of literals in the goal
    Nodes m_gd; // goal description
    Nodes k_init;
public:
    string name(Literal x);
    StringList name(Literals* x);
    int read();
    void print() const;
};

#endif
