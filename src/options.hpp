#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <string>
#include <vector>

namespace Options {

struct [[nodiscard]] Check {
    std::string name;
    bool value = false;
};

struct [[nodiscard]] Spin {
    std::string name;
    int min = 0;
    int max = 0;
    int val = 0;
};

struct [[nodiscard]] Button {
    std::string name;
};

struct [[nodiscard]] Combo {
    std::string name;
    std::size_t idx = 0;
    std::vector<std::string> values;
};

struct [[nodiscard]] String {
    std::string name;
    std::string value;
};

// Create options
extern Check debug;

}  // namespace Options

#endif
