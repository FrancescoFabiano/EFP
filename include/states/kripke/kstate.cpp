/* 
 * File:   kstate.cpp
 * Author: Francesco
 *
 * Created on April 9, 2019, 10:43 AM
 */

#include "kstate.h"
state kstate::compute_succ (const action &);

bool kstate::entails (fluent f)
{
  Literals::const_iterator it;
  it = lits.find (l);
  if (it == lits.end ())
    {
      return false;
    }
  return true;
}

bool kstate::entails (const belief_formula & bf);
