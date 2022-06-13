#include "finitary_theory.h"
//#include "box_whether_formula.ipp"

#include <utility>


finitary_theory::finitary_theory() = default;


finitary_theory::finitary_theory(fluent_ptr_set pointed_fluents, fluent_ptr_set ck_fluents,
                                    std::set<const formula*>  ck_formulae, std::set<const box_formula<logic>*> knows,
                                    std::set<const box_whether_formula<logic>*> knows_whether) :
        m_pointed_fluents(std::move(pointed_fluents)),
        m_ck_fluents(std::move(ck_fluents)),
        m_ck_formulae(std::move(ck_formulae)),
        m_knows(std::move(knows)),
        m_knows_whether(std::move(knows_whether)) {}


const fluent_ptr_set &finitary_theory::get_pointed_fluents() const {
    return m_pointed_fluents;
}


const fluent_ptr_set &finitary_theory::get_ck_fluents() const {
    return m_ck_fluents;
}


const std::set<const formula*> &finitary_theory::get_ck_formulae() const {
    return m_ck_formulae;
}


const std::set<const box_formula<logic>*> &finitary_theory::get_knows() const {
    return m_knows;
}


const std::set<const box_whether_formula<logic>*> &finitary_theory::get_knows_whether() const {
    return m_knows_whether;
}
