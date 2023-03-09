#include "gtest/gtest.h"
#include "../src/include/rules.h"
#include "../src/include/RSJparser.tcc"

using namespace testing;

// std::string reverseRuleJsonText = "{ \"rule\": \"reverse\",
//                                      \"list\": testData
//                                    }"

// std::vector<int> testData = {1,2,3,4,5};

void check_split_by_period_result(std::vector<std::string> expected, std::vector<std::string> actual){
  ASSERT_EQ(expected.size(), actual.size()) << "Vectors are not same length";
  ASSERT_EQ(expected, actual);
}

void check_throws_invalid_arg(std::string test){
  ASSERT_THROW(RuleMethods::get_number_inside_brackets(test), std::invalid_argument);
}

// TEST(reverse_rule_tests, Normal_Behaviour){
//   RSJresource reverseRule = parse_json(reverseRuleJsonText);

//   RuleMethods::reverse()

// }

TEST(split_by_period_tests, Empty_Input){
  std::string test = "";
  std::vector<std::string> result = RuleMethods::split_by_period(test);

  check_split_by_period_result({}, result);
}

TEST(split_by_period_tests, One_Period) {
  std::string test = "Hello.World";
  std::vector<std::string> result = RuleMethods::split_by_period(test);

  check_split_by_period_result({"Hello","World"}, result);
}

TEST(split_by_period_tests, No_Periods){
  std::string test = "Hello";
  std::vector<std::string> result = RuleMethods::split_by_period(test);

  check_split_by_period_result({"Hello"}, result);
}

TEST(split_by_period_tests, Handles_NewLines){
  std::string test = "Hello.\nWorld";
  std::vector<std::string> result = RuleMethods::split_by_period(test);

  check_split_by_period_result({"Hello", "World"}, result);
}

TEST(split_by_period_tests, Period_at_Start){
  std::string test = ".Hello World";
  std::vector<std::string> result = RuleMethods::split_by_period(test);

  check_split_by_period_result({"Hello World"}, result);
}

TEST(split_by_period_tests, Period_at_End){
  std::string test = "Hello World.";
  std::vector<std::string> result = RuleMethods::split_by_period(test);

  check_split_by_period_result({"Hello World"}, result);
}

TEST(split_by_period_tests, Many_Periods){
  std::string test = ".He.l.lo W.or.ld.";
  std::vector<std::string> result = RuleMethods::split_by_period(test);

  check_split_by_period_result({"He", "l", "lo W", "or", "ld"}, result);
}

TEST(get_number_inside_brackets_tests, Normal_Input){
  std::string test = "(5)";
  int result = RuleMethods::get_number_inside_brackets(test);
  ASSERT_EQ(5, result);
}

TEST(get_number_inside_brackets_tests, No_Brackets){
  std::string test = "16";
  check_throws_invalid_arg(test);
}

TEST(get_number_inside_brackets_tests, No_Closing_Bracket){
  std::string test = "(5";
  check_throws_invalid_arg(test);
}

TEST(get_number_inside_brackets_tests, Inverted_Brackets){
  std::string test = ")1(";
  check_throws_invalid_arg(test);
}

TEST(get_number_inside_brackets_tests, Number_Outside_Brackets){
  std::string test = "3()";
  check_throws_invalid_arg(test);
}

TEST(get_number_inside_brackets_tests, Number_Partially_Outside){
  std::string test = "(67)58";
  ASSERT_EQ(67, RuleMethods::get_number_inside_brackets(test));
}

TEST(get_number_inside_brackets_tests, Empty_Brackets){
  std::string test = "()";
  check_throws_invalid_arg(test);
}

TEST(get_number_inside_brackets_tests, Extremely_Large_Number){
  std::string test = "(9999999999999)";
  ASSERT_THROW(RuleMethods::get_number_inside_brackets(test), std::out_of_range);
}

TEST(get_number_inside_brackets_tests, Non_Integer){
  std::string test = "(Hello)";
  check_throws_invalid_arg(test);
}

int main(int argc, char **argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}