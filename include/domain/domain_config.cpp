#include "domain_config.h"

void domain_config::set_default_config() {
    debug = false;
    results_file = false;
    global_obsv = true;
    check_visited = false;
    bisimulation = Bisimulation_Algorithm::NO_BISIMULATION;
    used_heur = Heuristic::NO_H;
    used_search = Search_Strategy::BFS;
    s_type = state_type::POSSIBILITIES;
    domain_logic = logic::KD45;
    mode = Initial_State_Mode::FINITARY_S5_THEORY;
    act_check = action_check::EXE_POINTED__COND_WORLD;

    input_lang = spec_lang_type::MAR; // EPDDL;
    update_models = up_model_type::STANDARD;
    models_filename = "";

    execute_given_actions = false;
    kopt = false;

    iter_dfs_max_depth = 1;
    iter_dfs_step = 1;
}

domain_config::domain_config() {
    build_command_line_options();
    set_default_config();
}

domain_config::domain_config(const boost::shared_ptr<reader> &reader) {
    build_command_line_options();
    set_domain_reader(reader);
    set_default_config();
}

void domain_config::build_command_line_options() {
//    const std::string help_debug = "\tMakes the solving process verbose.";
//    const auto set_debug = [this](const std::string& option_value) {
//        this->set_debug(true);
//        std::cout << "haha" << std::endl;
//    };
//
//    command_line_option debug_option("-d", "--debug", help_debug, set_debug);
//    options.emplace_back(debug_option);
//
//    const std::string help_results_file = "\tPrint the plan time in a file to make the tests confrontations easier.";
//    const auto set_results_file = [this](const std::string& option_value) {
//        this->set_results_file(true);
//    };
//
//    command_line_option results_file_option("", "--results-file", help_results_file, set_results_file);
//    options.emplace_back(results_file_option);
//
//    debug_option.get_config_parameter().operator()((std::string &) "");    // Does this fire lambda?
//
//    const std::string help_global_obsv = "\t";
//    const auto set_global_obsv = [this](const std::string& option_value) {
//        this->set_debug(true);
//        std::cout << "haha" << std::endl;
//    };
//    command_line_option global_obsv_option("-d", "--debug", help_global_obsv, set_global_obsv);

//    global_obsv = true;
//    check_visited = false;
//    bisimulation = bis_type::BIS_NONE;
//    used_heur = heuristics::NO_H;
//    used_search = search_type::BFS;
//    s_type = state_type::POSSIBILITIES;
//    domain_logic = logic::KD45;
//    mode = initial_state_mode::FINITARY_S5_THEORY;
//    act_check = action_check::EXE_POINTED__COND_WORLD;
//
//    input_lang = spec_lang_type::MAR; // EPDDL;
//    update_models = up_model_type::STANDARD;
//    models_filename = "";
//
//    execute_given_actions = false;
//    kopt = false;
//
//    max_depth = 1;
//    step = 1;
}

boost::shared_ptr<reader> domain_config::get_domain_reader() const {
    return domain_reader;
}

std::string domain_config::get_domain_name() const {
    return domain_name;
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

Bisimulation_Algorithm domain_config::get_bisimulation() const {
    return bisimulation;
}

Heuristic domain_config::get_used_heur() const {
    return used_heur;
}

Search_Strategy domain_config::get_used_search() const {
    return used_search;
}

state_type domain_config::get_state_type() const {
    return s_type;
}

logic domain_config::get_domain_logic() const {
    return domain_logic;
}

Initial_State_Mode domain_config::get_initial_state_mode() const {
    return mode;
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

short domain_config::get_iter_dfs_max_depth() const {
    return iter_dfs_max_depth;
}

short domain_config::get_iter_dfs_step() const {
    return iter_dfs_step;
}

void domain_config::set_domain_name(const std::string & to_set) {
    if (domain_name.empty()) {
        domain_name = to_set;
    }
}

void domain_config::set_domain_reader(const boost::shared_ptr<reader> & to_set) {
    domain_config::domain_reader = to_set;
}

void domain_config::set_debug(bool to_set) {
    domain_config::debug = to_set;
}

void domain_config::set_results_file(bool to_set) {
    domain_config::results_file = to_set;
}

void domain_config::set_is_global_obsv(bool to_set) {
    domain_config::global_obsv = to_set;
}

void domain_config::set_check_visited(bool to_set) {
    domain_config::check_visited = to_set;
}

void domain_config::set_bisimulation(Bisimulation_Algorithm to_set) {
    domain_config::bisimulation = to_set;
}

void domain_config::set_used_heur(Heuristic to_set) {
    domain_config::used_heur = to_set;
}

void domain_config::set_used_search(Search_Strategy to_set) {
    domain_config::used_search = to_set;
}

void domain_config::set_state_type(state_type to_set) {
    domain_config::s_type = to_set;
}

void domain_config::set_domain_logic(logic to_set) {
    domain_config::domain_logic = to_set;
}

void domain_config::set_initial_state_mode(Initial_State_Mode to_set) {
    mode = to_set;
}

void domain_config::set_act_check(action_check to_set) {
    domain_config::act_check = to_set;
}

void domain_config::set_input_lang(spec_lang_type to_set) {
    domain_config::input_lang = to_set;
}

void domain_config::set_update_models(up_model_type to_set) {
    domain_config::update_models = to_set;
}

void domain_config::set_models_filename(const std::string &to_set) {
    domain_config::models_filename = to_set;
}

void domain_config::set_execute_given_actions(bool to_set) {
    domain_config::execute_given_actions = to_set;
}

void domain_config::set_kopt(bool to_set) {
    domain_config::kopt = to_set;
}

void domain_config::add_given_action(const std::string & action_name) {
    domain_config::given_actions.push_back(action_name);
}

void domain_config::set_iter_dfs_max_depth(short to_set) {
    domain_config::iter_dfs_max_depth = to_set;
}

void domain_config::set_iter_dfs_step(short to_set) {
    domain_config::iter_dfs_step = to_set;
}
