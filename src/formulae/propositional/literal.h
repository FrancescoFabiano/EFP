#ifndef EFP_LITERAL_H
#define EFP_LITERAL_H


#include "../formula.h"

class literal : public formula {
private:
    fluent m_fluent;
    bool m_is_negated;

protected:
    unsigned long calc_modal_depth() const override;

public:
    literal(const fluent &fluent, bool is_negated);

    bool is_entailed(const kstate &state, const kworld_ptr &world) const override;
    bool is_entailed(const pstate &state, const pworld_ptr &world) const override;
};


#endif //EFP_LITERAL_H
