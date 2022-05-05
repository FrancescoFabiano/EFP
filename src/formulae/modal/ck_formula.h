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

    bool is_entailed(const kstate &state, const kworld_ptr &world) const override;
    bool is_entailed(const pstate &state, const pworld_ptr &world) const override;

    bool is_propositional() const override;
};

#endif //EFP_CK_FORMULA_H
