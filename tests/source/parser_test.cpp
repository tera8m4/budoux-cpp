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

    const std::string_view TEST_SENTENCE = "abcdeabcd";
    const Parser parser(model);
    const auto result = parser.Parse(TEST_SENTENCE);
    REQUIRE(result.size() == 2);
    REQUIRE_THAT(result, Catch::Matchers::Equals(std::vector<std::string_view>{"abcde", "abcd"}));
}