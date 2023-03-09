//read the project description carefully
//https://coursys.sfu.ca/2022fa-cmpt-373-d1/pages/ProjectDescription
//all the variables will be maps in variables/constants/per-player/per-audience
#pragma once
#include "RSJparser.tcc"
#include "JSONDeserialized.h"

enum class Result {
	SUCCESS, 
	FAILURE,
	TIMEOUT
};

//note: because of the way json files are written, players do not need to be accessible through maps
struct class Thread {
	Rule* current_rule;
	//TODO add a single varientVariable (note: the key will be used in findif to change "bool ready" in message.cpp
	bool ready;
	auto at_most_timer_end_time;
}

class Game {
public:
  Game(){}
  Game(Constants constants, Configuration configuration, Variables Variables,
    PerPlayer PerPlayer, PerAudience perAudience, std::vector<Rule> rules) : configuration(configuration),
      constants(constants), variables(variables), perPlayer(perPlayer),
      perAudience(perAudience), rules(rules) {}

  // Execute each rule in linear order from the top level nesting
  void startGame();
private:
	Configuration configuration;
	Constants constants;
	Variables variables;
	PerPlayer perPlayer;
	PerAudience perAudience;
	
	std::vector<Rule> rules; //TODO remove this
	Rule head_rule; //TODO use this instead
	
	std::vector <Thread> threads;

  // Server server;
};

namespace RuleMethods {
  /*
  * Executes the rules stored in the json value object in the manner of a for loop.
  * list_of_rules is the JSON Array object that contain rules,
  * and root is the variable to the object containing the entire JSON object
  */
  void foreach(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience);
  void loop(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience);
  void inparallel(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience);

  //Note that this creates the potential for bugs even in a single threaded scenario depending on your design. It may be refined in the future.
  void parallelfor(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience);

  void jsonswitch(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience);

  //A when construct is analogous to if-then-else. The first case with a condition that evaluates to true has its rule list evaluated.
  void when(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience);

  Result extend(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience);
  Result reverse(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience);
  Result shuffle(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience);
  Result sort(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience);

  //Sorts a list in ascending order
  Result deal(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience);

  Result discard(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience);
  Result add(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience);
  Result timer(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience);
  Result input_choice(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience);
  Result input_text(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience);
  Result input_vote(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience);
  Result message(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience);
  Result globalmessage(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience);
  Result scores(Rule& rule, Configuration& gameConfig, Constants& gameConsts, Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience);
  void parse_for_variables(std::string & s);
  void parse_variable_list(RSJresource& json, RSJresource& to);
  void list_attributes();

  std::function<void(Rule&, Configuration&, Constants&, Variables&, PerPlayer&, PerAudience&)> nextRule(RuleName rule_name);
  const std::map<const RuleName, const std::function<void(Rule&, Configuration&, Constants&, Variables&, PerPlayer&, PerAudience&)>> ruleNameToFunc {
    { RuleName::_foreach, &foreach },
    { RuleName::_loop, &loop }

    // Uncomment pairs as functions get implemented
    // { "inparallel", &inparallel },
    // { "parallelfor", &parallelfor },
    // { "switch", &jsonswitch },
    // { "when", &when },
    // { "extend", &extend },
    // { "reverse", &reverse },
    // { "shuffle", &shuffle },
    // { "sort", &sort },
    // { "deal", &deal },
    // { "discard", &discard },
    // { "add", &add },
    // { "timer", &timer },
    // { "input-choice", &inputchoice },
    // { "input-text", &inputtext },
    // { "input-vote", &inputvote },
    // { "message", &message },
    // { "global-message", &globalmessage },
    // { "scores", &scores }
  };

};
