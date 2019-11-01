/*
 * \brief Class that implements planning_graph.h.
 *
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date November 1, 2019
 */

#include "planning_graph.h"

pg_eState::pg_eState()
{
	set_action(-1);
}

pg_eState::pg_eState(const kstate & partial_kripke, const kworld_ptr_set & pointeds)
{
	set_partial_kripke(partial_kripke);
	set_pointeds(pointeds);
}

void pg_eState::set_partial_kripke(const kstate & partial_kripke)
{
	m_partial_kripke = partial_kripke;
}

void pg_eState::set_pointeds(const kworld_ptr_set & pointeds)
{
	m_pointeds = pointeds;
}

void pg_eState::add_pointed(kworld_ptr pointed)
{
	m_pointeds.insert(pointed);
}

void pg_eState::set_action(action_id action)
{
	m_action = action;
}

const kstate & pg_eState::get_partial_kripke() const
{
	return m_partial_kripke;
}

const kworld_ptr_set & pg_eState::get_pointeds() const
{
	return m_pointeds;
}

action_id pg_eState::get_action() const
{
	return m_action;
}
//printing

void pg_eState::print() const
{
	std::cout << "\n The partial Kripke structure in the node after executing " << action << " is:\n";
	m_partial_kripke.print();
	std::cout << "\n The set of pointed states is: ";
	printer::get_instance().print_list(m_pointeds);
	std::cout << "\n------------------------------\n";
}

pg_action_level::pg_action_level()
{
}

pg_action_level::pg_action_level(const action_set & actions)
{
	set_actions(actions);
}

void pg_action_level::set_actions(const action_set & actions)
{
	m_actions = actions;
}

void pg_action_level::add_action(const action & act)
{
	m_actions.insert(act);
}

const action_set & pg_action_level::get_actions() const
{
	return action_set;
}

//printing

void pg_action_level::print() const
{
	std::cout << "\nPrinting an Action Level:";
	for (action_set::iterator it_acts = actions.begin(); it_acts != actions.end(); it_acts++) {
		std::cout << "\nAction " << it_acts->get_name() << std::endl;
	}
}

pg_state_level::pg_state_level()
{
}

pg_state_level::pg_state_level(const pg_eState_set & eStates)
{
	set_eStates(eStates);
}

void pg_state_level::set_eStates(const pg_eState_set & eStates)
{
	m_eStates = eStates;
}

void pg_state_level::add_eState(const pg_eState & eState)
{
	m_eStates.insert(eState);
}

const pg_eState_set& pg_state_level::get_eState()
{
	return m_eStates;
}

//check satisfaction of a belief formula given an epistemic_model_list level (i.e., eNodes)

bool pg_state_level::pg_entailment(const pg_eState_set & eStates, const belief_formula & bf) const
{
	pg_eState_set::const_iterator it_ests;
	switch ( bf.bf_type ) {
	case FLUENT_FORMULA:
	{
		fluent_formula ff = bf.get_fluent_formula;
		//For example, assume ff = [ [1,2] , [3,4] ], i.e., (1 and 2) or (3 and 4)
		for (fluent_formula::iterator it_ff = ff.begin(); it_ff != ff.end(); it_ff++) {
			//Fluent Formula is in it_ffDNF form, so need one of *it_ff satisfied to finish
			// *it_ff = [1 ,2 ]
			bool andSatisfied = true;
			for (fluent_set::iterator it_fs = it_ff->begin(); it_fs != it_ff->end(); it_fs++) {
				//need all *i satisfied to finish
				//*i  = 1
				//generate BFNode to test
				BFNode test = BFNode();
				test.node_type = fluForm;
				Fluents fs = Fluents();
				fs.insert(*it_fs);
				FluentFormula ffs = FluentFormula();
				ffs.insert(fs);
				test.fluform = &ffs;

				//test for each EpistemicModelsNode, need at least one node to satisfy
				bool litsatisfy = false;
				for (EpistemicModelsNode emn : v) {
					Kripke* curK = emn.get_Kripke();
					set<int> pts = emn.get_pointeds();
					//need all pId in pts of this node to satisfy
					bool nodefail = false;
					for (int p : pts) {
						curK->set_pId(p);
						if (!curK->entail(test)) {
							nodefail = true;
							break;
						}
					}
					if (!nodefail) {
						litsatisfy = true;
						break;
					}
				}
				if (!litsatisfy) {
					andSatisfied = false;
					break;
				}
			}
			if (andSatisfied) {
				return true;
			}
		}
		return false;
	}
	case BForm:
	{
		Agent ag = bf.agentPro2;
		BFNode bfnext = *bf.bfnode1;
		//move edge of label ag for each element of v.
		vector<EpistemicModelsNode> newV = vector<EpistemicModelsNode> ();
		for (it_ests = v.begin(); it_ests != v.end(); it_ests++) {
			Kripke* kstd = (*it_ests).get_Kripke();
			set<int> pointeds = (*it_ests).get_pointeds();
			set<int> nextPointeds = set<int>();
			for (set<int>::iterator ip = pointeds.begin(); ip != pointeds.end(); ip++) {
				map<int, set<int> > k1map = kstd->get_stateslist().at(*ip).get_preparedMap();
				if (k1map.find(ag) != k1map.end()) {
					set<int> forcheck = k1map[ag];
					nextPointeds.insert(forcheck.cbegin(), forcheck.cend());
				}
			}
			newV.push_back(EpistemicModelsNode(kstd, nextPointeds));
		}
		return emNodes_entailment(newV, bfnext);
	}
	case propForm:
	{
		if (bf.bfOperator == BFNOT) {
			//TODO: check here again, for now we assume all -B_i \phi is satisfied because we cannot verify this.
			//return !emNodes_entailment(v,*bf.bfnode1);
			return true;
		} else if (bf.bfOperator == NONE) {
			return emNodes_entailment(v, *bf.bfnode1);
		} else if (bf.bfOperator == BFOR) {
			return emNodes_entailment(v, *bf.bfnode1) || emNodes_entailment(v, *bf.bfnode2);
		} else if (bf.bfOperator == BFAND) {
			return emNodes_entailment(v, *bf.bfnode1) && emNodes_entailment(v, *bf.bfnode2);
		} else {
			return false;
		}
	}
	case EForm:
	{
		Agents::iterator it;
		Agents bfag = bf.agents;
		BFNode bfnext = *bf.bfnode1;
		//move edge of label ag for each element of v.
		vector<EpistemicModelsNode> newV = vector<EpistemicModelsNode> ();
		for (it_ests = v.begin(); it_ests != v.end(); it_ests++) {
			Kripke* kstd = (*it_ests).get_Kripke();
			set<int> pointeds = (*it_ests).get_pointeds();
			set<int> nextPointeds = set<int>();
			for (set<int>::iterator ip = pointeds.begin(); ip != pointeds.end(); ip++) {
				map<int, set<int> > k1map = kstd->get_stateslist().at(*ip).get_preparedMap();
				for (it = bfag.begin(); it != bfag.end(); it++) {
					Agent ag = *it;
					if (k1map.find(ag) != k1map.end()) {
						set<int> forcheck = k1map[ag];
						nextPointeds.insert(forcheck.cbegin(), forcheck.cend());
					}
				}
			}
			newV.push_back(EpistemicModelsNode(kstd, nextPointeds));
		}
		return emNodes_entailment(newV, bfnext);
	}
	case CForm:
	{
		Agents ags = bf.agents;
		BFNode bfnext = *bf.bfnode1;
		vector<EpistemicModelsNode> newV = vector<EpistemicModelsNode> ();
		for (it_ests = v.begin(); it_ests != v.end(); it_ests++) {
			Kripke* kstd = (*it_ests).get_Kripke();
			set<int> pointeds = (*it_ests).get_pointeds();
			set<int> nextPointeds = set<int>();
			for (set<int>::iterator ip = pointeds.begin(); ip != pointeds.end(); ip++) {
				StateList sl = kstd->reachableStates(*ip, ags);
				nextPointeds.insert(sl.cbegin(), sl.cend());
			}
			newV.push_back(EpistemicModelsNode(kstd, nextPointeds));
		}
		return emNodes_entailment(newV, bfnext);
	}
	case BFEmpty:
		return true;
	}
}
bool pg_state_level::pg_entailment(const belief_formula & bf) const;

bool pg_state_level::pg_executable(const action & act) const;

//printing
void pg_state_level::print() const;