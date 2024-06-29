#pragma once

#include <array>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace budoux {
struct string_hash {
    using hash_type = std::hash<std::string_view>;
    using is_transparent = void;

    std::size_t operator()(const char* str) const {
        return hash_type{}(str);
    }

    std::size_t operator()(std::string_view str) const {
        return hash_type{}(str);
    }
    std::size_t operator()(std::string const& str) const {
        return hash_type{}(str);
    }
};

struct Model {
    enum class EParams : int { BW1, BW2, BW3, TW1, TW2, TW3, TW4, UW1, UW2, UW3, UW4, UW5, UW6, MAX };

    const std::array<const std::unordered_map<std::string, int, string_hash, std::equal_to<>>, static_cast<int>(EParams::MAX)> params;

    const auto& GetParam(const EParams param) const {
        return params[static_cast<int>(param)];
    }

    int GetScore(const EParams Param, const std::string_view& InKey) const {
        const auto& param = params[static_cast<int>(Param)];
        if (const auto& it = param.find(InKey); it != param.end()) {
            return it->second;
        }
        return 0;
    }
};
} // namespace budoux