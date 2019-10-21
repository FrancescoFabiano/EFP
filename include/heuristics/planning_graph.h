/**
 * \class planning_graph
 * \brief Class that implements the epistemic planning graph data structure for heuristics extrapolation.
 *
 * \details An e-Planning graph is a structure introduced in <https://aaai.org/ocs/index.php/ICAPS/ICAPS18/paper/view/17733>
 *  that, as the planning graph from the classical planning environment, it is used to extrapolate
 * qualitative value for the states.
 * 
 *
 * \copyright GNU Public License.
 * 
 * \author Francesco Fabiano.
 * \date September 24, 2019
 */

#pragma once

class planning_graph
{
protected:
    Planner* m_planner;
    PGraphENodeLevel k_levels; //for union of \K_i levels for the sake of saving memory
    vector<PGraphActionLevel> a_levels; //for A_i levels

private:
    EpistemicModelsNode effect_sensing(Literal l, AgentSet fd, AgentSet pd, AgentSet od, string action);
    EpistemicModelsNode effect_ontic(Literal l, AgentSet fd, AgentSet od, string action);
    EpistemicModelsNode effect_announce(FluentFormula ff, AgentSet fd, AgentSet pd, AgentSet od, string action);
    void cartesian_product(list< vector<Literal> > sets, list< vector<Literal> >& product);

public:
    PlanningGraph(Planner* planner);
    ~PlanningGraph();

    void add_K_level(PGraphENodeLevel);
    void add_A_level(PGraphActionLevel);

    int constructEPG(Kripke*); //construct planning graph and return the its level satisfied the goal.

    //reimplement these methods to get fd, pd, od, without respect to a kripke structure because we do not consider ``if" in a observes and aware_of statement
    AgentSet get_fd(const KAction& act) const;
    AgentSet get_pd(const KAction& act) const;
    AgentSet get_od(const KAction& act) const;

    //printing
    void print_info();

};
