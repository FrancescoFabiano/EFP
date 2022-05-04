#ifndef EFP_BOX_FORMULA_H
#define EFP_BOX_FORMULA_H


#include "../formula.h"

class box_formula : public formula {
private:
    agent m_ag;
    formula m_f;

protected:
    unsigned long calc_modal_depth() const override;

public:
    box_formula(const agent &ag, const formula &f);

    bool is_entailed(const kstate &state, const kworld_ptr &world) const override;
    bool is_entailed(const pstate &state, const pworld_ptr &world) const override;

    bool is_propositional() const override;
};


#endif //EFP_BOX_FORMULA_H
