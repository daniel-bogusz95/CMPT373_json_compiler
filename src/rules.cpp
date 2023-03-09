#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include <random>
#include <string_view>
#include <functional>

#include "RSJparser.tcc"
#include "rules.h"
#include "RuleHelpers.h"
// #include "server.h"

/*
Returns a method signature to the next rule from a passed in
argument of an enum representing the name of a rule.
*/
std::function<void(Rule&, Configuration&, Constants&, Variables&, PerPlayer&, PerAudience&)> RuleMethods::nextRule(RuleName rule_name) {
    return RuleMethods::ruleNameToFunc.at(rule_name);
}

// Root assumes that the value passed in is the entire root
// and rule is root for the rule itself
void RuleMethods::foreach(Rule& rule, Configuration& gameConfig, Constants& gameConsts, 
	Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience, Thread& thread) {
    // TODO: extract number of times to loop the for loop
    std::string list_string = std::get<std::string>(rule.valueMap.at("list"));
    std::vector<std::string> splitListString = RuleHelpers::splitByPeriod(list_string);

    int loopFrom, loopUntil;
    bool loopFromNumber = false;
    // based on the given config of configuration.Rounds.upfrom(1)
    if (splitListString[0].compare("configuration") == 0) {
        loopFrom = RuleHelpers::getIntInsideBrackets(splitListString[2]);

        // iterate though map, check the key if it contains splitListString[1]
        auto keyItr = std::find_if(gameConfig.setup.begin(), gameConfig.setup.end(), 
            [splitListString](std::pair<std::string, variableVariants>& pair) {
                auto key = splitListString[1];
                return (pair.first.find(key) != std::string::npos);
            }
        );

        if (keyItr == gameConfig.setup.end()) {
            std::cerr << splitListString[1] << " does not exist in the setup property. Exiting.";
            std::cerr << std::endl;
            abort();
        }

        // change to std::visit when time to expand to other RuleMethodss
        auto valueFromKey = std::get<size_t>(keyItr->second);
        loopUntil = valueFromKey;
        loopFromNumber = true;
    }
    

    // based on config of "list": "weapons"
    if (!loopFromNumber) {
        loopFrom = 1;
        auto keyItr = std::find_if(gameConsts.valueMap.begin(), gameConsts.valueMap.end(), 
            [splitListString](std::pair<std::string, variableVariants>& pair) {
                auto key = splitListString[0];
                return (pair.first.find(key) != std::string::npos);
            }
        );
        // change to std::visit when time to expand
        auto valueFromKey = std::get<std::vector<std::string>>(keyItr->second);
        loopUntil = valueFromKey.size();
    }

    std::vector<Rule>& rulesArray = rule.rulesList;
    for (loopFrom; loopFrom <= loopUntil; loopFrom++) {
		RuleHelpers::executeRules(rulesArray, gameConfig, gameConsts, gameVars, gamePerPlayer, gamePerAudience, Thread& thread);
    }
}

/*
assumes that until and while conditions are found in variables
example syntax: condition.true or condition.false -> "condition": "true" in variables
"until": "condition.true"
example syntax: condition.equal(n), condition.greaterThan(n), conditionGreaterEqualThan(n),
condition.lessThan(n), condition.lessEqualThan(n), condition.notEqual(n)
where "condition": n in variables and n is a number type 
*/
void RuleMethods::loop(Rule& rule, Configuration& gameConfig, Constants& gameConsts, 
	Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience, Thread& thread) {
        std::string condition_string;
    
	auto loopTypeItr = std::find_if(rule.valueMap.begin(), rule.valueMap.end(), 
		[](std::pair<std::string, variableVariants>& pair) {
		std::string untilStr = "until";
		std::string whileStr = "while";
		std::string_view loopTypeStr = pair.first;
		return (loopTypeStr.compare(untilStr) == 0 || loopTypeStr.compare(whileStr) == 0);
	});
	std::string_view loopTypeStr;
	auto loopErrStr = "no loop type found";
	if (loopTypeItr == rule.valueMap.end()) {
		loopTypeStr = loopErrStr;
	}
	else {
		loopTypeStr = loopTypeItr->first;
	}
	RuleHelpers::LoopMode loopMode = RuleHelpers::getLoopMode(loopTypeStr);	

    std::vector<std::string> splitConditionString = RuleHelpers::splitByPeriod(condition_string);
	std::string varName = splitConditionString.at(0);
	auto varValue = gameVars.valueMap.at(varName);
	
	std::vector<Rule>& rulesArray = rule.rulesList;
    if (loopMode == RuleHelpers::LoopMode::UNTIL) {
		while (RuleHelpers::conditionEvaluator(splitConditionString, varValue) == RuleHelpers::EvaluationResult::FALSE) {
			RuleHelpers::executeRules(rulesArray, gameConfig, gameConsts, gameVars, gamePerPlayer, gamePerAudience, Thread& thread);
		}
    }
    else if (loopMode == RuleHelpers::LoopMode::WHILE) {
		while (RuleHelpers::conditionEvaluator(splitConditionString, varValue) == RuleHelpers::EvaluationResult::TRUE) {
			RuleHelpers::executeRules(rulesArray, gameConfig, gameConsts, gameVars, gamePerPlayer, gamePerAudience, Thread& thread);
		}
    }
	else {
		std::cerr << "In the loop rule, neither the \"until\" or the \"while\" properties were detected. ";
        std::cerr << "Exiting." << std::endl;
        abort();
	}
}

void RuleMethods::rootswitch(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience, Thread& thread) {}
/*
{ "rule": "switch",
  "value": << value to switch upon >>,
  "list": << name of a constant list of allowable values >>,
  "cases": [
    { "case": << value >>, "rules": [ << Rules to execute when the value and case match >> ] },
    ...
  ]
}
*/

void RuleMethods::when(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience, Thread& thread) {}

//A when construct is analogous to if-then-else. The first case with a condition that evaluates to true has its rule list evaluated.
/*
{ "rule": "when",
  "cases": [
    { "condition": << Boolean guard for rules >>,
      "rules": [ << Rules to execute when the value and case match >> ]
    },
    ...
  ]
}
*/

void RuleMethods::extend(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience, Thread& thread)
{ 
  	if (root["variables"][rule["list"]] != root["variables"].end()) {//if variable list
  		root["variables"][rule["target"]].merge(root["variables"][rule["list"]]);
  	}
  	else if (root["constants"][rule["list"]] != root["constants"].end) {//if constant list
  		root["variables"][rule["target"]].merge(root["constants"][rule["list"]]);
  	}
  	else { //if immediate
  		root["variables"][rule["target"]].merge(rule["list"]);
  	}
 }

void RuleMethods::reverse(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience, Thread& thread)
{ 
	root["variables"][rule["list"]].reverse();
}

void RuleMethods::shuffle(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience, Thread& thread)
{
	//note: there is probably a more elegant implementation of this function.
	std::list<std::string> l = root["variables"][rule["list"]].as<std::list<std::string>>(NULL);
	std::vector<std::string> v = std::vector<std::string>();
	std::copy(l.begin(), l.end(), v.begin());
	std::random_shuffle(v.begin() , v.end());
	std::copy(v.begin(), v.end(), l.begin());
	root["variables"][rule["list"] = l;
}

void RuleMethods::sort(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience, Thread& thread)
{ 
  	if (rule["key"] != rule.end()) { //sorting a list within a map variable
  		root["variables"]["key"][rule["list"]].sort(); //note: I think this is what the outline says it should do, but it's not entirely clear
  	}
  	else { //sorting a list variable 
  		root["variables"][rule["list"]].sort();
  	}
}

void RuleMethods::deal(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience, Thread& thread)
{         
  	for (int i = 0 ; i < rule["count"] ; i++) {
  		root["variable"][rule["to"]].push_back(root["variable"][rule["from"]].pop_front());
  	}
}

void RuleMethods::discard(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience, Thread& thread)
{ 
	for (int i = 0 ; i < std::stoi(rule["count"] , i++) {
		root["variables"][rule["from"]].pop_front();
	}
}

void RuleMethods::add(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience, Thread& thread) 
{	
	std::string value = 0;
	if (root["variables"].find(rule["value"]) != root["variables"].end()) { //value is a variable
		root["variables"]["to"] = std::string(int(variable[rule["to"]]) + int(root["variables"][rule["value"]]))
	}
	if (root["constants"].find(rule["value"]) != root["variables"].end()) { //value is a constant
		root["variables"]["to"] = std::string(int(variable[rule["to"]]) + int(root["sonstants"][rule["value"]]))	
	}
	else { //value is an integer literal
		rule["to"] = std::string(int(variable[rule["to"]]) + int(rule["value"]))
	}
}

void RuleMethods::timer(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience, Thread& thread)
{
	int seconds = std::stoi(rule["duration"]);
  	std::string mode = rule["mode"];
 	auto start = std::chrono::system_clock::now();

	//must check each frame and interrupt input after ending
	set_timer(mode, start + seconds(seconds)); //TODO add to player struct, no other timer needs to be there 
}

void RuleMethods::input_choice(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience, Thread& thread)
{ 
	std::string message = rule["prompt"];
	parse_variables(root, message);
	
	int timeout = NULL;
	if (rule["timeout"] != rule.end()) {
		timeout = rule["timeout"];
	
	std::vector<std::string> choices;
	if (root["variables"].find(rule["choices"]) != root["variables"].end()) { //value is a variable
		std::vector<std::string> choices = parse_variable_list(root, rule["choices"]);//note: assumes the root file specifies players with another variable, set by another function
	}	
	else {//value is literal list 
		std::vector<std::string> choices = parse_variable_list(root, rule["choices"]);//note: assumes the root file specifies players with another variable, set by another function
	}

	root["variables"][rule["result"]] = server.input-choice(rule["to"], message, timeout); //"to" will be the name of the player or audience member
}

//This allows the player to make a multiple choice selection.
void RuleMethods::input_text(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience, Thread& thread)
{
	std::string message = rule["prompt"];
	parse_variables(root, message);
	
	int timeout = NULL;
	if (rule["timeout"] != rule.end()) {
		timeout = rule["timeout"];
	
	root["variables"][rule["result"]] = server.input-text(rule["to"], message, timeout); //"to" will be the name of the player or audience member
}

void RuleMethods::input_vote(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience, Thread& thread)
{ 
  	std::vector<std::string> recipients = parse_variable_list(root, rule["to"]);//note: list contains players/audience members, this must be handled in server.cpp
	std:;string message = rule["prompt"];
	parse_variables(root, message);

	std::vector<std::string> choices;
	if (root["variables"].find(rule["choices"]) != root["variables"].end()) { //value is a variable
		std::vector<std::string> choices = parse_variable_list(root, rule["choices"]);//note: assumes the root file specifies players with another variable, set by another function
	}	
	else {//value is literal list 
		std::vector<std::string> choices = parse_variable_list(root, rule["choices"]);//note: assumes the root file specifies players with another variable, set by another function
	}
	
	int timeout = NULL;
	if (rule["timeout"] != rule.end()) {
		timeout = rule["timeout"];

	map<std::string,std::string> result = server.vote(recipients, message, chocies, timeout);
	root["variables"][rule["result"]] = result;
}

void RuleMethods::message(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience, Thread& thread)
{
	std::vector<std::string> recipients = parse_variable_list(root, rule['to']);//note: assumes the root file specifies players with another variable, set by another function
	
	std::string message = rule["value"];
	parse_variables(root, message);

	server.player_message(recipients, message);
}

void RuleMethods::globalmessage(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience, Thread& thread){
	std::string message = rule["value"];
	parse_variables(root, message);
	server.global_message(message);
}

//This sends a message to the main screen of the RuleMethods instead of any player.
void RuleMethods::scores(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience, Thread& thread){
	std::vector<pair<std::string,std::string>> scores;
	for (int i = 0 ; i < players.length() ; i++){
		scores.push_back(per_player[players[i]][rule["score"]], make_pair(players[i]));
	}
	if (ascending)
		std::sort(scores.begin() , scores.end());
	server.display_scores(scores);
}







//non rule functions

//this function takes a string and adds in variables referenced by: {key}
void RuleMethods::parse_variables(RSJresource root, std::string & s) {
	std::string_view v {s};
	while (v.find_first_of('{') < v.size()) { //note: if using c++ 23, there is a better method for this = .contains()
		int start = v.find_first_of('{');
		int end = v.find_first_of('}');
		std::string variable_name = v.substr(start,end);
		std::string variable = root["variables"][variable_name]; //note: assumes variables are all strings, otherwise, convert before inserting
		s = s.substr(0,start) + variable + s.substr(end, v.size());
		//s.insert(start, variable); //not needed now
	}
}

//this function takes a list of strings (keys) and returns a vector of values for those keys
void RuleMethods::parse_variable_list(RSJresource root, root to) {
	std::vector<std::string> v;
	for ( int i = 0 ; i < to.size() ; i++) {
		v.push_back(root["variables"][to[i]]);
	}
}

void RuleMethods::list_attributes() 
{
	//given a list, need to be able to return 
		//.size
		//.contains
		//.collect (??)
		//.elements.key
}







void Game::startGame() {
	//TODO assumes there is already a master rule, this should be created in main.cpp and passed to the game object as an argument
	threads.push_back(Thread{"head" , head_rule, true});
	
	//TODO check timer for each thread, if timer, and if over time, do something 
	//need to do this every frame, since it should interrupt players while they are doing things
	//std::chrono::system_clock::now()
	
	while(true){
		auto next_thread = find_if(thread.begin(), thread.end(), [](Thread & thread) 
			{return thread.ready == true;});
		
		if (next_thread < thread.end()) {//if a player is ready for rules
			Rule* next_rule == next_thread.current_rule;
			next_rule = GetNextRule(*current_rule);
			current_thread.current_rule = next_rule;
			rule_map[next_rule.name]->exec(next_rule, configuration, variables, perPlayer, perAudience, thread);
		}
		else //still waiting for responses from players
			//TODO need to pass players, and variables so that it can update variables and set player to ready after receiving message
			read_message(sender); //checks until there are none to read, then returns	
		}
	}
}

void RuleMethods::inparallel(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience, Thread& thread) { 
	//TODO need to save instructions before use
		//ie. instead of adding, create an instruction for add, then execute them at the end of the loop
	//TODO copy paste the for loop here
}

//TODO add threads
//start with 1 thread = points to first rule
//splits at parallel for

void RuleMethods::parallelfor(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience, Thread& thread){
	//upon reaching this rule, replace it with 1 rule for each element in list (each have the same parent)
	//TODO create a thread for each = pointer to that rule
	//TODO each thread stores the element of the list
	//TODO when switching between threads = update variables with that element
	
	//TODO each of the new rules is just a for loop with the rules from this rule
}

//TODO add sub-rule index to each rule, initialize to -1
Rule GetNextRule(Rule current_rule) {
	if ((++current_rule.sub_rule_index) < current_rules.rules.size())
		return current_rule.rules[sub_rule_index];
	else if (current_rule.parent == nullptr) 
		return endGameRule; //TODO create end game rule (just ends game, fits into existing system well) (there should never be a situation where you are in parallel at end of game, so this is fine)
	else
		return GetNextRule(*current_rule.parent);
}

