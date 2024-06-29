#pragma once

#include <numeric>
#include <string_view>
#include <vector>

namespace budoux {

struct Model;

class Parser final {
public:
    explicit Parser(const Model& InModel);
    std::vector<std::string_view> Parse(const std::string_view Sentence) const;

private:
    const Model& ParsingModel;
    int BaseScore = 0;

    int ComputeBaseScore() const noexcept;
};

} // namespace budoux