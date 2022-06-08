#ifndef EFP_FINITARY_THEORY_H
#define EFP_FINITARY_THEORY_H


#include <utility>

#include "../formula.h"
#include "../propositional/literal.h"
#include "../propositional/or_formula.h"
#include "../modal/box_formula.h"
#include "box_whether_formula.h"

template<class M>
class finitary_theory {
private:
    std::list<const literal*> m_pointed_literals;

    std::list<const literal*> m_ck_literals;
    std::list<const formula*> m_ck_formulae;

    std::list<const box_formula<M>*> m_knows;
    std::list<const box_whether_formula<M>*> m_knows_whether;

public:
    finitary_theory(std::list<const literal*> &pointed_literals, std::list<const literal*> &ck_literals,
                    std::list<const formula*> ck_formulae, std::list<const box_formula<M>*> &knows,
                    std::list<const box_whether_formula<M>*> &knows_whether);

    const std::list<const literal*>& get_pointed_literals() const;

    const std::list<const literal*>& get_ck_literals() const;

    const std::list<const formula*>& get_ck_formulae() const;

    const std::list<const box_formula<M>*>& get_knows() const;

    const std::list<const box_whether_formula<M>*> &get_knows_whether() const;
};


#endif //EFP_FINITARY_THEORY_H
