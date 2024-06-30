#include "models/JAModel.h"
#include "models/model.h"
#include <budoux.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <iostream>

using namespace budoux;

TEST_CASE("Parser test") {
    Model model{.params = {{
                    {},
                    {},
                    {},
                    {},
                    {},
                    {},
                    {},
                    {},
                    {},
                    {},
                    {{"a", 1000}},
                }}};

    const std::string_view TEST_SENTENCE = "abcdeazxy";
    const Parser parser(model);
    const auto result = parser.Parse(TEST_SENTENCE);
    REQUIRE(result.size() == 2);
    REQUIRE_THAT(result, Catch::Matchers::Equals(std::vector<std::string_view>{"abcde", "azxy"}));
}

TEST_CASE("should separate even if it makes a phrase of one character") {
    Model model{.params = {{
                    {},
                    {},
                    {},
                    {},
                    {},
                    {},
                    {},
                    {},
                    {},
                    {},
                    {{"b", 1000}},
                }}};
    const std::string_view TEST_SENTENCE = "abcdeabcd";
    const Parser parser(model);
    const auto result = parser.Parse(TEST_SENTENCE);
    REQUIRE(result.size() == 3);
    REQUIRE_THAT(result, Catch::Matchers::Equals(std::vector<std::string_view>{"a", "bcdea", "bcd"}));
}

TEST_CASE("should return an empty list when the input is a blank string") {
    const Parser parser({});
    const auto result = parser.Parse("");
    REQUIRE(result.size() == 0);
}

TEST_CASE("Parse with japanese model") {
    Parser p{GetJAModel()};
    std::string s = "Google の使命は、世界中の情報を整理し、世界中の人がアクセスできて使えるようにすることです。";
    const auto result = p.Parse(s);
    REQUIRE(result.size() == 11);
    const std::vector<std::string_view> expected{"Google の", "使命は、",       "世界中の",     "情報を", "整理し、",  "世界中の",
                                                 "人が",      "アクセスできて", "使えるように", "する",   "ことです。"};
    REQUIRE_THAT(result, Catch::Matchers::Equals(expected));

    const auto second = p.Parse("今日は天気です。");
    const std::vector<std::string_view> expected_second{"今日は", "天気です。"};
    REQUIRE_THAT(second, Catch::Matchers::Equals(expected_second));
}
