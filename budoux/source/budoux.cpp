#include "budoux.h"
#include "models/model.h"
#include <list>
#include <numeric>
#include <string_view>
#include <utf8.h>
#include <vector>

namespace ParserHelpers {
class IteratorCache final {
    static constexpr int32_t CACHE_SIZE = 7;

public:
    IteratorCache(const std::string_view& InSentence)
        : Sentence{InSentence}
        , Iterators{CACHE_SIZE, InSentence.cend()} {

        utf8::iterator it(Sentence.begin(), Sentence.begin(), Sentence.end());

        for (int32_t i = 2; i < CACHE_SIZE; ++i) {
            Iterators[i] = it.base();

            try {
                ++it;
            } catch (const std::exception& e) {
                break;
            }
        }
    }

    const std::string_view::const_iterator& GetIterator(const int32_t InIndex) const {
        return Iterators[(Offset + CACHE_SIZE + InIndex) % CACHE_SIZE];
    }

    const std::string_view::const_iterator& operator[](const int32_t InIndex) const {
        return GetIterator(InIndex);
    }

    std::string_view CreateView(const int32_t InStart, const int32_t InEnd) const {
        return std::string_view{GetIterator(InStart), GetIterator(InEnd)};
    }

    void ReadNext() {
        const auto& LastIterator = GetIterator(CACHE_SIZE - 1);
        utf8::iterator it(LastIterator, LastIterator, Sentence.end());
        try {
            ++it;
            Iterators[Offset] = it.base();
        } catch (const std::exception& e) {
            Iterators[Offset] = Sentence.end();
        }
        Offset = (Offset + 1) % CACHE_SIZE;
    }

private:
    std::string_view Sentence;
    std::vector<std::string_view::const_iterator> Iterators;
    int32_t Offset = 0;
};
} // namespace ParserHelpers

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

    std::string_view::const_iterator lastBound = Sentence.cbegin();

    const int32_t letters = utf8::distance(Sentence.begin(), Sentence.end());
    ParserHelpers::IteratorCache Cache{Sentence};

    for (int i = 1; i < letters; ++i) {
        int score = BaseScore;

        if (i - 2 > 0) {
            score += ParsingModel.GetScore(Model::EParams::UW1, Cache.CreateView(0, 1));
        }
        if (i - 1 > 0) {
            score += ParsingModel.GetScore(Model::EParams::UW2, Cache.CreateView(1, 2));
        }
        score += ParsingModel.GetScore(Model::EParams::UW3, Cache.CreateView(2, 3));
        score += ParsingModel.GetScore(Model::EParams::UW4, Cache.CreateView(3, 4));
        if (i + 1 < letters) {
            score += ParsingModel.GetScore(Model::EParams::UW5, Cache.CreateView(4, 5));
        }
        if (i + 2 < letters) {
            score += ParsingModel.GetScore(Model::EParams::UW6, Cache.CreateView(5, 6));
        }
        if (i > 1) {
            score += ParsingModel.GetScore(Model::EParams::BW1, Cache.CreateView(1, 3));
        }
        score += ParsingModel.GetScore(Model::EParams::BW2, Cache.CreateView(2, 4));
        if (i + 1 < letters) {
            score += ParsingModel.GetScore(Model::EParams::BW3, Cache.CreateView(3, 5));
        }
        if (i - 2 > 0) {
            score += ParsingModel.GetScore(Model::EParams::TW1, Cache.CreateView(0, 3));
        }
        if (i - 1 > 0) {
            score += ParsingModel.GetScore(Model::EParams::TW2, Cache.CreateView(1, 4));
        }
        if (i + 1 < letters) {
            score += ParsingModel.GetScore(Model::EParams::TW3, Cache.CreateView(2, 5));
        }
        if (i + 2 < letters) {
            score += ParsingModel.GetScore(Model::EParams::TW4, Cache.CreateView(3, 6));
        }

        if (score > 0) {
            result.push_back(std::string_view{lastBound, Cache[3]});
            lastBound = Cache[3];
        }

        Cache.ReadNext();
    }

    if (lastBound != Sentence.cend()) {
        result.push_back(std::string_view{lastBound, Sentence.cend()});
    }

    return result;
}

} // namespace budoux