#ifndef EFP_DOMAIN_CONFIG_H
#define EFP_DOMAIN_CONFIG_H

#include "../utilities/define.h"

class domain_config {
private:
    bool debug;
    bool results_file;
    bool global_obsv;
    bool check_visited;
    bis_type bisimulation;
    heuristics used_heur;
    search_type used_search;
    state_type s_type;
    domain_restriction ini_restriction;
    domain_restriction goal_restriction;
    action_check act_check;

    spec_lang_type input_lang;
    up_model_type update_models;
    std::string models_filename;

    bool execute_given_actions;
    bool kopt;
    std::vector<std::string> given_actions;

    short max_depth;
    short step;

    void set_default_config();

public:
    domain_config();

    bool is_debug() const;

    bool is_results_file() const;

    bool is_global_obsv() const;

    bool is_check_visited() const;

    bis_type get_bisimulation() const;

    heuristics get_used_heur() const;

    search_type get_used_search() const;

    state_type get_state_type() const;

    domain_restriction get_ini_restriction() const;

    domain_restriction get_goal_restriction() const;

    action_check get_act_check() const;

    spec_lang_type get_input_lang() const;

    up_model_type get_update_models() const;

    const std::string &get_models_filename() const;

    bool is_execute_given_actions() const;

    bool is_kopt() const;

    const std::vector<std::string> &get_given_actions() const;

    short get_max_depth() const;

    short get_step() const;

    void set_debug(bool set_debug);

    void set_results_file(bool set_results_file);

    void set_is_global_obsv(bool set_global_obsv);

    void set_check_visited(bool set_check_visited);

    void set_bisimulation(bis_type set_bisimulation);

    void set_used_heur(heuristics set_used_heur);

    void set_used_search(search_type set_used_search);

    void set_state_type(state_type set_state_type);

    void set_ini_restriction(domain_restriction set_ini_restriction);

    void set_goal_restriction(domain_restriction set_goal_restriction);

    void set_act_check(action_check set_act_check);

    void set_input_lang(spec_lang_type set_input_lang);

    void set_update_models(up_model_type set_update_models);

    void set_models_filename(const std::string &models_filename);

    void set_execute_given_actions(bool set_execute_given_actions);

    void set_kopt(bool set_kopt);

    void add_given_action(const std::string & action_name);

    void set_max_depth(short set_max_depth);

    void set_step(short set_step);

};


#endif //EFP_DOMAIN_CONFIG_H