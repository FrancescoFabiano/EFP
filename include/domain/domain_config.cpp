#include "domain_config.h"

void domain_config::set_default_config() {
    debug = false;
    results_file = false;
    global_obsv = true;
    check_visited = false;
    bisimulation = BIS_NONE;
    used_heur = NO_H;
    used_search = BFS;
    s_type = POSSIBILITIES; //default
    ini_restriction = S5; //default
    goal_restriction = NONE; //default
    act_check = EXE_POINTED__COND_WORLD; //default

    input_lang = MAR; // EPDDL;
    update_models = STANDARD;
    models_filename = "";

    execute_given_actions = false;
    kopt = false;

    max_depth = 1;
    step = 1;
}

domain_config::domain_config() {
    set_default_config();
}

bool domain_config::is_debug() const {
    return debug;
}

bool domain_config::is_results_file() const {
    return results_file;
}

bool domain_config::is_global_obsv() const {
    return global_obsv;
}

bool domain_config::is_check_visited() const {
    return check_visited;
}

bis_type domain_config::get_bisimulation() const {
    return bisimulation;
}

heuristics domain_config::get_used_heur() const {
    return used_heur;
}

search_type domain_config::get_used_search() const {
    return used_search;
}

state_type domain_config::get_state_type() const {
    return s_type;
}

domain_restriction domain_config::get_ini_restriction() const {
    return ini_restriction;
}

domain_restriction domain_config::get_goal_restriction() const {
    return goal_restriction;
}

action_check domain_config::get_act_check() const {
    return act_check;
}

spec_lang_type domain_config::get_input_lang() const {
    return input_lang;
}

up_model_type domain_config::get_update_models() const {
    return update_models;
}

const std::string &domain_config::get_models_filename() const {
    return models_filename;
}

bool domain_config::is_execute_given_actions() const {
    return execute_given_actions;
}

bool domain_config::is_kopt() const {
    return kopt;
}

const std::vector<std::string> &domain_config::get_given_actions() const {
    return given_actions;
}

short domain_config::get_max_depth() const {
    return max_depth;
}

short domain_config::get_step() const {
    return step;
}

void domain_config::set_debug(bool set_debug) {
    domain_config::debug = set_debug;
}

void domain_config::set_results_file(bool set_results_file) {
    domain_config::results_file = set_results_file;
}

void domain_config::set_is_global_obsv(bool set_global_obsv) {
    domain_config::global_obsv = set_global_obsv;
}

void domain_config::set_check_visited(bool set_check_visited) {
    domain_config::check_visited = set_check_visited;
}

void domain_config::set_bisimulation(bis_type set_bisimulation) {
    domain_config::bisimulation = set_bisimulation;
}

void domain_config::set_used_heur(heuristics set_used_heur) {
    domain_config::used_heur = set_used_heur;
}

void domain_config::set_used_search(search_type set_used_search) {
    domain_config::used_search = set_used_search;
}

void domain_config::set_state_type(state_type set_state_type) {
    domain_config::s_type = set_state_type;
}

void domain_config::set_ini_restriction(domain_restriction set_ini_restriction) {
    domain_config::ini_restriction = set_ini_restriction;
}

void domain_config::set_goal_restriction(domain_restriction set_goal_restriction) {
    domain_config::goal_restriction = set_goal_restriction;
}

void domain_config::set_act_check(action_check set_act_check) {
    domain_config::act_check = set_act_check;
}

void domain_config::set_input_lang(spec_lang_type set_input_lang) {
    domain_config::input_lang = set_input_lang;
}

void domain_config::set_update_models(up_model_type set_update_models) {
    domain_config::update_models = set_update_models;
}

void domain_config::set_models_filename(const std::string &set_models_filename) {
    domain_config::models_filename = set_models_filename;
}

void domain_config::set_execute_given_actions(bool set_execute_given_actions) {
    domain_config::execute_given_actions = set_execute_given_actions;
}

void domain_config::set_kopt(bool set_kopt) {
    domain_config::kopt = set_kopt;
}

void domain_config::add_given_action(const std::string & action_name) {
    domain_config::given_actions.push_back(action_name);
}

void domain_config::set_max_depth(short set_max_depth) {
    domain_config::max_depth = set_max_depth;
}

void domain_config::set_step(short set_step) {
    domain_config::step = set_step;
}
