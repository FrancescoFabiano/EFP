#ifndef EFP_CK_FORMULA_H
#define EFP_CK_FORMULA_H


#include "../formula.h"

class ck_formula : public formula {
private:
    agent_set m_ags;
    formula m_f;

protected:
    unsigned long calc_modal_depth() const override;

public:
    ck_formula(const agent_set &ags, const formula &f);

    bool is_entailed(const kstate &state, const kworld_ptr &world) const override;
    bool is_entailed(const pstate &state, const pworld_ptr &world) const override;

    bool is_propositional() const override;
};

#endif //EFP_CK_FORMULA_H
