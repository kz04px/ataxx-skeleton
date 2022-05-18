#include <iostream>
#include "../options.hpp"
#include "protocol.hpp"

namespace UAI {

// Set an option
auto setoption(std::stringstream &stream) -> void {
    std::string word;

    stream >> word;
    if (word != "name") {
        return;
    }

    // Collect option name
    std::string name;
    while (stream >> word && word != "value") {
        if (!name.empty()) {
            name += " ";
        }
        name += word;
    }

    // Collect option value
    std::string value;
    while (stream >> word) {
        if (!value.empty()) {
            value += " ";
        }
        value += word;
    }

    if (name == "debug") {
        Options::debug.value = value == "true";
    }
}

}  // namespace UAI
