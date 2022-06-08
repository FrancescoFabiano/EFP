#ifndef EFP_OR_FORMULA_H
#define EFP_OR_FORMULA_H


#include "../formula.h"

class or_formula : public formula {
private:
    const formula *m_f1, *m_f2;

public:
    or_formula(const formula *f1, const formula *f2);

    bool is_entailed(const fluent_set *fluent_set) const override;
    bool is_entailed(const kstate *state, const kworld *world) const override;
    bool is_entailed(const pstate *state, const pworld *world) const override;

    bool operator==(const or_formula &to_compare) const;
    bool operator<(const or_formula &to_compare) const;
};

#endif //EFP_OR_FORMULA_H
