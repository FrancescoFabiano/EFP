#ifndef EFP_OR_FORMULA_H
#define EFP_OR_FORMULA_H


#include "../formula.h"

class or_formula : public formula {
private:
    formula m_f1, m_f2;

protected:
    unsigned long calc_modal_depth() const override;

public:
    or_formula(const formula &f1, const formula &f2);

    bool is_entailed(const kstate &state, const kworld_ptr &world) const override;
    bool is_entailed(const pstate &state, const pworld_ptr &world) const override;
};

#endif //EFP_OR_FORMULA_H
