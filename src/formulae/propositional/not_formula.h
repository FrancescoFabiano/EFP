#ifndef EFP_NOT_FORMULA_H
#define EFP_NOT_FORMULA_H


#include "../formula.h"

class not_formula : public formula {
private:
    const formula *m_f;

public:
    explicit not_formula(const formula *f);

    bool is_entailed(const kstate &state, const kworld_ptr &world) const override;
    bool is_entailed(const pstate &state, const pworld_ptr &world) const override;

    bool operator==(const not_formula &to_compare) const;
    bool operator<(const not_formula &to_compare) const;
};


#endif //EFP_NOT_FORMULA_H
