#ifndef EFP_DOMAIN_CONFIG_H
#define EFP_DOMAIN_CONFIG_H

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "../../include/definitions/domain_config_def.h"
#include "../../include/definitions/domain_def.h"
//#include "../utilities/reader.h"

class domain_config {
private:
    /**\brief The given to the *this*. (The name of the file that contains the description of *this*)*/
    std::string domain_name;
    bool debug;
    bool results_file;
    bool global_obsv;
    bool check_visited;
    Bisimulation_Algorithm bisimulation;
    Heuristic used_heur;
    Search_Strategy used_search;
    state_type s_type;
    logic domain_logic;
    Initial_State_Mode mode;
    action_check act_check;

    spec_lang_type input_lang;
    up_model_type update_models;
    std::string models_filename;

    bool execute_given_actions;
    bool kopt;
    std::vector<std::string> given_actions;

    int iter_dfs_max_depth;
    int iter_dfs_step;

    void build_command_line_options();

    void set_default_config();

public:
    domain_config();

    std::string get_domain_name() const;

    bool is_debug() const;

    bool is_results_file() const;

    bool is_global_obsv() const;

    bool is_check_visited() const;

    Bisimulation_Algorithm get_bisimulation() const;

    Heuristic get_used_heur() const;

    Search_Strategy get_used_search() const;

    state_type get_state_type() const;

    logic get_domain_logic() const;

    Initial_State_Mode get_initial_state_mode() const;

    action_check get_act_check() const;

    spec_lang_type get_input_lang() const;

    up_model_type get_update_models() const;

    const std::string &get_models_filename() const;

    bool is_execute_given_actions() const;

    bool is_kopt() const;

    const std::vector<std::string> &get_given_actions() const;

    int get_iter_dfs_max_depth() const;

    int get_iter_dfs_step() const;

    void set_domain_name(const std::string & to_set);

    void set_debug(bool to_set);

    void set_results_file(bool to_set);

    void set_is_global_obsv(bool to_set);

    void set_check_visited(bool to_set);

    void set_bisimulation(Bisimulation_Algorithm to_set);

    void set_used_heur(Heuristic to_set);

    void set_used_search(Search_Strategy to_set);

    void set_state_type(state_type to_set);

    void set_domain_logic(logic to_set);

    void set_initial_state_mode(Initial_State_Mode to_set);

    void set_act_check(action_check to_set);

    void set_input_lang(spec_lang_type to_set);

    void set_update_models(up_model_type to_set);

    void set_models_filename(const std::string &models_filename);

    void set_execute_given_actions(bool to_set);

    void set_kopt(bool to_set);

    void add_given_action(const std::string & action_name);

    void set_iter_dfs_max_depth(int to_set);

    void set_iter_dfs_step(int to_set);
};


#endif //EFP_DOMAIN_CONFIG_DEF_H
