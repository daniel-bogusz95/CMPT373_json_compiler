add_executable(rulesTests rulesTests.cc)
target_link_libraries(rulesTests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(rulesTests)