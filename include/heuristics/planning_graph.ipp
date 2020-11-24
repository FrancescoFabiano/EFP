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
#include "../domain/domain.h"
//pg_eState::pg_eState()
//{
//	set_action(-1);
//}
//
//pg_eState::pg_eState(const T & partial_kripke, const kworld_ptr_set & pointeds)
//{
//	set_partial_kripke(partial_kripke);
//	set_pointeds(pointeds);
//}
//
//void pg_eState::set_partial_kripke(const T & partial_kripke)
//{
//	m_partial_kripke = partial_kripke;
//}
//
//void pg_eState::set_pointeds(const kworld_ptr_set & pointeds)
//{
//	m_pointeds = pointeds;
//}
//
//void pg_eState::add_pointed(kworld_ptr pointed)
//{
//	m_pointeds.insert(pointed);
//}
//
//void pg_eState::set_action(action_id action)
//{
//	m_action = action;
//}
//
//const T & pg_eState::get_partial_kripke() const
//{
//	return m_partial_kripke;
//}
//
//const kworld_ptr_set & pg_eState::get_pointeds() const
//{
//	return m_pointeds;
//}
//
//action_id pg_eState::get_action() const
//{
//	return m_action;
//}
////printing
//
//void pg_eState::print() const
//{
//	std::cout << "\n The partial Kripke structure in the node after executing " << action << " is:\n";
//	m_partial_kripke.print();
//	std::cout << "\n The set of pointed states is: ";
//	printer::get_instance().print_list(m_pointeds);
//	std::cout << "\n------------------------------\n";
//}



pg_action_level::pg_action_level()
{

	set_depth(0);
}

pg_action_level::pg_action_level(const action_set & actions)
{

	set_actions(actions);
	set_depth(0);
}

pg_action_level::pg_action_level(const action_set & actions, unsigned short depth)
{

	set_actions(actions);
	set_depth(depth);
}

void pg_action_level::set_actions(const action_set & actions)
{

	m_actions = actions;
}

void pg_action_level::add_action(const action & act)
{

	m_actions.insert(act);
}

void pg_action_level::set_depth(unsigned short depth)
{
	m_depth = depth;
}

unsigned short pg_action_level::get_depth() const
{
	return m_depth;
}

const action_set & pg_action_level::get_actions() const
{

	return m_actions;
}

bool pg_action_level::operator=(const pg_action_level& to_assign)
{
	set_actions(to_assign.get_actions());
	set_depth(to_assign.get_depth());
	return true;
}


//printing

void pg_action_level::print() const
{
	std::cout << "\nPrinting an Action Level:";
	for (action_set::iterator it_acts = m_actions.begin(); it_acts != m_actions.end(); it_acts++) {

		std::cout << "\nAction " << it_acts->get_name() << std::endl;
	}
}

/*\*********************************************************************************************/
template <class T>
pg_state_level<T>::pg_state_level()
{
	set_depth(0);
}

template <class T>
pg_state_level<T>::pg_state_level(const std::set<T> & eStates)
{

	set_eStates(eStates);
	set_depth(0);

}

template <class T>
pg_state_level<T>::pg_state_level(const std::set<std::pair<belief_formula,bool>> & pairBeliefBool, const std::set<fluent> & fluentSet)
{

    set_pair_belief_bool(pairBeliefBool);
    set_fluent_set(fluentSet);
    set_depth(0);

}

template <class T>
pg_state_level<T>::pg_state_level(const std::set<std::pair<belief_formula,bool>> & pairBeliefBool, const std::set<fluent> & fluentSet, unsigned short depth)
{

    set_pair_belief_bool(pairBeliefBool);
    set_fluent_set(fluentSet);
    set_depth(depth);

}


template <class T>
pg_state_level<T>::pg_state_level(const std::set<T> & eStates, unsigned short depth)
{

	set_eStates(eStates);
	set_depth(depth);

}

template <class T>
void pg_state_level<T>::set_eStates(const std::set<T> & eStates)
{

	m_eStates = eStates;
}

template <class T>
void pg_state_level<T>::set_pair_belief_bool(const std::set<std::pair<belief_formula,bool>> & pairBeliefBool)
{

    m_pairBeliefBool = pairBeliefBool;
}

template <class T>
void pg_state_level<T>::set_fluent_set(const std::set<fluent> & fluentSet)
{

    m_fluentSet = fluentSet;
}

template <class T>
bool pg_state_level<T>::add_eState(const T & eState)
{
	return(m_eStates.insert(eState)).second;
}

template <class T>
bool pg_state_level<T>::add_fluent(const fluent & fluent)
{
    return(m_fluentSet.insert(fluent)).second;
}

template <class T>
void pg_state_level<T>::set_depth(unsigned short depth)
{
	m_depth = depth;
}

template <class T>
unsigned short pg_state_level<T>::get_depth() const
{
	return m_depth;
}

template <class T>
const std::set<T>& pg_state_level<T>::get_eStates() const
{

	return m_eStates;
}
template <class T>
const std::set<std::pair<belief_formula,bool>> & pg_state_level<T>::get_pair_belief_bool() const
{
    return m_pairBeliefBool;
}
template <class T>
const std::set<fluent>& pg_state_level<T>::get_fluent_set() const
{

    return m_fluentSet;
}


//check satisfaction of a belief formula given an epistemic_model_list level (i.e., eNodes)

template <class T>
bool pg_state_level<T>::pg_entailment(const std::set<std::pair<belief_formula,bool>> & pairBeliefBool, const belief_formula & bf) const
{
	typename std::set<std::pair<belief_formula,bool>>::const_iterator it_kstset;
	std::pair<belief_formula,bool> pairFalse = std::make_pair(bf,false);
    std::pair<belief_formula,bool> pairTrue = std::make_pair(bf,true);

    if(  pairBeliefBool.find(pairTrue) != pairBeliefBool.end() )
    {
        return true;
    }
    else if (pairBeliefBool.find(pairFalse) != pairBeliefBool.end()){
        return false;
    }
    else
    {
        std::cout << "Errore creazione planning graph" << std::endl;
        std::exit(1);
    }

}

template <class T>
bool pg_state_level<T>::pg_entailment(const belief_formula & bf) const
{

	return pg_entailment(m_pairBeliefBool, bf);
}

template <class T>
bool pg_state_level<T>::pg_entailment(const std::set<std::pair<belief_formula,bool>> & pairBeliefBool, const formula_list & fl) const
{

	formula_list::const_iterator it_fl;
	for (it_fl = fl.begin(); it_fl != fl.end(); it_fl++) {
		if (!pg_entailment(pairBeliefBool, (*it_fl))) {
			return false;
		}
	}
	return true;
}

template <class T>
bool pg_state_level<T>::pg_entailment(const formula_list & fl) const
{

	return pg_entailment(m_pairBeliefBool, fl);
}


//TODO concetto di entailment per le fluent formule, check nei fluent set ci siano tutti nella fluent formula

template <class T>
bool pg_state_level<T>::pg_executable(const action & act) const
{
	return pg_entailment(act.get_executability());
}


template <class T>
bool pg_state_level<T>::operator=(const pg_state_level& to_assign)
{
	set_eStates(to_assign.get_eStates());
	set_depth(to_assign.get_depth());
	return true;
}

/*\******************************************************************************************************************/

/*\*****START PLANNING GRAPH TIME MEASURE*******
std::chrono::duration<double> t1, t2, t3, t4;

 *\******END PLANNING GRAPH TIME MEASURE********/
template <class T>
planning_graph<T>::planning_graph(const T& state_init, const formula_list & goal, bool is_single)
{
	/*\*****START PLANNING GRAPH TIME MEASURE*******
	auto start_pg_build = std::chrono::system_clock::now();
	 *\******END PLANNING GRAPH TIME MEASURE********/


	set_goal(goal);
	pg_state_level<T> pg_init;
	pg_init.add_eState(state_init);
	m_state_levels.push_back(pg_init);
	m_never_executed = domain::get_instance().get_actions();
	set_length(0);
	set_sum(0);


	bool goal_reached = true;
	formula_list::const_iterator it_fl;

	/*\*****START PLANNING GRAPH TIME MEASURE*******
	auto start_pg_goal_ini = std::chrono::system_clock::now();
	 *\******END PLANNING GRAPH TIME MEASURE********/

	for (it_fl = m_goal.begin(); it_fl != m_goal.end();) {
		if (m_state_levels.back().pg_entailment(*it_fl)) {
			it_fl = m_goal.erase(it_fl);

		} else {
			goal_reached = false;
			it_fl++;
		}
	}



	/*\*****START PLANNING GRAPH TIME MEASURE*******
	auto end_pg_goal_ini = std::chrono::system_clock::now();
	std::chrono::duration<double> pg_goal_ini_time = end_pg_goal_ini - start_pg_goal_ini;
	t1 = std::chrono::milliseconds::zero();
	t2 = std::chrono::milliseconds::zero();
	t3 = std::chrono::milliseconds::zero();
	t4 = std::chrono::milliseconds::zero();
	 *\******END PLANNING GRAPH TIME MEASURE********/

	if (!goal_reached) {
		if (!is_single) {
			pg_build();
		} else {
			std::vector<belief_formula> to_explore = list_bf_classical();
			pg_build_classical(to_explore);

			//TODO
//            std::list<belief_formula> to_explore = list_bf_grounded(1,goal);
  //          pg_build_initially(to_explore);

		}

	} else {
		std::cerr << "\nBUILDING: The given state is goal\n";
		exit(1);
	}

	/*
	 * Call new implementation

	    if (!goal_reached) {
		if (!is_single) {
	        std::vector<belief_formula> to_explore = list_bf_classical();
			pg_build_grounded(to_explore);
		} else {
			std::vector<belief_formula> to_explore = list_bf_classical();
			pg_build_classical(to_explore);
		}

	} else {
		std::cerr << "\nBUILDING: The given state is goal\n";
		exit(1);
	}

	 */


	/*\*****START PLANNING GRAPH TIME MEASURE*******
	auto end_pg_build = std::chrono::system_clock::now();
	std::chrono::duration<double> pg_build_time = end_pg_build - start_pg_build;
	std::cout << "\n\nGenerated Planning Graph of length " << get_length() << " in " << pg_build_time.count() << " seconds of which:";
	std::cout << "\nFirst goal check:      " << pg_goal_ini_time.count();
	std::cout << "\nAction Level creation: " << t1.count();
	std::cout << "\n\nState Level Creation:  " << t2.count() << " of which:";
	std::cout << "\nState equality:        " << t4.count();
	std::cout << "\n\nGoals Check:           " << t3.count() << std::endl;
	 *\******END PLANNING GRAPH TIME MEASURE********/


}

template <class T>
void planning_graph<T>::set_satisfiable(bool sat)
{

	m_satisfiable = sat;
}

template <class T>
bool planning_graph<T>::is_satisfiable()
{

	return m_satisfiable;
}

template <class T>
void planning_graph<T>::pg_build()
{
	pg_state_level<T> s_level_curr = m_state_levels.back();
	pg_action_level a_level_curr;
	a_level_curr.set_depth(get_length());

	/*\*****START PLANNING GRAPH TIME MEASURE*******
	auto start = std::chrono::system_clock::now();
	 *\******END PLANNING GRAPH TIME MEASURE********/

	if (m_action_levels.size() > 0) {
		a_level_curr = m_action_levels.back();
	}
	action_set::iterator it_actset;
	for (it_actset = m_never_executed.begin(); it_actset != m_never_executed.end();) {
		if (s_level_curr.pg_executable(*it_actset)) {
			a_level_curr.add_action(*it_actset);
			it_actset = m_never_executed.erase(it_actset);
		} else {
			it_actset++;
		}
	}
	/*\*****START PLANNING GRAPH TIME MEASURE*******
	auto end = std::chrono::system_clock::now();
	t1 += end - start;
	 *\******END PLANNING GRAPH TIME MEASURE********/


	add_action_level(a_level_curr);
	set_length(get_length() + 1);

	//The no-op is done with the copy
	pg_state_level<T> s_level_next = s_level_curr;
	s_level_next.set_depth(get_length());
	bool new_state_insertion = false;
	T tmp_state;

	/*\*****START PLANNING GRAPH TIME MEASURE*******
	start = std::chrono::system_clock::now();
	 *\******END PLANNING GRAPH TIME MEASURE********/

	/*\*****START PLANNING GRAPH TIME MEASURE*******
	std::cerr << "\n\nNew Plan Length: " << get_length();
	if (get_length() > 3) {
		std::cerr << "\nAction Level creation: " << t1.count();
		std::cerr << "\nState Level Creation:  " << t2.count() << " of which:";
		std::cerr << "\nState equality:        " << t4.count();
	}
	 *\******END PLANNING GRAPH TIME MEASURE********/


	for (it_actset = a_level_curr.get_actions().begin(); it_actset != a_level_curr.get_actions().end(); it_actset++) {

		for (typename std::set<T>::const_iterator it_kstset = s_level_curr.get_eStates().begin(); it_kstset != s_level_curr.get_eStates().end(); it_kstset++) {
			tmp_state = *it_kstset;
			//std::cerr << "\nChecking New State";

			/*\*****START PLANNING GRAPH TIME MEASURE*******
			auto startN = std::chrono::system_clock::now();
			bool tmptest = s_level_next.add_eState(tmp_state.compute_succ(*it_actset));
			auto endN = std::chrono::system_clock::now();
			t4 += endN - startN;
			if (tmptest) {
				//				std::cerr << "\nAdded New State";
				new_state_insertion = true;
			} else {
				//				std::cerr << "\nNOT Added New State";

			}
			 *\******END PLANNING GRAPH TIME MEASURE********/

			if (s_level_next.add_eState(tmp_state.compute_succ(*it_actset))) {
				new_state_insertion = true;
			}

		}
	}
	/*\*****START PLANNING GRAPH TIME MEASURE*******
	end = std::chrono::system_clock::now();
	t2 += end - start;
	 *\******END PLANNING GRAPH TIME MEASURE********/


	add_state_level(s_level_next);

	bool goal_reached = true;
	//unsigned short goal_index = 0;


	formula_list::const_iterator it_fl;

	/*\*****START PLANNING GRAPH TIME MEASURE*******
	start = std::chrono::system_clock::now();
	 *\******END PLANNING GRAPH TIME MEASURE********/


	//Remove each sub goal already satisfied: it will always be and we just need to check it once
	for (it_fl = m_goal.begin(); it_fl != m_goal.end();) {

		if (s_level_next.pg_entailment(*it_fl)) {
			it_fl = m_goal.erase(it_fl);
			m_pg_sum += m_pg_length;

		} else {
			it_fl++;
			goal_reached = false;
		}
	}
	/*\*****START PLANNING GRAPH TIME MEASURE*******
	end = std::chrono::system_clock::now();
	t3 += end - start;
	 *\******END PLANNING GRAPH TIME MEASURE********/
	if (goal_reached) {
		set_satisfiable(true);
		return;
	} else if (!new_state_insertion) {
		set_satisfiable(false);
		return;
	} else {
		pg_build();
	}
}

template <class T>
void planning_graph<T>::pg_build_initially(std::list<belief_formula> & converted_bf) //aggiungere come parametri la lista iniziale delle belief formula e dei fluentset
{

    std::set<std::pair<belief_formula, bool>> m_pairBeliefBool;
    int n = converted_bf.size();
    std::list<belief_formula>::iterator iter;
    for(iter = converted_bf.begin(); iter != converted_bf.end(); iter++ )
    {
        std::pair <belief_formula,bool> bar = std::make_pair(*iter,true);
        m_pairBeliefBool.insert(bar);
    }
    pg_state_level<T> s_level_curr = m_state_levels.back();
    pg_action_level a_level_curr;
    a_level_curr.set_depth(get_length());

    if (m_action_levels.size() > 0) {
        a_level_curr = m_action_levels.back();
    }
    action_set::iterator it_actset;
    for (it_actset = m_never_executed.begin(); it_actset != m_never_executed.end();) {
        if (s_level_curr.pg_executable(*it_actset)) {
            a_level_curr.add_action(*it_actset);
            it_actset = m_never_executed.erase(it_actset);
        } else {
            it_actset++;
        }
    }
    /*\*****START PLANNING GRAPH TIME MEASURE*******
    auto end = std::chrono::system_clock::now();
    t1 += end - start;
     *\******END PLANNING GRAPH TIME MEASURE********/


    add_action_level(a_level_curr);
    set_length(get_length() + 1);

    s_level_curr.set_pair_belief_bool(m_pairBeliefBool);
    //aggiungo i fluenti iniziali
    s_level_curr.add_fluent(domain::get_instance().get_initial_description().get_initially_known_fluents());
    //The no-op is done with the copy
    pg_state_level<T> s_level_next = s_level_curr;
    s_level_next.set_depth(get_length());

    add_state_level(s_level_next);

    //prendo tutte le componeenti delle azioni belief formula nella descrizione del dominio e sono tutte a false

    pg_build_grounded();
}


template <class T>
void planning_graph<T>::pg_build_grounded()//std::vector<belief_formula> & converted_bf) //aggiungere come parametri la lista iniziale delle belief formula e dei fluentset
{
    //aggiungo a fuori
    //aggiungo a tutte le belief formule false all'inizo
    //aggiungo e contorllo all'inizo se il belief formula sono vere all'inzio
    /*std::set<std::pair<belief_formula,bool>> m_pairBeliefBool;
    int n = converted_bf.size();
    for(int i = 0; i < n; i++ )
    {
        m_pairBeliefBool.insert(std::make_pair(converted_bf,true));
    }*/
    pg_state_level<T> s_level_curr = m_state_levels.back();
    pg_action_level a_level_curr;
    a_level_curr.set_depth(get_length());

    /*\*****START PLANNING GRAPH TIME MEASURE*******
    auto start = std::chrono::system_clock::now();
     *\******END PLANNING GRAPH TIME MEASURE********/

    if (m_action_levels.size() > 0) {
        a_level_curr = m_action_levels.back();
    }
    action_set::iterator it_actset;
    for (it_actset = m_never_executed.begin(); it_actset != m_never_executed.end();) {
        if (s_level_curr.pg_executable(*it_actset)) {
            a_level_curr.add_action(*it_actset);
            it_actset = m_never_executed.erase(it_actset);

        } else {
            it_actset++;
        }
    }
    /*\*****START PLANNING GRAPH TIME MEASURE*******
    auto end = std::chrono::system_clock::now();
    t1 += end - start;
     *\******END PLANNING GRAPH TIME MEASURE********/


    add_action_level(a_level_curr);
    set_length(get_length() + 1);

    //aggiungo i fluenti iniziali lo facc gia in pg_build_initally
    //s_level_curr.add_fluent(domain::get_instance().get_initial_description().get_initially_known_fluents());
    //The no-op is done with the copy
    pg_state_level<T> s_level_next = s_level_curr;
    s_level_next.set_depth(get_length());
    bool new_state_insertion = false;
    T tmp_state;

    /*\*****START PLANNING GRAPH TIME MEASURE*******
    start = std::chrono::system_clock::now();
     *\******END PLANNING GRAPH TIME MEASURE********/

    /*\*****START PLANNING GRAPH TIME MEASURE*******
    std::cerr << "\n\nNew Plan Length: " << get_length();
    if (get_length() > 3) {
        std::cerr << "\nAction Level creation: " << t1.count();
        std::cerr << "\nState Level Creation:  " << t2.count() << " of which:";
        std::cerr << "\nState equality:        " << t4.count();
    }
     *\******END PLANNING GRAPH TIME MEASURE********/



    for (it_actset = a_level_curr.get_actions().begin(); it_actset != a_level_curr.get_actions().end(); it_actset++) {

        //devo ora cercare su i fluenti e sulle belief formule

        for (typename std::set<T>::const_iterator it_kstset = s_level_curr.get_eStates().begin(); it_kstset != s_level_curr.get_eStates().end(); it_kstset++) {
            tmp_state = *it_kstset;

            fluent_set new_fluent = tmp_state.compute_succ2(*it_actset);

            //aggiungo i nuovi fluenti dalla azione
            fluent_set::iterator iter;
            for(iter = new_fluent.begin(); iter != new_fluent.end(); iter++)
            {
                s_level_next.add_fluent(iter);
            }

            std::set<std::pair<belief_formula,bool>> tmp_state_pair_belief_bool = tmp_state.get_pair_belief_bool();
            std::set<std::pair<belief_formula,bool>>::iterator iter2;
            for(iter2 = tmp_state_pair_belief_bool.begin(); iter2 != tmp_state_pair_belief_bool.end();iter2++)
            {
                //controllo nella belief formula falsa
                if(!iter2->second){

                    //ottengo le condizioni di eseguibilità? in relatà è gia eseguita quinid non ha senso
                    //it_actset->get_executability();

                    //ottengo gli effetti della azione che dovrò inserire
                    it_actset->get_effects();
                    //qui posso ora controllare il oblyviusly e il partially e il full (alcuni fully potrebbero essere in partially o viceversa)
                    const std::map<boost::dynamic_bitset<>, belief_formula> fully = it_actset->get_fully_observants();
                    const std::map<boost::dynamic_bitset<>, belief_formula> partially = it_actset->get_partially_observants();

                    //controllo ora se una la belief formula è stata generata
                    //prendo belief formula ancora a false
                    //controllo che l'azione di quale fluente si occupa e guardo se il fluente è uguale
                    //guardo effettuo azione è e dalla beleif controllo  nei ari aggenti cosa succede e aggiungo le nuove belieff
                    //agente può essere oblivious partilayy e completo li creo tutti
                    //se mi blocco dal dominio prendo tutte le belief formla con nesting <= 2 es B(a,B(b,F)) agente a sa che agente b sa f
                    //se ho tre B o più mi fermo
                }
            }

        }
    }
    /*\*****START PLANNING GRAPH TIME MEASURE*******
    end = std::chrono::system_clock::now();
    t2 += end - start;
     *\******END PLANNING GRAPH TIME MEASURE********/


    add_state_level(s_level_next);

    bool goal_reached = true;
    //unsigned short goal_index = 0;


    formula_list::const_iterator it_fl;

    /*\*****START PLANNING GRAPH TIME MEASURE*******
    start = std::chrono::system_clock::now();
     *\******END PLANNING GRAPH TIME MEASURE********/


    //Remove each sub goal already satisfied: it will always be and we just need to check it once
    for (it_fl = m_goal.begin(); it_fl != m_goal.end();) {

        if (s_level_next.pg_entailment(*it_fl)) {
            it_fl = m_goal.erase(it_fl);
            m_pg_sum += m_pg_length;

        } else {
            it_fl++;
            goal_reached = false;
        }
    }
    /*\*****START PLANNING GRAPH TIME MEASURE*******
    end = std::chrono::system_clock::now();
    t3 += end - start;
     *\******END PLANNING GRAPH TIME MEASURE********/
    if (goal_reached) {
        set_satisfiable(true);
        return;
    } else if (!new_state_insertion) {
        set_satisfiable(false);
        return;
    } else {
        pg_build_grounded();

        // nuova chiamata
    }
}

template <class T>
std::vector<belief_formula> planning_graph<T>::list_bf_classical(unsigned short nesting)
{
	std::vector<belief_formula> ret;
	fluent_set::const_iterator it_fl;

	fluent_set fluents = domain::get_instance().get_fluents();

	for (it_fl = fluents.begin(); it_fl != fluents.end(); it_fl++) {
		/*f*/
		belief_formula pg_c_bf;
		pg_c_bf.set_formula_type(FLUENT_FORMULA);
		pg_c_bf.set_fluent_formula_from_fluent(*it_fl);
		pg_c_bf.set_is_grounded(true);
		ret.push_back(pg_c_bf);
		make_nested_bf_classical(nesting, 0, pg_c_bf, ret);
	}

	return ret;

}

template <class T>
std::list<belief_formula> planning_graph<T>::list_bf_grounded(unsigned short nesting,const std::list<belief_formula> & goal)
{
    std::list<belief_formula> ret;
    fluent_set::const_iterator it_fl;

    //initially

    std::list<belief_formula> initially = domain::get_instance().get_initial_description().get_initial_conditions();
    fluent_set initially_fluent_set = domain::get_instance().get_initial_description().get_initially_known_fluents();

    std::list<belief_formula>::iterator iter_initially;
    for(iter_initially = initially.begin(); iter_initially!=initially.end();iter_initially++)
    {
        //todo inserire initally
        //ret.push_back(&iter_initially);
    }

    fluent_set fluents = domain::get_instance().get_fluents();
    for (it_fl = fluents.begin(); it_fl != fluents.end(); it_fl++) {
        /*f*/
        belief_formula pg_c_bf;
        pg_c_bf.set_formula_type(FLUENT_FORMULA);
        pg_c_bf.set_fluent_formula_from_fluent(*it_fl);
        pg_c_bf.set_is_grounded(true);
        ret.push_back(pg_c_bf);
        make_nested_bf_classical2(nesting, 0, pg_c_bf, ret);
    }

    //goal
    std::list<belief_formula> goals = domain::get_instance().get_goal_description();
    //std::copy (goals_vector.begin(), goals_vector.end(), std::back_inserter(ret));

    std::list<belief_formula>::iterator iter_goals;
    for(iter_goals = goals.begin(); iter_goals!=goals.end();iter_goals++)
    {
        //TODO inserire goals
       // ret.push_back(&goals);
    }

    return ret;

}


template <class T>
void planning_graph<T>::make_nested_bf_classical(unsigned short nesting, unsigned short depth, belief_formula & to_explore, std::vector<belief_formula> & ret)
{
	agent_set::const_iterator it_agset;
	for (it_agset = domain::get_instance().get_agents().begin(); it_agset != domain::get_instance().get_agents().end(); it_agset++) {
		if ((to_explore.get_agent() != *it_agset) || (depth == 0)) {
			belief_formula pg_c_bf;
			/*B(ag, phi)*/
			pg_c_bf.set_formula_type(BELIEF_FORMULA);
			pg_c_bf.set_bf1(to_explore);
			pg_c_bf.set_agent(*it_agset);
			pg_c_bf.set_is_grounded(true);
			ret.push_back(pg_c_bf);

			/*-B(ag, phi)*/
			belief_formula neg_pg_c_bf;
			neg_pg_c_bf.set_formula_type(PROPOSITIONAL_FORMULA);
			neg_pg_c_bf.set_bf1(pg_c_bf);
			neg_pg_c_bf.set_operator(BF_NOT);
			neg_pg_c_bf.set_is_grounded(true);
			/*std::cerr << "\n\nDEBUG: The added bf is: ";
			neg_pg_c_bf.print_deground();*/
			ret.push_back(neg_pg_c_bf);

			if (nesting > (depth + 1)) {
				make_nested_bf_classical(nesting, (depth + 1), pg_c_bf, ret);
				make_nested_bf_classical(nesting, (depth + 1), neg_pg_c_bf, ret);

			}

			/*\todo Add common knowledge etc?*/
		}
	}
}



template <class T>
void planning_graph<T>::make_nested_bf_classical2(unsigned short nesting, unsigned short depth, belief_formula & to_explore, std::list<belief_formula> & ret)
{
    agent_set::const_iterator it_agset;
    for (it_agset = domain::get_instance().get_agents().begin(); it_agset != domain::get_instance().get_agents().end(); it_agset++) {
        if ((to_explore.get_agent() != *it_agset) || (depth == 0)) {
            belief_formula pg_c_bf;
            /*B(ag, phi)*/
            pg_c_bf.set_formula_type(BELIEF_FORMULA);
            pg_c_bf.set_bf1(to_explore);
            pg_c_bf.set_agent(*it_agset);
            pg_c_bf.set_is_grounded(true);
            ret.push_back(pg_c_bf);

            /*-B(ag, phi)*/
            belief_formula neg_pg_c_bf;
            neg_pg_c_bf.set_formula_type(PROPOSITIONAL_FORMULA);
            neg_pg_c_bf.set_bf1(pg_c_bf);
            neg_pg_c_bf.set_operator(BF_NOT);
            neg_pg_c_bf.set_is_grounded(true);
            /*std::cerr << "\n\nDEBUG: The added bf is: ";
            neg_pg_c_bf.print_deground();*/
            ret.push_back(neg_pg_c_bf);

            if (nesting > (depth + 1)) {
                make_nested_bf_classical(nesting, (depth + 1), pg_c_bf, ret);
                make_nested_bf_classical(nesting, (depth + 1), neg_pg_c_bf, ret);

            }

            /*\todo Add common knowledge etc?*/
        }
    }
}

template <class T>
void planning_graph<T>::pg_build_classical(std::vector<belief_formula> & converted_bf)
{
	pg_state_level<T> s_level_curr = m_state_levels.back();
	pg_action_level a_level_curr;
	a_level_curr.set_depth(get_length());

	/*\*****START PLANNING GRAPH TIME MEASURE*******
	auto start = std::chrono::system_clock::now();
	 *\******END PLANNING GRAPH TIME MEASURE********/

	if (m_action_levels.size() > 0) {
		a_level_curr = m_action_levels.back();
	}
	action_set::iterator it_actset;
	for (it_actset = m_never_executed.begin(); it_actset != m_never_executed.end();) {
		if (s_level_curr.pg_executable(*it_actset)) {
			a_level_curr.add_action(*it_actset);
			it_actset = m_never_executed.erase(it_actset);
		} else {
			it_actset++;
		}
	}
	/*\*****START PLANNING GRAPH TIME MEASURE*******
	auto end = std::chrono::system_clock::now();
	t1 += end - start;
	 *\******END PLANNING GRAPH TIME MEASURE********/


	add_action_level(a_level_curr);
	set_length(get_length() + 1);

	//The no-op is done with the copy
	pg_state_level<T> s_level_next = s_level_curr;
	s_level_next.set_depth(get_length());
	bool new_state_insertion = false;
	T tmp_state;

	/*\*****START PLANNING GRAPH TIME MEASURE*******
	start = std::chrono::system_clock::now();
	 *\******END PLANNING GRAPH TIME MEASURE********/

	/*\*****START PLANNING GRAPH TIME MEASURE*******
	std::cerr << "\n\nNew Plan Length: " << get_length();
	if (get_length() > 3) {
		std::cerr << "\nAction Level creation: " << t1.count();
		std::cerr << "\nState Level Creation:  " << t2.count() << " of which:";
		std::cerr << "\nState equality:        " << t4.count();
	}
	 *\******END PLANNING GRAPH TIME MEASURE********/


	for (it_actset = a_level_curr.get_actions().begin(); it_actset != a_level_curr.get_actions().end(); it_actset++) {

		for (typename std::set<T>::const_iterator it_kstset = s_level_curr.get_eStates().begin(); it_kstset != s_level_curr.get_eStates().end(); it_kstset++) {
			tmp_state = *it_kstset;
			//std::cerr << "\nChecking New State";

			/*\*****START PLANNING GRAPH TIME MEASURE*******
			auto startN = std::chrono::system_clock::now();
			bool tmptest = s_level_next.add_eState(tmp_state.compute_succ(*it_actset));
			auto endN = std::chrono::system_clock::now();
			t4 += endN - startN;
			if (tmptest) {
				//				std::cerr << "\nAdded New State";
				new_state_insertion = true;
			} else {
				//				std::cerr << "\nNOT Added New State";

			}
			 *\******END PLANNING GRAPH TIME MEASURE********/

			if (s_level_next.add_eState(tmp_state.compute_succ(*it_actset))) {
				new_state_insertion = true;
			}

		}
	}
	/*\*****START PLANNING GRAPH TIME MEASURE*******
	end = std::chrono::system_clock::now();
	t2 += end - start;
	 *\******END PLANNING GRAPH TIME MEASURE********/


	add_state_level(s_level_next);

	bool goal_reached = true;
	//unsigned short goal_index = 0;


	formula_list::const_iterator it_fl;

	/*\*****START PLANNING GRAPH TIME MEASURE*******
	start = std::chrono::system_clock::now();
	 *\******END PLANNING GRAPH TIME MEASURE********/


	//Remove each sub goal already satisfied: it will always be and we just need to check it once
	for (it_fl = m_goal.begin(); it_fl != m_goal.end();) {
		if (s_level_next.pg_entailment(*it_fl)) {
			it_fl = m_goal.erase(it_fl);
		} else {
			it_fl++;
			goal_reached = false;
		}
	}

	std::vector<belief_formula>::iterator it_vecbf;
	for (it_vecbf = converted_bf.begin(); it_vecbf != converted_bf.end();) {
		if (s_level_next.pg_entailment(*it_vecbf)) {
			m_bfs_score.insert(std::pair<belief_formula, unsigned short>(*it_vecbf, get_length()));
			it_vecbf = converted_bf.erase(it_vecbf);
		} else {
			it_vecbf++;
		}
	}

	/*\*****START PLANNING GRAPH TIME MEASURE*******
	end = std::chrono::system_clock::now();
	t3 += end - start;
	 *\******END PLANNING GRAPH TIME MEASURE********/
	if (goal_reached) {
		set_satisfiable(true);
		return;
	} else if (!new_state_insertion) {
		set_satisfiable(false);
		return;
	} else {
		pg_build_classical(converted_bf);
	}
}

template <class T>
void planning_graph<T>::add_state_level(const pg_state_level<T> & s_level)
{

	m_state_levels.push_back(s_level);
}

template <class T>
void planning_graph<T>::add_action_level(const pg_action_level & a_level)
{

	m_action_levels.push_back(a_level);
}

template <class T>
void planning_graph<T>::set_length(unsigned short length)
{

	m_pg_length = length;
}

template <class T>
void planning_graph<T>::set_sum(unsigned short sum)
{

	m_pg_sum = sum;
}

template <class T>
unsigned short planning_graph<T>::get_length()
{
	if (is_satisfiable()) {
		return m_pg_length;
	}
	std::cerr << "\nThe planning graph could not find any solution. Check for the satisfiability before calling \"get_length\"\n";
	exit(1);
}//construct planning graph and return the level that satisfied the goal.

template <class T>
unsigned short planning_graph<T>::get_sum()
{
	if (is_satisfiable()) {
		return m_pg_sum;
	}
	std::cerr << "\nThe planning graph could not find any solution. Check for the satisfiability before calling \"get_sum\"\n";
	exit(1);
}//

template <class T>
const pg_bfs_score & planning_graph<T>::get_bfs_score()
{
	if (is_satisfiable()) {
		return m_bfs_score;
	}
	std::cerr << "\nThe planning graph could not find any solution. Check for the satisfiability before calling \"get_bfs_score\"\n";
	exit(1);
}//

template <class T>
void planning_graph<T>::set_goal(const formula_list & goal)
{
	m_goal = goal;
}

/*template <class T>
const pg_worlds_score & planning_graph<T>::get_worlds_score()
{
	if (is_satisfiable()) {

		return NULL;
	}
	std::cerr << "\nThe planning graph could not find any solution. Check for the satisfiability before calling \"get_worlds_score\"\n";
	exit(1);
}

template <class T>
const pg_bfs_score & planning_graph<T>::get_bfs_score()
{
	if (is_satisfiable()) {
		return NULL;
	}
	std::cerr << "\nThe planning graph could not find any solution. Check for the satisfiability before calling \"get_bfs_score\"\n";
	exit(1);
} FOR FUTURE USE */