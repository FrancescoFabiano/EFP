/**
 * \class planner
 * 
 * \brief Templatic class that implements the searching technique (BFS).
 *
 * The class is templatic because it searches with different state representation.
 *
 * \todo Templatic with priority queue?.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date May 6, 2019
 */
#pragma once
#include <queue>
#include <stdlib.h>
#include <chrono>
#include <ctime>
#include <sys/wait.h>
#include <pthread.h>
#include <unistd.h>
 

#include "../domain/domain.h"
#include "../heuristics/heuristics_manager.h"
//THIS IS BECAUSE IS A TEMPLATIC CLASS AND IT IS A DEPENDECY
#include "../states/state_T.ipp"

/** \brief Class used as comparator for the priority queue.*/
template <class T>
struct compare_heuristic
{

    bool operator()(const T & state1, const T & state2)
    {
        return state1.get_heuristic_value() > state2.get_heuristic_value();
    }
};

template <class T>
class planner
{
private:
    /**The queue that contains all the states<T> yet to be visited.*/
    std::queue< T > m_search_space;
    std::priority_queue<T, std::vector<T>, compare_heuristic<T> > m_heur_search_space;

    /**Queue for DFS**/
    std::stack<T> m_search_space_DFS;

    /**Function that searches on m_search_space using BFS on all the actions.
     * 
     * @param[in] results_file: if true print the plan time in a file to easy the confrontation with the old version.
     * @return true if a plan is found.
     * @return false otherwise.*/
    bool search_BFS(bool results_file, parallel_type ptype = P_SERIAL);

    //ricerca con DFS
    bool search_DFS(bool results_file, parallel_type ptype = P_SERIAL);

    //ricerca con dfs iterativa che scandisce a livelli la ricerca in profondita, maxDepth è il massimo cammino in profonidtà e step di quanto ad ogni giro incrementiamo
    //i nostri step in profondità
    bool search_IterativeDFS(bool results_file, short maxDepth, short step, parallel_type ptype = P_SERIAL);


    /**Search that generates dataset*/
    bool ML_dataset_creation(ML_Dataset_Params *ML_dataset);
    void append_to_dataset(std::string fpath, T *state, int depth, int score);

    bool dataset_launcher( std::string fpath, int max_depth, bool useDFS);
    int  dataset_DFS_recur(std::string fpath, int max_depth, int depth, T state, bool bisimulation, action_set *actions);
    int  dataset_BFS_recur(std::string fpath, int max_depth, int depth, T state, bool bisimulation, action_set *actions);


    /**Function that launches all heuristic searches within either threads or forked processes
     * 
     * Calls either \ref search_thread(pthread_params *params) or \ref search(bool results_file, parallel_input pin, heuristics used_heur, search_type used_search, short IDFS_d, short IDFS_s)
     * @param[in] results_file: to be passed back to the search function
     * @param[in] pin: structure used to determine if forking or threading are used, and whether child processes are all waited on.
     * @param[in] used_heur: to be passed back to the search function
     * @param[in] used_search: to be passed back to the search function
     * @param[in] IDFS_d: to be passed back to the search function
     * @param[in] IDFS_s: to be passed back to the search function
     * @return true if no issues occured when threading or forking. */
    bool parallel_search(bool results_file, parallel_input pin, heuristics used_heur, search_type used_search, short IDFS_d, short IDFS_s);

    /**thread function which launches search serially for parallelism**/
    // void* my_search_thread(void *args);

    /**Function that searches on m_search_space using Best First Search.
     * 
     * @param[in] results_file: if true print the plan time in a file to easy the confrontation with the old version.
     * @param[in] used_heur: used to determine if any heuristic has to be used and which one.
     * @return true if a plan is found.
     * @return false otherwise.*/
    bool search_heur(bool results_file, heuristics used_heur, parallel_type ptype = P_SERIAL);
    /* \brief The \ref state_type.*/
    //state_type m_state_type;
public:

    /**Function that searches on m_search_space.
     * 
     * Calls either \ref search_BFS(bool results_file) or \ref search_heur(bool results_file) or \ref parallel_search(bool results_file, parallel_input pin, heuristics used_heur, search_type used_search, short IDFS_d, short IDFS_s)
     * @param[in] results_file: if true print the plan time in a file to easy the confrontation with the old version.
     * @param[in] pin: structure used to determine if any parallelism has to be used and whether all threads are waited on.
     * @param[in] used_heur: used to determine if any heuristic has to be used and which one.
     * @param[in] used_search: used to determine the type of search employed by the planner.
     * @param[in] IDFS_d: used as initial "max depth" parameter by I_DFS.
     * @param[in] IDFS_s: used "step" parameter by I_DFS.
     * @return true if a plan is found.*/
    bool search(bool results_file, parallel_input pin, heuristics used_heur, search_type used_search, ML_Dataset_Params generate_heur_ML_data, short IDFS_d, short IDFS_s);

    /**Function print out the solution time.
     * 
     * @param[in] elapsed_seconds: the time of the search.
     * @param[in] expanded_nodes: nodes expanded by the search algorithm.
     * @param[in] goal: the goal state.
     * @param[in] results_file: if true print the plan time in a file to easy the confrontation with the old version.
     * @param[in] givenplan: if true changes the name of the output folder for the comparison time.
     * @param[in] used_search: The type of serach used.
     * @param[in] used_heur: which heuristic has been used.*/
    void print_results(std::chrono::duration<double> elapsed_seconds, int expanded_nodes, T goal, bool results_file, bool givenplan, search_type used_serach, heuristics used_heur = NO_H, parallel_type ptype=P_SERIAL);


    /**Function that searches on m_search_space using the given actions.
     *@param[in] act_name: the names of the \ref action to execute (ordered).*/
    void execute_given_actions(std::vector<std::string>& act_name);

    /**Function that searches on m_search_space using the given actions and print out the execution time.
     * 
     * Every useless I\O step is removed for time accuracy
     * 
     *@param[in] act_name: the names of the \ref action to execute (ordered).*/
    void execute_given_actions_timed(std::vector<std::string>& act_name, parallel_type ptype = P_SERIAL);

    /**Function that checks whether the given actions exist. 
     * 
     * It also removes extra commas between actions
     * 
     *@param[in] act_name: the names of the \ref action to execute (ordered).*/
    void check_actions_names(std::vector<std::string>& act_name);
};