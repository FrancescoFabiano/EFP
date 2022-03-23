/**
 * \class cem_parser
 * \brief Singleton class that parse the file that described the custom cems.
 * 
 * 
 * @see cevent, cem, cem_store.
 * 
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date April 28, 2021
 */


#pragma once
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
//#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include <regex>
#include "../utilities/define.h"

class cem_parser
{
private:
    static void apply_spaces_regex(std::string & to_clean, const std::regex & pattern);

    static void parse_conditions(const std::string & line, event_metacond & cond, int line_count);

    static void parse_ag_list(const std::string & line);

    static void parse_edge(const std::string & edge, cem_edges & edges);

    static void parse_edges_list(const std::string & line, cem_edges & edges);

public:

    static void parse(const std::string & filename);

};

