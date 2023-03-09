#include <map>
#include "rules.h"
#include <vector>
#include "include/RSJparser.tcc"

//for the time being, this file only takes into account the requirements of rock_paper_scissors.json
//note: by this point, players are already connected to the server
int run_game(char** file_path, Server &server)
{     	
	//note: this can be done elsewhere and passed to this functio, since it must be parsed to check configuration data to set up the game (to check for min/max players, display name, etc.) 
	RSJresource json = parse_json(file_path);
	if (json == NULL) return 1;
	
	//start rule loop
	RSJresource rule(json["rules"]);
	for (int i = 0 ; i < json["rules"].size() ; i++)
		next_rule(json, rule[i])
		
	return 0;
}

void next_rule(RSJresource& root, RSJresource& rule){
	for (int i = 0 ; i < rule["rules"].size() ; i++) {
		//TODO create timer vector
		if (timer.back() < std::chrono::system_clock::now()){
			return;
		}
		
		//TODO replace below with rule_map[rule]->exec(rule);
		
		switch(rule["rule"]) {
	  	case "foreach":
	    		foreach(json, json, rule);
	    		break;
	  	case "loop":
	    		loop(json, rule);
	    		break;
	    	case "inparallel":
	    		inparallel(json, rule);
	    		break;
	    	case "parallelfor":
	    		parallelfor(json, rule);
	    		break;
	    	case "_switch":
	    		_switch(json, rule);
	    		break;
	    	case "when":
	    		when(json, rule);
	    		break;
	  	case "extend":
	    		extend(json, rule);
	    		break;
	    	case "reverse":
	    		reverse(json, rule);
	    		break;
	    	case "shuffle":
	    		shuffle(json, rule);
	    		break;
	    	case "sort":
	    		sort(json, rule);
	    		break;
	    	case "deal":
	    		deal(json, rule);
	    		break;
	  	case "discard":
	    		discard(json, rule);
	    		break;
	    	case "add":
	    		add(json, rule);
	    		break;
	    	case "timer":
	    		timer(json, rule);
	    	case "input choice":
	    		input choice(json, rule);
	    		break;
	    	case "input-text":
	    		input-text(json, rule);
	    		break;
	    	case "input-vote":
	    		input-vote(json, rule);
	    		break;
	  	case "message":
	    		message(json, rule);
	    		break;
	    	case "globalmessage":
	    		globalmessage(json, rule);
	    		break;
	    	
	    	
	  	default:
	    		cerr << "rule not found: " << rule["rule"] << endl;
			return;
		}
}

