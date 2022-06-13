#ifndef EFP_BOX_WHETHER_FORMULA_H
#define EFP_BOX_WHETHER_FORMULA_H

#include "../formula.h"

template<class M>
class box_whether_formula : public formula {
private:
    const agent* m_ag;
    const formula *m_f;

public:
    box_whether_formula<M>(const agent* ag, const formula *f);

    bool is_entailed(const kstate *state, const kworld *world) const override;
    bool is_entailed(const pstate *state, const pworld *world) const override;

    bool is_propositional() const override;

    const agent* get_agent() const;
    const formula* get_formula() const;

    bool operator==(const box_whether_formula<M> &to_compare) const;
    bool operator<(const box_whether_formula<M> &to_compare) const;
};

#endif //EFP_BOX_WHETHER_FORMULA_H
