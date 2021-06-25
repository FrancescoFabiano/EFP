///**
// * \class state
// * \brief Class that encodes a state of planner.h.
// *
// * \copyright GNU Public License.
// *
// * \author Francesco Fabiano.
// * \date June 21, 2021
// */
//
//#pragma once
//#include <fstream>
//
//#include "kripke/kstate.h"
//#include "possibilities/pstate.h"
//#include "possibilities_opt/pstate_opt.h"
//
//class state {
//public:
//    virtual world add_world(const world_ptr & world);
//    virtual world add_edge(const world_ptr & world1, const world_ptr & world2, const agent & ag);
//
//    virtual void remove_world(const world_ptr & world);
//    virtual void remove_edge(const world_ptr & world1, const world_ptr & world2, const agent & ag);
//
//    virtual world_set get_believed_worlds(const agent & ag, const world_ptr & world);
//    virtual world_set get_believed_worlds(const agent_set & ags, const world_ptr & world);
//
//    virtual world_set get_reached_worlds(const agent_set & ags, const world_ptr & world) const;
//
//    virtual static bool entails(const fluent & to_check, const world_ptr & world);
//    virtual static bool entails(const fluent_set & to_check, const world_ptr & world);
//    virtual static bool entails(const fluent_formula & to_check, const pworld_ptr & world);
//    virtual bool entails(const belief_formula & to_check, const pworld_ptr & world) const;
//    virtual bool entails(const belief_formula & to_check, const pworld_ptr_set & worlds) const;
//    virtual bool entails(const formula_list & to_check, const pworld_ptr & world) const;
//
//    virtual void build_initial();
//    virtual state compute_succ(const action & act) const;
//
//    virtual void contract();
//};
