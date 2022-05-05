#ifndef EFP_FORMULA_H
#define EFP_FORMULA_H


#include "../states/kripke/kstate.h"
#include "../states/possibilities/pstate.h"

class formula {
protected:
    unsigned long m_modal_depth;

public:
    virtual bool is_entailed(const kstate &state, const kworld_ptr &world) const;
    virtual bool is_entailed(const pstate &state, const pworld_ptr &world) const;

    virtual bool is_propositional() const;

    unsigned long get_modal_depth() const;
};


#endif //EFP_FORMULA_H
