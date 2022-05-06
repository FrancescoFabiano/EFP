#ifndef EFP_AND_FORMULA_H
#define EFP_AND_FORMULA_H


#include "../formula.h"

class and_formula : public formula {
private:
    const formula *m_f1, *m_f2;

public:
    and_formula(const formula *f1, const formula *f2);

    bool is_entailed(const kstate &state, const kworld_ptr &world) const override;
    bool is_entailed(const pstate &state, const pworld_ptr &world) const override;

    bool operator==(const and_formula &to_compare) const;
    bool operator<(const and_formula &to_compare) const;
};


#endif //EFP_AND_FORMULA_H
