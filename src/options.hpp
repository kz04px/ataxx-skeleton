#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <string>
#include <vector>

namespace Options {

struct Check {
    std::string name;
    bool value = false;
};

struct Spin {
    std::string name;
    int min = 0;
    int max = 0;
    int val = 0;
};

struct Button {
    std::string name;
};

struct Combo {
    std::string name;
    std::size_t idx = 0;
    std::vector<std::string> values;
};

struct String {
    std::string name;
    std::string value;
};

// Create options
extern Check debug;

}  // namespace Options

#endif
