#pragma once

#include <map>
#include <variant>
#include <string>

#include "JSONDeserialized.h"

Configuration createTestConfig() {
    Configuration testData;

    testData.name = "Rock, Paper, Scissors";
    testData.minPlayerCount = 2;
    testData.maxPlayerCount = 4;
    testData.audience = false;
    testData.setup.insert(std::pair<std::string, variableVariants>("Rounds", (size_t)10));

    return testData;
}

Constants createTestConstants() {
    Constants testData;

    // Possible implementation without vectors
    // testData.valueMap.insert(std::pair<std::string, variableVariants>("weapons[0].name", "Rock"));
    // testData.valueMap.insert(std::pair<std::string, variableVariants>("weapons[0].beats", "Scissors"));
    // testData.valueMap.insert(std::pair<std::string, variableVariants>("weapons[1].name", "Paper"));
    // testData.valueMap.insert(std::pair<std::string, variableVariants>("weapons[1].beats", "Rock"));
    // testData.valueMap.insert(std::pair<std::string, variableVariants>("weapons[2].name", "Scissors"));
    // testData.valueMap.insert(std::pair<std::string, variableVariants>("weapons[2].beats", "Paper"));

    std::vector<std::string> weaponsName;
    weaponsName.reserve(3);
    weaponsName.push_back("Rock");
    weaponsName.push_back("Paper");
    weaponsName.push_back("Scissors");
    std::vector<std::string> weaponsBeats;
    weaponsBeats.reserve(3);
    weaponsBeats.push_back("Scissors");
    weaponsBeats.push_back("Rocks");
    weaponsBeats.push_back("Paper");

    testData.valueMap.insert(std::pair<std::string, variableVariants>("weapons.name", weaponsName));
    testData.valueMap.insert(std::pair<std::string, variableVariants>("weapons.beats", weaponsBeats));

    return testData;
}

Variables createTestVariables() {
    Variables testData;

    std::vector<std::string> winners; 
    testData.valueMap.insert(std::pair<std::string, variableVariants>("winners", winners));
    
    return testData;
}

PerPlayer createTestPerPlayer() {
    PerPlayer testData;

    testData.valueMap.insert(std::pair<std::string, variableVariants>("wins", (size_t)0));

    return testData;
}

PerAudience createTestPerAudience() {
    PerAudience testData;

    return testData;
}

Configuration testSaySomethingConfig() {
    Configuration testConfig;
    testConfig.name = "say something 10 times";
    testConfig.minPlayerCount = 1;
    testConfig.maxPlayerCount = 1;
    testConfig.audience = false;
    testConfig.setup.insert(std::pair<std::string, variableVariants>("Rounds", 10));
    return testConfig;
}

Constants testSaySomethingConsts() {
    Constants testConsts;
    return testConsts;
}

Variables testSaySomethingVars() {
    Variables testVars;
    testVars.valueMap.insert(std::pair<std::string, variableVariants>("input", "not talkative"));
    return testVars;
}

PerPlayer testSaySomethingPerPlayer() {
    PerPlayer testPerPlayer;
    return testPerPlayer;
}

PerAudience testSaySomethingPerAudience() {
    PerAudience testPerAudience;
    return testPerAudience;
}

std::vector<Rule> testSaySomethingRules() {
    Rule firstRule;
    // firstRule.valueMap.insert(std::pair<std::string, variableVariants>("rule", "foreach"));
    firstRule.ruleName = RuleName::_foreach;
    firstRule.valueMap.insert(std::pair<std::string, variableVariants>("list", "configuration.Rounds.upfrom(1)"));
    firstRule.valueMap.insert(std::pair<std::string, variableVariants>("element", "round"));

    Rule nestedForEach;
    nestedForEach.ruleName = RuleName::_foreach;
    nestedForEach.valueMap.insert(std::pair<std::string, variableVariants>("list", "players"));
    nestedForEach.valueMap.insert(std::pair<std::string, variableVariants>("element", "player"));

    Rule inputText;
    inputText.ruleName = RuleName::_input_text;
    inputText.valueMap.insert(std::pair<std::string, variableVariants>("to", "player"));
    inputText.valueMap.insert(std::pair<std::string, variableVariants>("prompt", "What do you want to say?"));
    inputText.valueMap.insert(std::pair<std::string, variableVariants>("result", "input"));
    
    Rule globalMessage;
    globalMessage.ruleName = RuleName::_global_message;
    globalMessage.valueMap.insert(std::pair<std::string, variableVariants>("value", "input"));

    nestedForEach.rulesList.push_back(inputText);
    firstRule.rulesList.push_back(nestedForEach);
    firstRule.rulesList.push_back(globalMessage);

    std::vector<Rule> testRules;
    testRules.push_back(firstRule);
    return testRules;
}