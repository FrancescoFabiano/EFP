/*
 * \brief The main file.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano, Alessandro Burigana.
 * \date Aprile 1, 2019
 */

#include "cem_parser.h"
#include "../actions/custom_event_models/cevent.h"
#include "../actions/custom_event_models/cem_store.h"

void cem_parser::apply_spaces_regex(std::string & to_clean, const std::regex & pattern)
{

	std::string tmp_cleaned;

	while (std::regex_match(to_clean, pattern)) {
		tmp_cleaned = std::regex_replace(to_clean, pattern, "$1");
		tmp_cleaned += std::regex_replace(to_clean, pattern, "$3");
		to_clean = tmp_cleaned;
	}
}

void cem_parser::parse_conditions(const std::string & line, event_metacond & cond, int line_count)
{

	std::regex spaces_opened("(.*[\\w\\(\\$])(\\s+)([\\w\\(\\$].*)");
	std::regex spaces_closed("(.*[\\w\\)\\$])(\\s+)([\\w\\)\\$].*)");
	std::regex negation("not\\(([^\\s]+)\\)");

	//std::cout << std::endl << "\nOriginal string is " << line << std::endl;

	std::string cleaned = line;

	apply_spaces_regex(cleaned, spaces_opened);
	apply_spaces_regex(cleaned, spaces_closed);

	boost::replace_all(cleaned, ")(", ") (");


	std::istringstream args(cleaned);
	std::string sub_arg;

	bool is_negated;


	while (args >> sub_arg) {
		is_negated = false;
		sub_arg = std::regex_replace(sub_arg, std::regex("\\(([^\\s]+)\\)"), "$1");
		if (std::regex_match(sub_arg, negation)) {
			is_negated = true;
			sub_arg = std::regex_replace(sub_arg, negation, "$1");
		}

		if (sub_arg.compare("$act_pre$") == 0) {

			if (is_negated) {
				cond.insert(neg_act_pre);
			} else {
				cond.insert(act_pre);
			}


		} else if (sub_arg.compare("$act_eff$") == 0) {
			if (is_negated) {
				cond.insert(neg_act_eff);
			} else {
				cond.insert(act_eff);
			}
		} else if (sub_arg.compare("none") == 0) {
			cond.clear();
			cond.insert(none);
			return;
			//std::cout << "\n\t\t" << sub_arg;
		} else {
			//Here parse of normal fluent/Belief formulae to add as extra field in the event
			std::cerr << "\nParsing Error at Line " << line_count << ": The specified condition has no meaning. Please use \'act_eff\', \'act_pre\', or \'none\'.\n";
			exit(1);
		}
	}
}

void cem_parser::parse_ag_list(const std::string & line)
{

	std::string cleaned = line;

	boost::replace_all(cleaned, ",", ";");

	std::regex spaces_int("(.*\\;)(\\s+)(\\w.*)");
	std::regex spaces_ext("(.*\\w)(\\s+)(\\;.*)");

	//std::cout << std::endl << "\nOriginal string is " << line << std::endl;


	apply_spaces_regex(cleaned, spaces_int);
	apply_spaces_regex(cleaned, spaces_ext);

	boost::replace_all(cleaned, ";", " ");
	boost::replace_all(cleaned, ")(", ") (");



	std::istringstream args(cleaned);
	std::string sub_arg;



	while (args >> sub_arg) {
		cem_store::get_instance().add_agent_group(sub_arg);
	}

}

void cem_parser::parse_edge(const std::string & edge, cem_edges & edges)
{

	std::string cleaned = edge;

	boost::replace_all(cleaned, ";", " ");

	std::istringstream args(cleaned);
	std::string sub_arg;

	args >> sub_arg;
	cevent_ptr first = cem_store::get_instance().get_event(cem_store::get_instance().get_event_id(sub_arg));


	args >> sub_arg;
	cevent_ptr second = cem_store::get_instance().get_event(cem_store::get_instance().get_event_id(sub_arg));

	args >> sub_arg;
	agent_group_id e_to_add_ag = cem_store::get_instance().get_agent_group(sub_arg);


	auto it_eve = edges.find(first);

	if (it_eve != edges.end()) {
		auto it_evm = it_eve->second.find(e_to_add_ag);

		if (it_evm != it_eve->second.end()) {
			it_evm->second.insert(second);
		} else {
			it_eve->second.insert(cevent_map::value_type(e_to_add_ag,{second}));

		}
	} else {
		cevent_map tmp;
		tmp.insert(cevent_map::value_type(e_to_add_ag,{second}));
		edges.insert(cem_edges::value_type(first, tmp));
	}
	//	if (edges.find(e_to_add_ag) != edges.end()) {
	//		edges[e_to_add_ag].insert(e_to_add);
	//	} else {
	//		std::set<cem_edge> tmp_set;
	//		tmp_set.insert(e_to_add);
	//		edges.insert(std::pair<agent_group_id, std::set<cem_edge> >(e_to_add_ag, tmp_set));
	//	}
}

void cem_parser::parse_edges_list(const std::string & line, cem_edges & edges)
{
	std::string cleaned = line;

	boost::replace_all(cleaned, ",", ";");

	std::regex spaces_opened("(.*[\\w\\(\\;])(\\s+)([\\w\\(\\;].*)");
	std::regex spaces_closed("(.*[\\w\\)\\;])(\\s+)([\\w\\)\\;].*)");

	//std::cout << std::endl << "\nOriginal string is " << line << std::endl;


	apply_spaces_regex(cleaned, spaces_opened);
	apply_spaces_regex(cleaned, spaces_closed);
	boost::replace_all(cleaned, ")(", ") (");


	//boost::replace_all(cleaned, ";", " ");


	std::istringstream args(cleaned);
	std::string sub_arg;

	while (args >> sub_arg) {
		sub_arg = std::regex_replace(sub_arg, std::regex("\\(([^\\s]+)\\)"), "$1");
		parse_edge(sub_arg, edges);
	}
}

void cem_parser::parse(const std::string & filename)
{
	//std::cout << "\nTesting the generation of Event Models starting from the file: " << filename << std::endl;
	std::ifstream cem_file(filename);


	if (cem_file.is_open()) {

		bool in_event = false, in_model = false, not_assigned = true;
		std::string line;
		std::string type, field, internal_field;


		std::regex indentation("(\\s+)(.*)");
		std::regex comment("([^*]*)(\\*+)(.*)");

		std::regex type_regex("(\\()(\\s*)([^\\s]+)(\\s*)(\\:)(\\s*)(.*)");
		std::regex field_regex("(\\:)(\\s*)([^\\s]+)(\\s+)([\\(\\{])(.+)([\\)\\}])(.*)");


		event_id e_id;
		event_metacond e_meta_pre;
		event_metacond e_meta_post;
		cevent e_to_add;

		cem_id p_id;
		event_id p_pointed_id;
		cem_edges p_edges;
		cem c_to_add;


		int line_count = 0;

		while (getline(cem_file, line)) {

			++line_count;

			line = std::regex_replace(line, indentation, "$2");
			line = std::regex_replace(line, comment, "$1");
			boost::to_lower(line);

			if (std::regex_match(line, type_regex)) {

				type = std::regex_replace(line, type_regex, "$3");

				if (!not_assigned) {
					std::cerr << "\nParsing Error at Line " << line_count << ": Before declaring a new " << type << " you need to finish the previous type (Maybe you are missing a parenthesis?).\n";
					exit(1);
				}

				if (type.compare("event") == 0) {
					if (!not_assigned) {
						std::cerr << "\nParsing Error at Line " << line_count << ": Trying to define an \'event\' while already defining an \'event\' or a \'model\'.\n";
					}

					e_id = -1;
					e_meta_pre.clear();
					e_meta_post.clear();

					in_event = true;
					not_assigned = false;

				} else if (type.compare("model") == 0) {

					if (!not_assigned) {
						std::cerr << "\nParsing Error at Line " << line_count << ": Trying to define a \'model\' while already defining an \'event\' or a \'model\'.\n";

					}

					p_id = -1;
					p_pointed_id = -1;
					p_edges.clear();


					in_model = true;
					not_assigned = false;
				} else if (type.compare("obs_groups") == 0) {
					if (!not_assigned) {
						std::cerr << "\nParsing Error at Line " << line_count << ": Trying to define the \'observability groups\' while already defining an \'event\' or a \'model\'.\n";

					}
					std::string obs = std::regex_replace(line, type_regex, "$7");

					parse_ag_list(std::regex_replace(obs, std::regex("\\{(.*)\\}(.*)"), "$1"));

				} else {
					std::cerr << "\nParsing Error at Line " << line_count << ": The specified type -- " << type << " -- is not defined in our syntax. Please use either \'event\', \'model\', or \'obs_groups\'.\n";
					exit(1);
				}

			} else if (std::regex_match(line, std::regex("(\\()(.)+(\\:)(\\s*)(\\S)+"))) {
				std::cerr << "\nParsing Error at Line " << line_count << ": The syntax requires a newline before the definition of a new field.\n";
				exit(1);
			} else if (std::regex_match(line, field_regex)) {

				field = std::regex_replace(line, field_regex, "$3");
				internal_field = std::regex_replace(line, field_regex, "$6");

				if (not_assigned) {
					std::cerr << "\nParsing Error at Line " << line_count << ": Before declaring a new \'" << field << "\' you need to open a new \'event\' or \'model\'.\n";
					exit(1);
				}

				if (field.compare("id") == 0 && (e_id == -1 || p_id == -1)) {

					if (in_event) {
						e_id = cem_store::get_instance().get_event_id(internal_field);
					} else if (in_model) {
						p_id = cem_store::get_instance().get_cem_id(internal_field);
					} else {
						std::cerr << "\nParsing Error at Line " << line_count << ": Before declaring a new \'" << field << "\' you need to open a new \'event\' or \'model\'.\n";
						exit(1);
					}
				} else if (field.compare("pointed") == 0) {
					if (!in_model) {
						std::cerr << "\nParsing Error at Line " << line_count << ": Pointed event found outside of a model definition.";
						exit(1);
					}
					p_pointed_id = cem_store::get_instance().get_event_id(internal_field);
				} else if (field.compare("precondition") == 0) {
					if (!in_event) {
						std::cerr << "\nParsing Error at Line " << line_count << ": Precondition found outside of an event definition.";
						exit(1);
					}
					parse_conditions(internal_field, e_meta_pre, line_count);
				} else if (field.compare("postcondition") == 0) {
					if (!in_event) {
						std::cerr << "\nParsing Error at Line " << line_count << ": Postcondition found outside of an event definition.";
						exit(1);
					}

					parse_conditions(internal_field, e_meta_post, line_count);
				} else if (field.compare("events") == 0) {
					if (!in_model) {
						std::cerr << "\nParsing Error at Line " << line_count << ": Events found outside of a model definition.";
						exit(1);
					}
					//	std::cout << "\n\tWith events: ";
					//	parse_list(internal_field);

					//Add here something as check

				} else if (field.compare("edges") == 0) {
					if (!in_model) {
						std::cerr << "\nParsing Error at Line " << line_count << ": Edges found outside of a model definition.";
						exit(1);
					}
					parse_edges_list(internal_field, p_edges);

					//std::cout << field << " -> " << internal_field << std::endl;
				} else {
					std::cerr << "\nParsing Error at Line " << line_count << ": The specified field -- " << field << " -- is not properly defined in our syntax (watch out for repetition). Please use \'id\', \'precondition\', \'postcondition\', \'events\', or \'edges\'.\n";
					exit(1);
				}
			} else if (std::regex_match(line, std::regex("(\\))(\\s*)"))) {

				if (in_event) {
					e_to_add.set_id(e_id);
					e_to_add.set_meta_precondition(e_meta_pre);
					e_to_add.set_meta_postconditions(e_meta_post);

					cem_store::get_instance().add_event(e_to_add);
					//std::cerr << "\nDEBUG: Event is ";
					//e_to_add.print();
				} else if (in_model) {
					c_to_add.set_id(p_id);
					c_to_add.set_pointed_id(p_pointed_id);
					c_to_add.set_edges(p_edges);

					cem_store::get_instance().add_cem(c_to_add);
					//std::cerr << "\nDEBUG: Model is ";
					//c_to_add.print();
				}
				in_event = false;
				in_model = false;
				not_assigned = true;

			} else if (line.size() > 0) {
				std::cerr << "\nParsing Error at Line " << line_count << ": the syntax in \'" << line << "\' is not correct.\n";
				exit(1);

			}
		}
	} else {
		std::cerr << "\nParsing Error: It is impossible to open the file \'" << filename << "\'.";
		exit(1);
	}
	cem_file.close();
}

