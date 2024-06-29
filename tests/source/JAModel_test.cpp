#include <catch2/catch_test_macros.hpp>
#include <models/JAModel.h>
#include <models/model.h>

TEST_CASE("model generation validation") {
    using namespace budoux;

    const budoux::Model& jaModel = budoux::GetJAModel();
    REQUIRE(!jaModel.params[0].empty());

    REQUIRE(jaModel.GetParam(Model::EParams::UW3).size() == 171);
    REQUIRE(jaModel.GetParam(Model::EParams::UW3).at("中") == 998);
}