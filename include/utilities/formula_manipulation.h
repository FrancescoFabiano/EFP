/* 
 * File:   formula_manipulation.h
 * Author: Francesco
 *
 * Created on April 7, 2019, 4:36 PM
 */

#pragma once

#include "define.h"
#include "belief_formula.h"

class formula_manipulation{
    
private:
    static bool is_consistent(const fluent_list &, const fluent_list &);
    
    static fluent negate_fluent(const fluent);
    
public:

    static fluent_list and_ff(const fluent_list&, const fluent_list&);
    static fluent_formula and_ff(const fluent_formula&, const fluent_formula&);
};



