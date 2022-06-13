#ifndef EFP_FORMULA_H
#define EFP_FORMULA_H

#include "../../include/definitions/domain_def.h"

class kstate;
class kworld;
class pstate;
class pworld;

//template<class S, class W>
class formula {
protected:
    unsigned long m_modal_depth;

public:
    formula();

    virtual bool is_entailed(const fluent_ptr_set *fluent_set) const = 0;
    virtual bool is_entailed(const kstate *state, const kworld *world) const = 0;
    virtual bool is_entailed(const pstate *state, const pworld *world) const = 0;

//    virtual bool is_entailed(const S* state, const W *world) const = 0;

    virtual bool is_propositional() const;

    unsigned long get_modal_depth() const;

    virtual bool operator==(const formula &to_compare) const = 0;
    virtual bool operator<(const formula &to_compare) const = 0;
};


#endif //EFP_FORMULA_H
