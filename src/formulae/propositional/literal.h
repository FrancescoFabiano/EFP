#ifndef EFP_LITERAL_H
#define EFP_LITERAL_H


#include "../formula.h"

class literal : public formula {
private:
    const fluent *m_fluent;
    const bool m_is_negated;

public:
    literal(const fluent *fluent, bool is_negated);

    bool is_entailed(const kstate &state, const kworld_ptr &world) const override;
    bool is_entailed(const pstate &state, const pworld_ptr &world) const override;
};


#endif //EFP_LITERAL_H
