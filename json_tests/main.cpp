// not ready for push
#include <iostream>
#include <map>
#include <variant>
#include <string>
#include <vector>
#include <optional>
#include <fstream>

#include "json.hpp"
#include "jsonclass.h"

using json = nlohmann::json;

//use 2d array method for nested json file

enum class RuleName
{
    _foreach,
    _loop,
    _parallelfor,
    _inparallel,
    _switch,
    _when,
    _extend,
    _reverse,
    _shuffle,
    _sort,
    _deal,
    _discard,
    _add,
    _timer,
    _input_choice,
    _input_text,
    _input_vote,
    _message,
    _global_message,
    _scores
};

namespace config
{
    struct Configuration
    {
        std::string name;
        size_t minPlayerCount;
        size_t maxPlayerCount;
        bool audience;
        /// @brief add std::map<std::string, variableVariants> setup;
    };
}

namespace rule
{
    struct Rule
    {
        RuleName ruleName;
        std::map<std::string, variableVariants> valueMap;
        std::vector<Rule> rulesList;
    };
}

void makeConfig() // to json
{
    /// @brief add std::map<std::string, variableVariants> setup; here in json object creator
    config::Configuration cf1 = {"config_name_1", 3, 2, true};
    json config_json_object = cf1;

    //Json::parser config_json_object = cf1; not working
}

void readConig()
{
    /// @brief in this test_object, check again with std::map<std::string, variableVariants> setup; included in struct
    auto test_object = R"(
    {
        "configuration" :
        {
            "name" : "config_name_1",
            "minPlayerCount" : 2,
            "maxPlayerCount" : 3,
            "audience" : true
        }
    })"_json;

    /// @brief to do here.. replace int by auto container, {int, string, mao, vector or other things}
    std::map<std::string, int> m = j.at("configuration").get<std::map<std::string, int/*create a enum here*/>>();
    std::cout << m.at("name") << "\n";
    std::cout << m.at("minPlayerCount") << "\n";
    std::cout << m.at("maxPlayerCount") << "\n";
    std::cout << m.at("audience") << "\n";
}

void readConig()
{
    /// @brief in this test_object, check again with std::map<std::string, variableVariants> setup; included in struct
    auto test_object = R"(
    {
        "config" :
        {
            "name" : "config_name_1",
            "minPlayerCount" : 2,
            "maxPlayerCount" : 3,
            "audience" : true
        }
    })"_json;
    
    /// @brief to do here.. replace int by auto container, {int, string, mao, vector or other things}
    std::map<std::string, int> m = j.at("config").get<std::map<std::string, int/*create a enum here*/>>();
    std::cout << m.at("name") << "\n";
    std::cout << m.at("minPlayerCount") << "\n";
    std::cout << m.at("maxPlayerCount") << "\n";
    std::cout << m.at("audience") << "\n";
}