#pragma once
#include "RSJparser.tcc"
#include "JSONDeserialized.h"
#include "RSJparser.tcc"
#include "rules.h"
#include "RuleHelpers.h"
#include "server.h"

#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include <random>
#include <string_view>
#include <functional>
#include <filesystem>

void main();
std::vector <std::string> GetGames();
std::vector <std::string> GetGames();
Configuration GetConfiguration(RSJson json);
Constants GetConstants(RSJson json);
Variables GetVariables(RSJson json);
PerPlayer GetPerPlayer(RSJson json);
PerAudience GetPerAudience(RSJson json);
std::vector<Rule> GetRules(RSJson json);
Rule GetRule(RSJson json_rule);

std::map <std::string,RuleName> RuleNameMap = {
	{"foreach" : _foreach},
	{"loop" : _loop},
	{"parallelfor" : _parallelfor},
	{"inparallel" : _inparallel},
	{"switch" : _switch},
	{"when" : _when},
	{"extend" : _extend},
	{"reverse" : _reverse},
	{"shuffle" : _shuffle},
	{"sort" : _sort},
	{"deal" : _deal},
	{"discard" : _discard},
	{"add" : _add},
	{"timer" : _timer},
	{"input-choice" : _input_choice},
	{"input-text" : _input_text},
	{"input-vote" : _input_vote},
	{"message" : _message},
	{"global-message" : _global_message},
	{"scores" : _scores},
};

