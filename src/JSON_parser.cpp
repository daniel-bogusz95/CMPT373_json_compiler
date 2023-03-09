#include <fstream>
#include <iostream>
#include <map>
#include <iomanip>
#include "include/RSJparser.tcc"

//JSON parser:
//https://github.com/subh83/RSJp-cpp

json parse_json(char** file_path)
{     	 	
	std::ifstream file(file_path);
	if (!file) {
		std::cerr << "unable to open file: " << file_path << std::endl;
		return NULL;
	}

	RSJresource json(file);
	return json;
}

//note: must keep all data from JSON files in the form of maps so that they are key indexible
//	ie. the JSON rules are written to reference variables by key, so our rule functions must reference variables like: json_object['variables'][rule_specified_variable_key]
