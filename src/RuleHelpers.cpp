#include <vector>
#include <string>
#include <functional>
#include <iostream>
#include <map>

#include "RuleHelpers.h"
#include "rules.h"

std::vector<std::string> RuleHelpers::splitByPeriod(std::string& to_parse) {
    std::vector<std::string> to_return;
    to_return.reserve(to_parse.length());
    bool done_parsing = false;
    auto current_pos = to_parse.begin();

    while (!done_parsing) {
        auto index = std::distance(to_parse.begin(), current_pos);
        auto period_itr = std::find_if(current_pos, to_parse.end(), 
            [](const char& c) {
                return c == '.';
            }
        );
        auto period_index = std::distance(to_parse.begin(), period_itr);
        auto extracted = to_parse.substr(index, period_index - index);
        to_return.push_back(extracted);

        done_parsing = (period_itr == to_parse.end());
        current_pos = period_itr + 1;
    }

    to_return.shrink_to_fit();
    return to_return;
}

// pass in a upfrom(n) statement and it will return n
int RuleHelpers::getIntInsideBrackets(const std::string& query) {
    auto open_bracket_index = query.find('(');
    auto closed_bracket_index = query.find(')');
    auto to_parse = query.substr(open_bracket_index + 1, closed_bracket_index - open_bracket_index);
    int return_n;
    
    try {
        return_n = std::stoi(to_parse);
    } 
    catch (std::invalid_argument& e) {
        std::cerr << "Argument passed in is unable to be parsed: " << to_parse << std::endl;
        abort();
    } catch (std::out_of_range& e) {
        std::cerr << "Argument passed in is too large: " << to_parse << std::endl;
        abort();
    }

    return return_n;
}

double RuleHelpers::getDoubleInsideBrackets(const std::string& query) {
    auto open_bracket_index = query.find('(');
    auto closed_bracket_index = query.find(')');
    auto to_parse = query.substr(open_bracket_index + 1, closed_bracket_index - open_bracket_index);
    int return_n;
    
    try {
        return_n = std::stod(to_parse);
    } 
    catch (std::invalid_argument& e) {
        std::cerr << "Argument passed in is unable to be parsed: " << to_parse << std::endl;
        abort();
    } catch (std::out_of_range& e) {
        std::cerr << "Argument passed in is too large: " << to_parse << std::endl;
        abort();
    }

    return return_n;
}

RuleHelpers::LoopMode RuleHelpers::getLoopMode(std::string_view loopTypeStr) {
	if (loopTypeStr.compare("until") == 0) {
		return RuleHelpers::LoopMode::UNTIL;
	}
	else if (loopTypeStr.compare("while") == 0) {
		return RuleHelpers::LoopMode::WHILE;
	}
	else { 
		return RuleHelpers::LoopMode::INVALID;
	}
}

RuleHelpers::EvaluationResult RuleHelpers::conditionEvaluator(std::vector<std::string> splitConditionString, variableVariants varValue) {
    const std::string& variableName = splitConditionString.at(0);
    const std::string& condition = splitConditionString.at(1);
    ConditionType cType;
    try {
        cType = typeMap.at(condition);
    }
    catch (std::out_of_range e) {
        return RuleHelpers::EvaluationResult::ERROR;
    };

    // Is there a better way to write the Visitor s.t. I don't have to
    // pass in condition to extract the number inside?
    // Current solution is crude so will need refactoring.
	struct Visitor {
		EvaluationResult operator()(int& lhs, std::string& toExtractNumber, ConditionType cType) {
            int rhs = getIntInsideBrackets(toExtractNumber);
            if (cType == ConditionType::EQUAL) {
                if (lhs == rhs) {
                    return EvaluationResult::TRUE;
                }
                else {
                    return EvaluationResult::FALSE;
                }
            }
            else if (cType == ConditionType::NOTEQUAL) {
                if (lhs != rhs) {
                    return EvaluationResult::TRUE;
                }
                else {
                    return EvaluationResult::FALSE;
                }
            }
            else if (cType == ConditionType::GREATER) {
                if (lhs > rhs) {
                    return EvaluationResult::TRUE;
                }
                else {
                    return EvaluationResult::FALSE;
                }
            }
            else if (cType == ConditionType::GREATEREQUAL) {
                if (lhs >= rhs) {
                    return EvaluationResult::TRUE;
                }
                else {
                    return EvaluationResult::FALSE;
                }
            }
            else if (cType == ConditionType::LESS) {
                if (lhs < rhs) {
                    return EvaluationResult::TRUE;
                }
                else {
                    return EvaluationResult::FALSE;
                }
            }
            else if (cType == ConditionType::LESSEQUAL) {
                if (lhs <= rhs) {
                    return EvaluationResult::TRUE;
                }
                else {
                    return EvaluationResult::FALSE;
                }
            }
            else {
                return EvaluationResult::ERROR;
            }
        }
		EvaluationResult operator()(double& lhs, std::string& toExtractNumber, ConditionType cType) {
            double rhs = getDoubleInsideBrackets(toExtractNumber);
            if (cType == ConditionType::EQUAL) {
                if (lhs == rhs) {
                    return EvaluationResult::TRUE;
                }
                else {
                    return EvaluationResult::FALSE;
                }
            }
            else if (cType == ConditionType::NOTEQUAL) {
                if (lhs != rhs) {
                    return EvaluationResult::TRUE;
                }
                else {
                    return EvaluationResult::FALSE;
                }
            }
            else if (cType == ConditionType::GREATER) {
                if (lhs > rhs) {
                    return EvaluationResult::TRUE;
                }
                else {
                    return EvaluationResult::FALSE;
                }
            }
            else if (cType == ConditionType::GREATEREQUAL) {
                if (lhs >= rhs) {
                    return EvaluationResult::TRUE;
                }
                else {
                    return EvaluationResult::FALSE;
                }
            }
            else if (cType == ConditionType::LESS) {
                if (lhs < rhs) {
                    return EvaluationResult::TRUE;
                }
                else {
                    return EvaluationResult::FALSE;
                }
            }
            else if (cType == ConditionType::LESSEQUAL) {
                if (lhs <= rhs) {
                    return EvaluationResult::TRUE;
                }
                else {
                    return EvaluationResult::FALSE;
                }
            }
            else {
                return EvaluationResult::ERROR;
            }
        }
		EvaluationResult operator()(size_t& lhs, std::string& toExtractNumber, ConditionType cType) {
            size_t rhs = (size_t)getIntInsideBrackets(toExtractNumber);
            if (cType == ConditionType::EQUAL) {
                if (lhs == rhs) {
                    return EvaluationResult::TRUE;
                }
                else {
                    return EvaluationResult::FALSE;
                }
            }
            else if (cType == ConditionType::NOTEQUAL) {
                if (lhs != rhs) {
                    return EvaluationResult::TRUE;
                }
                else {
                    return EvaluationResult::FALSE;
                }
            }
            else if (cType == ConditionType::GREATER) {
                if (lhs > rhs) {
                    return EvaluationResult::TRUE;
                }
                else {
                    return EvaluationResult::FALSE;
                }
            }
            else if (cType == ConditionType::GREATEREQUAL) {
                if (lhs >= rhs) {
                    return EvaluationResult::TRUE;
                }
                else {
                    return EvaluationResult::FALSE;
                }
            }
            else if (cType == ConditionType::LESS) {
                if (lhs < rhs) {
                    return EvaluationResult::TRUE;
                }
                else {
                    return EvaluationResult::FALSE;
                }
            }
            else if (cType == ConditionType::LESSEQUAL) {
                if (lhs <= rhs) {
                    return EvaluationResult::TRUE;
                }
                else {
                    return EvaluationResult::FALSE;
                }
            }
            else {
                return EvaluationResult::ERROR;
            }
        }
		EvaluationResult operator()(bool& lhs, std::string& toExtractNumber, ConditionType cType) {
            if (cType == ConditionType::TRUE) {
                return lhs == true ? EvaluationResult::TRUE : EvaluationResult::FALSE;
            }
            else if (cType == ConditionType::FALSE) {
                return lhs == false ? EvaluationResult::TRUE : EvaluationResult::FALSE;
            }
            else {
                return EvaluationResult::ERROR;
            }
        }
	};

    EvaluationResult returnVal = std::visit(Visitor(), varValue, condition, cType);
	return returnVal;
}

void RuleHelpers::executeRules(std::vector<Rule> ruleList, Configuration& gameConfig, Constants& gameConsts, 
	Variables& gameVars, PerPlayer& gamePerPlayer, PerAudience& gamePerAudience) {
    for (Rule& ruleToRun : ruleList) {
            auto ruleName = ruleToRun.ruleName;
            auto nextRuleMethod = RuleMethods::nextRule(ruleName);
            nextRuleMethod(ruleToRun, gameConfig, gameConsts, gameVars, gamePerPlayer, gamePerAudience);
    }
}
