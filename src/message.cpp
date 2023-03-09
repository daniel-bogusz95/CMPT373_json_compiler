#include <message.h>

//TODO replace all message types with 1 message
//TODO would still need to add handling in rules
//TODO This is necessary to allow us to send any data to the GUI and it will display it, without knowing all message types
struct Message {
	std::string message_type;
	std::vector<std::string> arguments;
};

Result read_message() {
	if (server.readBuffer.empty()) return;
	std::string s = server.readBuffer.pop_front();
	RSJresource json(s);
	std::string type = json["message_type"].as<std::string>("ERROR");
	if (type == "ERROR") return Result::FAILURE;
	message m = deserialize_commmand_map[type]->exec(json);
	Result r = process_message(m);
	return r;
{

Result process_message(struct input_text m) {
	std::string input = CreateGUI(m); //this method updates GUI then waits for input or timer to end
	struct choice_return r = {input, m.recipient, m.save_variable}; 
	std::string s = serialize(r);
	server.send_message(s);
	return SUCCESS;
}

Result process_message(struct input_choice m) {
	std::string input = CreateGUI(m); //this method updates GUI then waits for input, or timer to end
	struct choice_return r = {input, m.recipient, m.save_variable}; 
	std::string s = serialize(r);
	server.send_message(s);
	return SUCCESS;
}

Result process_message(struct player_message m) {
	std::string input = CreateGUI(m); //this method updates GUI then returns immediately
	return SUCCESS;
}

Result process_message(struct choice_return m) {
	if (m.time_limit >  0 && m.m.time_limit > time.time()) return;
	variable[m.save_variable] = m.input; 
	return SUCCESS;
}

Result Serialize(message m) {
	return serialize_commmand_map[m.message_type]->exec(m);
}

RSJresource serialize_input_text_message(message m) {
	RSJresource json("
	{
	    "message_type" : "NULL",
	    "input" : "NULL",
	    "recipient" : "NULL",
	    "save_variable" : "NULL"
    	    "time_limit" : "0"
	    ");
	}
	json["message_type"] = m.message_type;
	json["text"] = m.text;
	json["recipient"] = m.recipient;
	json["save_variable"] = m.save_variable;
	json["time_limit"] = m.time_limit;
	return json;
}

RSJresource serialize_input_choice_message(message m) {
	RSJresource json("
	{
	    "message_type" : "NULL",
	    "input" : "NULL",
	    "recipient" : "NULL",
	    "save_variable" : "NULL"
    	    "time_limit" : "0"
	    ");
	}
	json["message_type"] = m.message_type;
	json["text"] = m.text;
	json["recipient"] = m.recipient;
	json["save_variable"] = m.save_variable;
	json["time_limit"] = m.time_limit;
	return json;
}

RSJresource serialize_player_message(message m) {
	RSJresource json("
	{
	    "message_type" : "NULL",
	    "input" : "NULL",
	    "recipient" : "NULL",
	    "save_variable" : "NULL"
    	    "time_limit" : "0"
	    ");
	}
	json["message_type"] = m.message_type;
	json["text"] = m.text;
	json["recipient"] = m.recipient;
	return json;
}

RSJresource serialize_choice_return_message(message m) {
	RSJresource json("
	{
	    "message_type" : "NULL",
	    "input" : "NULL",
	    "recipient" : "NULL",
	    "save_variable" : "NULL"
    	    "time_limit" : "0"
	    ");
	}
	json["message_type"] = m.message_type;
	json["input"] = m.input;
	json["recipient"] = m.recipient;
	json["save_variable"] = m.save_variable;
	json["time_limit"] = m.time_limit;
	return json;
}

message deserialize_input_text_message(RSJresource json) {
	m input_text{
		json["message_type"].as<std::string>("ERROR");
		json["text"].as<std::string>("ERROR")
		json["recipient"].as<std::string>("ERROR")
		json["save_variable"].as<std::string>("ERROR")
		json["time_limit"].as<std::string>("ERROR");
	};
	return m;
}

message deserialize_input_choice_message(RSJresource json) {
	m input_choice{
		json["message_type"].as<std::string>("ERROR");
		json["text"].as<std::string>("ERROR")
		json["recipient"].as<std::string>("ERROR")
		json["choices"].as<std::vector<std::string>>("ERROR")
		json["save_variable"].as<std::string>("ERROR")
		json["time_limit"].as<std::string>("ERROR");
	};
	return m;
}

message deserialize_player_message(RSJresource json) {
	m player_message{
		json["message_type"].as<std::string>("ERROR");
		json["text"].as<std::string>("ERROR")
		json["recipient"].as<std::string>("ERROR")
	};
	return m;	
}

message deserialize_choice_return_message(RSJresource json) {
	m choice_return{
		json["message_type"].as<std::string>("ERROR");
		json["input"].as<std::string>("ERROR")
		json["recipient"].as<std::string>("ERROR")
		json["save_variable"].as<std::string>("ERROR")
		json["time_limit"].as<std::string>("ERROR");
	};
	return m;	
}

