#include "finitary_theory.h"

template<class M>
finitary_theory<M>::finitary_theory(std::list<const literal*> &pointed_literals, std::list<const literal*> &ck_literals,
                                    std::list<const formula*> ck_formulae, std::list<const box_formula<M>*> &knows,
                                    std::list<const box_whether_formula<M>*> &knows_whether) :
        m_pointed_literals(std::move(pointed_literals)),
        m_ck_literals(std::move(ck_literals)),
        m_ck_formulae(std::move(ck_formulae)),
        m_knows(std::move(knows)),
        m_knows_whether(std::move(knows_whether)) {}

template<class M>
const std::list<const literal*> &finitary_theory<M>::get_pointed_literals() const {
    return m_pointed_literals;
}

template<class M>
const std::list<const literal*> &finitary_theory<M>::get_ck_literals() const {
    return m_ck_literals;
}

template<class M>
const std::list<const formula*> &finitary_theory<M>::get_ck_formulae() const {
    return m_ck_formulae;
}

template<class M>
const std::list<const box_formula<M>*> &finitary_theory<M>::get_knows() const {
    return m_knows;
}

template<class M>
const std::list<const box_whether_formula<M>*> &finitary_theory<M>::get_knows_whether() const {
    return m_knows_whether;
}
