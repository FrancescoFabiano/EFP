/*
 * \brief The main file.
 *
 * \copyright GNU Public License.
 *
 * \author Francesco Fabiano.
 * \date Aprile 1, 2019
 */

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include <regex>
#include "../../include/delphic/pem.h"

//Leggi il pointed ---- Singolo

void apply_spaces_regex(std::string & to_clean, const std::regex & pattern)
{

	std::string tmp_cleaned;

	while (std::regex_match(to_clean, pattern)) {
		tmp_cleaned = std::regex_replace(to_clean, pattern, "$1");
		tmp_cleaned += std::regex_replace(to_clean, pattern, "$3");
		to_clean = tmp_cleaned;
	}
}

void parse_conditions(const std::string & line)
{

	std::regex spaces_opened("(.*[\\w\\(\\$])(\\s+)([\\w\\(\\$].*)");
	std::regex spaces_closed("(.*[\\w\\)\\$])(\\s+)([\\w\\)\\$].*)");
	std::regex negation("not\\(([^\\s]+)\\)");

	//std::cout << std::endl << "\nOriginal string is " << line << std::endl;

	std::string cleaned = line;

	apply_spaces_regex(cleaned, spaces_opened);
	apply_spaces_regex(cleaned, spaces_closed);

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
			std::cout << "\n\t\t" << sub_arg << " is negated: " << is_negated;
		} else if (sub_arg.compare("$act_eff$") == 0) {
			std::cout << "\n\t\t" << sub_arg << " is negated: " << is_negated;
		} else if (sub_arg.compare("none") == 0) {
			std::cout << "\n\t\t" << sub_arg;
		} else {
			std::cerr << "\nParsing Error: The specified condition has no meaning. Please use \'act_eff\', \'act_pre\', or \'none\'.\n";
			exit(1);
		}
	}
}

void parse_list(const std::string & line)
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

	args >> sub_arg;
	std::cout << "\n\t\t" << sub_arg;
	while (args >> sub_arg) {
		std::cout << "; " << sub_arg;
	}

}

void parse_edge(const std::string & edge)
{
	std::string cleaned = edge;

	boost::replace_all(cleaned, ";", " ");

	std::istringstream args(cleaned);
	std::string sub_arg;

	args >> sub_arg;
	std::cout << "\n\t\t" << sub_arg;
	while (args >> sub_arg) {
		std::cout << "-" << sub_arg;
	}
}

void parse_edges_list(const std::string & line)
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
		parse_edge(sub_arg);
	}
}

int main(int argc, char** argv)
{
	std::string filename = "custom_pem.txt";
	std::cout << "\nTesting the generation of Event Models starting from the file: " << filename << std::endl;
	std::ifstream pem_file(filename);


	if (pem_file.is_open()) {

		bool in_event = false, in_model = false, not_assigned = true;
		std::string line;
		std::string type, field, internal_field;


		std::regex indentation("(\\s+)(.*)");
		std::regex comment("([^*]*)(\\*+)(.*)");

		std::regex type_regex("(\\()(\\s*)([^\\s]+)(\\s*)(\\:)(\\s*)(.*)");
		std::regex field_regex("(\\:)(\\s*)([^\\s]+)(\\s+)([\\(\\{])(.+)([\\)\\}])(.*)");


		while (getline(pem_file, line)) {

			line = std::regex_replace(line, indentation, "$2");
			line = std::regex_replace(line, comment, "$1");

			if (std::regex_match(line, type_regex)) {

				type = std::regex_replace(line, type_regex, "$3");

				if (!not_assigned) {
					std::cerr << "\nParsing Error: Before declaring a new " << type << " you need to finish the previous type (Maybe you are missing a parenthesis?).\n";
					exit(1);
				}

				if (type.compare("event") == 0) {
					if (!not_assigned) {
						std::cerr << "\nParsing Error: Trying to define an \'event\' while already defining an \'event\' or a \'model\'.\n";

					}

					in_event = true;
					not_assigned = false;
					std::cout << "\nDefining new Event";
				} else if (type.compare("model") == 0) {

					if (!not_assigned) {
						std::cerr << "\nParsing Error: Trying to define a \'model\' while already defining an \'event\' or a \'model\'.\n";

					}
					in_model = true;
					not_assigned = false;
					std::cout << "\nDefining new Model";
				} else if (type.compare("obs_groups") == 0) {
					if (!not_assigned) {
						std::cerr << "\nParsing Error: Trying to define the \'observability groups\' while already defining an \'event\' or a \'model\'.\n";

					}
					std::cout << "\nDefining the Observabilty groups";
					std::string obs = std::regex_replace(line, type_regex, "$7");

					parse_list(std::regex_replace(obs, std::regex("\\{(.*)\\}(.*)"), "$1"));
					std::cout << "\nDone Defining\n\n";

				} else {
					std::cerr << "\nParsing Error: The specified type -- " << type << " -- is not defined in our syntax. Please use either \'event\', \'model\', or \'obs_groups\'.\n";
					exit(1);
				}

			} else if (std::regex_match(line, std::regex("(\\()(.)+(\\:)(\\s*)(\\S)+"))) {
				std::cerr << "\nParsing Error: The syntax requires a newline before the definition of a new field.\n";
				exit(1);
			} else if (std::regex_match(line, field_regex)) {

				field = std::regex_replace(line, field_regex, "$3");
				internal_field = std::regex_replace(line, field_regex, "$6");

				if (not_assigned) {
					std::cerr << "\nParsing Error: Before declaring a new \'" << field << "\' you need to open a new \'event\' or \'model\'.\n";
					exit(1);
				}

				if (field.compare("id") == 0) {
					std::cout << "\n\tId is: " << internal_field;
				} else if (field.compare("pointed") == 0) {
					if (!in_model) {
						std::cerr << "\nParsing Error: Pointed event found outside of a model definition.";
						exit(1);
					}
					std::cout << "\n\tPointed is: " << internal_field;
				}
				else if (field.compare("precondition") == 0) {
					if (!in_event) {
						std::cerr << "\nParsing Error: Precondition found outside of an event definition.";
						exit(1);
					}
					std::cout << "\n\tPreconditions are: ";
					parse_conditions(internal_field);
					//std::cout << field << " -> " << internal_field << std::endl;
				} else if (field.compare("postcondition") == 0) {
					if (!in_event) {
						std::cerr << "\nParsing Error: Postcondition found outside of an event definition.";
						exit(1);
					}
					std::cout << "\n\tPostconditions are: ";

					parse_conditions(internal_field);
					//std::cout << field << " -> " << internal_field << std::endl;
				} else if (field.compare("events") == 0) {
					if (!in_model) {
						std::cerr << "\nParsing Error: Events found outside of a model definition.";
						exit(1);
					}
					std::cout << "\n\tWith events: ";
					parse_list(internal_field);
					//std::cout << field << " -> " << internal_field << std::endl;
				} else if (field.compare("edges") == 0) {
					if (!in_model) {
						std::cerr << "\nParsing Error: Edges found outside of a model definition.";
						exit(1);
					}
					std::cout << "\n\tWith edges: ";
					parse_edges_list(internal_field);

					//std::cout << field << " -> " << internal_field << std::endl;
				} else {
					std::cerr << "\nParsing Error: The specified field -- " << field << " -- is not defined in our syntax. Please use \'id\', \'precondition\', \'postcondition\', \'events\', or \'edges\'.\n";
					exit(1);
				}
			} else if (std::regex_match(line, std::regex("(\\))(\\s*)"))) {
				in_event = false;
				in_model = false;
				not_assigned = true;
				std::cout << "\nDone Defining\n\n";
			} else if (line.size() > 0) {
				std::cerr << "\nParsing Error: the syntax in \'" << line << "\' is not correct.\n";
				exit(1);

			}
		}
	} else {
		std::cerr << "\nParsing Error: It is impossible to open the file \'" << filename << "\'.";
		exit(1);
	}
	pem_file.close();

	exit(0);
}

