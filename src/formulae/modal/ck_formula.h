#ifndef EFP_CK_FORMULA_H
#define EFP_CK_FORMULA_H


#include "../formula.h"

template<class M>
class ck_formula : public formula {
private:
    const agent_set *m_ags;
    const formula *m_f;

public:
    ck_formula<M>(const agent_set *ags, const formula *f);

    bool is_entailed(const kstate *state, const kworld *world) const override;
    bool is_entailed(const pstate *state, const pworld *world) const override;

    bool is_propositional() const override;

    bool operator==(const ck_formula<M> &to_compare) const;
    bool operator<(const ck_formula<M> &to_compare) const;
};

#endif //EFP_CK_FORMULA_H
