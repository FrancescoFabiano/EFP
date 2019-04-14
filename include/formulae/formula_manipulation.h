/* 
 * File:   formula_manipulation.h
 * Author: Francesco
 *
 * Created on April 7, 2019, 4:36 PM
 */

#pragma once

#include "belief_formula.h"

#include "../utilities/define.h"

class formula_manipulation{
    
private:
    static bool is_consistent(const fluent_set &, const fluent_set &);
    
    static fluent negate_fluent(const fluent);
    
public:

    static fluent_set and_ff(const fluent_set&, const fluent_set&);
    static fluent_formula and_ff(const fluent_formula&, const fluent_formula&);
};



