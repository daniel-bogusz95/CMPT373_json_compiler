#include <main.h>

#include "RSJparser.tcc"
#include "JSONDeserialized.h"
#include "RSJparser.tcc"
#include "rules.h"
#include "RuleHelpers.h"
#include "server.h"
#include "gui.h" //TODO not sure if CMake is set up for this

#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include <random>
#include <string_view>
#include <functional>
#include <filesystem>

enum class Box {
	Configuration,
	Constants,
	Variables,
	PerPlayer,
	PerAudience,
	Rule
};

//code adapted from: https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
bool stringIsNum(const std::string & s) {
	!s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

//TODO handle recursion of lists
void GetValueMap(Box & box, const std::string & s) {
	//TODO handle recursion of lists
		//call GetVariable
}

void GetVariable(Box & box, const std::string & key, const std::string & s) {
	if (s == "true") {
		box.valueMap.insert(std::pair<std::string, variableVariants>(key, true)); 
		return;
	}
	else if (s == "false"){
		box.valueMap.insert(std::pair<std::string, variableVariants>(key, false)); 
		return;
	}
	else if (stringIsNum(s)){
		box.valueMap.insert(std::pair<std::string, variableVariants>(key, std::stof(s))); 
		return;
	}
	testVars.valueMap.insert(std::pair<std::string, variableVariants>(key, s)); 
	return;
}


//function adapted from https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
std::vector <std::string> GetGames() {
	std::vector <std::string> vec;
	const std::string game_folder = "../games/";
	for (const auto & entry : std::filesystem::directory_iterator(game_folder))
        	vec.push_back(entry.path());
        return vec;
}

Configuration GetConfiguration(RSJson json) {
	RSJson json_config(json["configuration"]);
	std::map<std::string, variableVariants> setup = {};//TODO how to create variableVarient?
	return Configuration {
		json_config["name"].as<std::string>("ERROR"),
		json_config["player_count"]["min"].as<size_t>("ERROR"),
		json_config["player_count"]["max"].as<size_t>("ERROR"),
		json_config["audience"].as<bool>("ERROR"),
		setup
	};
}

Constants GetConstants(RSJson json) {
	RSJson json_constants(json["constants"]);
	Constants constants;
	std::map<std::string, variableVariants> valueMap = GetValueMap();
	return constants;
}

Variables GetVariables(RSJson json) {
	RSJson json_constants(json["variables"]);
	Variables variables;
	std::map<std::string, variableVariants> valueMap = GetValueMap();
	return variables;
}

PerPlayer GetPerPlayer(RSJson json) {
	RSJson json_constants(json["per-player"]);
	PerPlayer perPlayer;
	std::map<std::string, variableVariants> valueMap = GetValueMap();
	return perPlayer;
}

PerAudience GetPerAudience(RSJson json) {
	RSJson json_constants(json["per-audience"]);
	PerAudience perAudience;
	std::map<std::string, variableVariants> valueMap = GetValueMap();
	return perAudience;
}

//note: because we want to return a vector of rules, using a helper function before recursion is the easiest way
std::vector<Rule> GetRules(RSJson json) {
	RSJson json_rules(json["rules"]);
	std::vector<Rule> rules;
	for (auto & rule_json : json_rules) {
		rules.push_back(rule_json);
	}
	return rules;
}

Rule GetRule(RSJson json_rule) {
	Rule rule;
	std::vector<Rule> rulesList;
	for (auto & sub_rule_json : json_rule["rules"]) {
		rulesList.push_back(GetRule(rule, sub_rule_json));
		rulesList.back().parent = &rule;
	}
	std::map<std::string, variableVariants> valueMap = GetValueMap();
	Rule rule {
		RuleNameMap[json_rules["name"].as<std::string>("ERROR")],
		nullptr,
		valueMap,
		rulesList
	};
	return rule;
}

void main() {
	while (true) {
		std::vector <std::string> game_paths = GetGames(); //get list of game files in game folder
		//TODO std::string game_string = GUIGameSelect(game_paths); //this could just be a choice messgae? would that work if sent to server instead of client? 
		std::string game_path = "display_test.json";//TODO this is only for testing, remove after creating GUI for game selection
		RSJson json = parse_json(game_path);
		Configuration configuration = GetConfiguration();
		Constants constants = GetConstants(json);
		Variables variables = GetVariables(json);
		PerPlayer perPlayer = GetPerPlayer(json);
		PerAudience perAudience = GetPerAudience(json);
		std::vector<Rule> rules = GetRules(json);
		Game game = Game(constants, configuration, variables, perPlayer, perAudience, rules);
		//TODO create server
		//TODO wait for appropriate number of players to join, 
		//TODO GUILobby(); //give option to start game
		//TODO limit number of players to max, remove extras/prevent from joining
		game.startGame();
		continue; //game ends, start loop over
	}
}
