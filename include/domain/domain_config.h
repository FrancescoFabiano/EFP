#ifndef EFP_DOMAIN_CONFIG_H
#define EFP_DOMAIN_CONFIG_H

#include <boost/shared_ptr.hpp>
#include "../utilities/define.h"
#include "../utilities/reader.h"

class domain_config {
private:
    boost::shared_ptr<reader> domain_reader;

    /**\brief The given to the *this*. (The name of the file that contains the description of *this*)*/
    std::string domain_name;
    bool debug;
    bool results_file;
    bool global_obsv;
    bool check_visited;
    bis_type bisimulation;
    heuristics used_heur;
    search_type used_search;
    state_type s_type;
    logic domain_logic;
    initial_state_mode mode;
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

    void set_domain_reader(const boost::shared_ptr<reader> & to_set);

public:
    domain_config();

    domain_config(const boost::shared_ptr<reader> &reader);

    boost::shared_ptr<reader> get_domain_reader() const;

    std::string get_domain_name() const;

    bool is_debug() const;

    bool is_results_file() const;

    bool is_global_obsv() const;

    bool is_check_visited() const;

    bis_type get_bisimulation() const;

    heuristics get_used_heur() const;

    search_type get_used_search() const;

    state_type get_state_type() const;

    logic get_domain_logic() const;

    initial_state_mode get_initial_state_mode() const;

    action_check get_act_check() const;

    spec_lang_type get_input_lang() const;

    up_model_type get_update_models() const;

    const std::string &get_models_filename() const;

    bool is_execute_given_actions() const;

    bool is_kopt() const;

    const std::vector<std::string> &get_given_actions() const;

    short get_max_depth() const;

    short get_step() const;

    void set_domain_name(const std::string & to_set);

    void set_debug(bool to_set);

    void set_results_file(bool to_set);

    void set_is_global_obsv(bool to_set);

    void set_check_visited(bool to_set);

    void set_bisimulation(bis_type to_set);

    void set_used_heur(heuristics to_set);

    void set_used_search(search_type to_set);

    void set_state_type(state_type to_set);

    void set_domain_logic(logic to_set);

    void set_initial_state_mode(initial_state_mode to_set);

    void set_act_check(action_check to_set);

    void set_input_lang(spec_lang_type to_set);

    void set_update_models(up_model_type to_set);

    void set_models_filename(const std::string &models_filename);

    void set_execute_given_actions(bool to_set);

    void set_kopt(bool to_set);

    void add_given_action(const std::string & action_name);

    void set_max_depth(short to_set);

    void set_step(short to_set);
};


#endif //EFP_DOMAIN_CONFIG_H
