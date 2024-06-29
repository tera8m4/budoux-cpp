#include "budoux.h"
#include "models/model.h"
#include <numeric>
#include <string_view>
#include <vector>

namespace budoux {
Parser::Parser(const Model& InModel)
    : ParsingModel{InModel}
    , BaseScore{ComputeBaseScore()} {}

int Parser::ComputeBaseScore() const noexcept {
    int value = 0;
    for (const auto& param : ParsingModel.params) {
        value = std::accumulate(param.begin(), param.end(), value, [](const int acc, const auto& pair) {
            return acc - pair.second;
        });
    }

    return value / 2;
}

std::vector<std::string_view> Parser::Parse(const std::string_view Sentence) const {
    std::vector<std::string_view> result;

    int lastBound = 0;
    for (int i = 1; i < Sentence.size(); ++i) {
        int score = BaseScore;

        if (i - 2 > 0) {
            score += ParsingModel.GetScore(Model::EParams::UW1, Sentence.substr(i - 3, 1));
        }
        if (i - 1 > 0) {
            score += ParsingModel.GetScore(Model::EParams::UW2, Sentence.substr(i - 2, 1));
        }
        score += ParsingModel.GetScore(Model::EParams::UW3, Sentence.substr(i - 1, 1));
        score += ParsingModel.GetScore(Model::EParams::UW4, Sentence.substr(i, 1));
        if (i + 1 < Sentence.size()) {
            score += ParsingModel.GetScore(Model::EParams::UW5, Sentence.substr(i + 1, 1));
        }
        if (i + 2 < Sentence.size()) {
            score += ParsingModel.GetScore(Model::EParams::UW6, Sentence.substr(i + 2, 1));
        }
        if (i > 1) {
            score += ParsingModel.GetScore(Model::EParams::BW1, Sentence.substr(i - 2, 2));
        }
        score += ParsingModel.GetScore(Model::EParams::BW2, Sentence.substr(i - 1, 2));
        if (i + 1 < Sentence.size()) {
            score += ParsingModel.GetScore(Model::EParams::BW3, Sentence.substr(i, 2));
        }
        if (i - 2 > 0) {
            score += ParsingModel.GetScore(Model::EParams::TW1, Sentence.substr(i - 3, 3));
        }
        if (i - 1 > 0) {
            score += ParsingModel.GetScore(Model::EParams::TW2, Sentence.substr(i - 2, 3));
        }
        if (i + 1 < Sentence.size()) {
            score += ParsingModel.GetScore(Model::EParams::TW3, Sentence.substr(i - 1, 3));
        }
        if (i + 2 < Sentence.size()) {
            score += ParsingModel.GetScore(Model::EParams::TW4, Sentence.substr(i, 3));
        }

        if (score > 0) {
            result.push_back(Sentence.substr(lastBound, i - lastBound));
            lastBound = i;
        }
    }

    result.push_back(Sentence.substr(lastBound));

    return result;
}

} // namespace budoux