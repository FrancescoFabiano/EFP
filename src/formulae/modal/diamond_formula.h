#ifndef EFP_DIAMOND_FORMULA_H
#define EFP_DIAMOND_FORMULA_H


#include "../formula.h"

template<class M>
class diamond_formula : public formula {
private:
    const agent *m_ag;
    const formula *m_f;

public:
    diamond_formula<M>(const agent *ag, const formula *f);

    bool is_entailed(const kstate &state, const kworld_ptr &world) const override;
    bool is_entailed(const pstate &state, const pworld_ptr &world) const override;

    bool is_propositional() const override;

    bool operator==(const diamond_formula<M> &to_compare) const;
    bool operator<(const diamond_formula<M> &to_compare) const;
};

#endif //EFP_DIAMOND_FORMULA_H
