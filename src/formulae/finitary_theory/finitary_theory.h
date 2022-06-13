#ifndef EFP_FINITARY_THEORY_H
#define EFP_FINITARY_THEORY_H


#include <utility>

#include "../formula.h"
#include "../propositional/or_formula.h"
#include "../modal/box_formula.h"
#include "box_whether_formula.ipp"

class finitary_theory {
private:
    const fluent_ptr_set m_pointed_fluents;

    const fluent_ptr_set m_ck_fluents;
    const std::set<const formula*> m_ck_formulae;

    const std::set<const box_formula<logic>*> m_knows;
    const std::set<const box_whether_formula<logic>*> m_knows_whether;

public:
    finitary_theory(fluent_ptr_set pointed_fluents, fluent_ptr_set ck_fluents,
                    std::set<const formula*>  ck_formulae, std::set<const box_formula<logic>*> knows,
                    std::set<const box_whether_formula<logic>*> knows_whether);

    finitary_theory();

    const fluent_ptr_set &get_pointed_fluents() const;

    const fluent_ptr_set &get_ck_fluents() const;

    const std::set<const formula*>& get_ck_formulae() const;

    const std::set<const box_formula<logic>*>& get_knows() const;

    const std::set<const box_whether_formula<logic>*> &get_knows_whether() const;
};


#endif //EFP_FINITARY_THEORY_H
