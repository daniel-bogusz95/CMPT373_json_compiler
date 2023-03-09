#pragma once

#include <map>
#include <variant>
#include <string>
#include <vector>
#include <optional>

// std::optional makes it not compile for some reason
using variableVariants = std::variant<int, float, size_t, std::string, 
    std::vector<int>, std::vector<float>, std::vector<size_t>, 
    std::vector<std::string>>;

template <typename Tag>
struct JsonVariable {
    std::map<std::string, variableVariants> valueMap;
};

// using Configuration = JsonVariable<struct Configuration>;
using Constants = JsonVariable<struct Constants>;
using Variables = JsonVariable<struct Variables>;
using PerPlayer = JsonVariable<struct PerPlayer>;
using PerAudience = JsonVariable<struct PerAudience>;

struct Configuration {
    std::string name;
    size_t minPlayerCount;
    size_t maxPlayerCount;
    bool audience;
    std::map<std::string, variableVariants> setup;
};

enum class RuleName {   _foreach, _loop, _parallelfor, _inparallel, _switch,
                        _when, _extend, _reverse, _shuffle, _sort, _deal,
                        _discard, _add, _timer, _input_choice, _input_text,
                        _input_vote, _message, _global_message, _scores      };

struct Rule {
    RuleName ruleName;
    std::map<std::string, variableVariants> valueMap;
    std::vector<Rule> rulesList;
};
