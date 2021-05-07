/**
 * \brief Implementation of \ref delphic_helper.h.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date April 29, 2021
 */

#include "delphic_helper.h"
#include "../utilities/helper_t.ipp"
#include "event.h"
#include "pem.h"
#include "pem_store.h"

pem_ptr delphic_helper::get_pem(const action & act)
{
	pem_id p_id;
	switch ( act.get_type() ) {
	case ONTIC:
	{
		//Hard-coded
		p_id = 2;
		break;
	}
	case SENSING:
	case ANNOUNCEMENT:
	{
		//Hard-coded
		p_id = 1;
		break;
	}
	default:
		std::cerr << "\nError: wrong action specification.\n";
		break;
	}

	return pem_store::get_instance().get_pem(p_id);

}

const pstate & delphic_helper::union_update(const pstate & state, const action & act)
{
	//DEBUG
	event_id e_id = 2;
	event_ptr event = pem_store::get_instance().get_event(e_id);
	std::cerr << "\nPrinting Action " << act.get_name() << " x Event " << e_id << "!\n";
	std::cerr << "*******Action*******";
	act.print();
	std::cerr << "\n*******Event*******";
	event.print();
	auto entailed_eff = helper::get_effects_if_entailed(act.get_effects(), state);

	std::cerr << "*******Effects*******\n";
	printer::get_instance().print_list(entailed_eff);
	std::cerr << "\n********************\n";

	std::cerr << "Real Preconditions (in the initial state): ";
	printer::get_instance().print_list(delphic_helper::get_total_pre(act.get_executability(), entailed_eff, event));
	std::cerr << "\nReal Postconditions (in the initial state): ";
	printer::get_instance().print_list(delphic_helper::get_total_effects(entailed_eff, event));




	return state;
}

const kstate & delphic_helper::union_update(const kstate & state, const action & act)
{
	std::cerr << "\nError: Union update not yet implmente for Kripke structures\n";
	exit(1);
}

const pworld & delphic_helper::world_cartesian_product(const pworld & world, const event_ptr & e)
{
	return world;
}

fluent_formula delphic_helper::get_total_effects(const fluent_formula & action_eff, const event_ptr & e)
{
	auto meta_post = e.get_meta_postconditions();
	if (meta_post.size() > 1) {
		std::cerr << "Error: malformed action postcondition in event " << e.get_id() << std::endl;
		exit(1);
	} else if (meta_post.size() == 0) {
		fluent_formula empty;
		return empty;
	} else {
		switch ( *meta_post.begin() ) {
		case act_eff:
			return action_eff;
		case neg_act_eff:
			return helper::negate_fluent_formula(action_eff);
			//Not break because it means it is empty
			//break;
		case none:
		default:
			fluent_formula empty;
			return empty;
			break;
		}
	}

	//Need to insert the merge with specific postcondition
}

formula_list delphic_helper::get_total_pre(const formula_list & action_pre, const fluent_formula & action_eff, const event_ptr & e)
{

	formula_list ret;


	auto meta_pre = e.get_meta_precondition();

	for (auto it_meta_pre = meta_pre.begin(); it_meta_pre != meta_pre.end(); ++it_meta_pre) {
		belief_formula tmp_bf;

		switch ( *it_meta_pre ) {
		case act_eff:
			tmp_bf.set_from_ff(action_eff);
			ret.push_back(tmp_bf);
			break;
		case neg_act_eff:
			tmp_bf.set_from_ff(helper::negate_fluent_formula(action_eff));
			ret.push_back(tmp_bf);
			break;
		case act_pre:
			for (auto it_act_pre = action_pre.begin(); it_act_pre != action_pre.end(); ++it_act_pre) {
				ret.push_back(*it_act_pre);
				
			}
			break;
		case none:
		default:
			ret.clear();
			return ret;
			break;
		}
	}

	return ret;

}
