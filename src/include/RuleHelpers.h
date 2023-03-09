#pragma once

#include <functional>
#include <vector>
#include <string>
#include <map>

#include "JSONDeserialized.h"

namespace RuleHelpers {
    enum class ConditionType {  EQUAL, NOTEQUAL, GREATER, GREATEREQUAL, 
                                LESS, LESSEQUAL, TRUE, FALSE, INVALID };
    const std::map<std::string, ConditionType> typeMap = {
        { "equal", ConditionType::EQUAL },
        { "notEqual", ConditionType::NOTEQUAL },
        { "greaterThan", ConditionType::GREATER },
        { "greaterEqualThan", ConditionType::GREATEREQUAL },
        { "less", ConditionType::LESS },
        { "lessEqualThan", ConditionType::LESSEQUAL },
        { "true", ConditionType::TRUE },
        { "false", ConditionType::FALSE }
    };

    std::vector<std::string> splitByPeriod(std::string& to_parse);
    int getIntInsideBrackets(const std::string& query);
    double getDoubleInsideBrackets(const std::string& query);

    enum class EvaluationResult { TRUE, FALSE, ERROR };
    EvaluationResult conditionEvaluator(std::vector<std::string> splitConditionString, variableVariants varValue);

    enum class LoopMode { UNTIL, WHILE, INVALID };
    LoopMode getLoopMode(std::string_view loop_str);

    void executeRules(std::vector<Rule> ruleList, Configuration& gameConfig, Constants& gameConsts, 
	    Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience);
};