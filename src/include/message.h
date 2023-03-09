//note: server and clients will need message.cpp and message.h
#include "RSJparser.tcc"

enum class Result {
	SUCCESS, 
	FAILURE,
	TIMEOUT
};

const std::function command;
map<std::string, &command> deserialize_commmand_map;
deserialize_commmand_map["input_text"] = &deserialize_input_text_message;
deserialize_commmand_map["input_choice"] = &deserialize_input_choice_message;
deserialize_commmand_map["player_message"] = &deserialize_player_message;
deserialize_commmand_map["choice_return"] = &deserialize_choice_return_message;

map<std::string, &command> serialize_commmand_map;
deserialize_commmand_map["input_text"] = &serialize_input_text_message;
deserialize_commmand_map["input_choice"] = &serialize_input_choice_message;
deserialize_commmand_map["player_message"] = &serialize_player_message;
deserialize_commmand_map["choice_return"] = &serialize_choice_return_message;


//message is an enum of message_type enums, each message_type has several values, use serialization to send messages without changing server.cpp or client.cpp
enum message {
	global_message,
	player_message,
	input_text,
	input_choice,
	choice_return,
	display_scores,
	//add more if rules are added that send messages over server
};

struct global_message {
	std::string message_type; //type, not used by GUI
	std::string text; //display message on screen
};

struct player_message {
	std::string message_type; //type, not used by GUI
	std::string text; //display message on screen
	std::string recipient; //for addressing, not used by GUI
};

//need to include variable to save to
struct input_text {
	std::string message_type; //type, not used by GUI
	std::string text;//diplay message on screen
	std::string recipient; //for addressing, not used by GUI
	std::string save_variable;//for returning variables, not used by GUI
	std::string time_limit; //can display current time - time_limit on screen
};

struct input_choice {
	std::string message_type;//type, not needed by GUI
	std::string text;//display message on screen
	std::string recipient;//used for addressing, not used by GUI
	std::vector<std::string> choices;//display buttons for each choice, the selected choice is added to the choice_return input
	std::string save_variable;//used for saving the variable, not used by GUI
	std::string time_limit;//can display current time - time_limit on screen
};

struct choice_return {
	std::string message_type;//set this to the message_type of the input message
	std::string input;//set this to the choice or input text
	std::string recipient;//the recipient variable of the input message
	std::string save_variable;//the save_variable of the input message
	std::string time_limit; //the \time_limit of the input message
};

//this may not need to be a message since it will not be sent through server, keep it this way for 
struct display_scores {
	std::string message_type; //type, do not edit
	std::vector<std::pair<std::string, std::string>> scores;//display on screen (columns of player_name and score)
};

Result read_message(std::string sender);

message deserialize_input_text_message(RSJresource json);
message deserialize_input_choice_message(RSJresource json);
message deserialize_player_message(RSJresource json);
message deserialize_choice_return_message(RSJresource json);

RSJresource serialize_input_text_message(message m);
RSJresource serialize_input_choice_message(message m);
RSJresource serialize_player_message(message m);
RSJresource serialize_choice_return_message(message m);

