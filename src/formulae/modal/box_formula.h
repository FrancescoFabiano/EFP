#ifndef EFP_BOX_FORMULA_H
#define EFP_BOX_FORMULA_H


#include "../formula.h"

template<class M>
class box_formula : public formula {
private:
    const agent *m_ag;
    const formula *m_f;

public:
    box_formula<M>(const agent *ag, const formula *f);

    bool is_entailed(const kstate &state, const kworld_ptr &world) const override;
    bool is_entailed(const pstate &state, const pworld_ptr &world) const override;

    bool is_propositional() const override;

    bool operator==(const box_formula<M> &to_compare) const;
    bool operator<(const box_formula<M> &to_compare) const;
};


#endif //EFP_BOX_FORMULA_H
