#include <iostream>
#include "options.hpp"

namespace Options
{
    std::unordered_map<std::string, Spin> spins;
    std::unordered_map<std::string, Check> checks;
    std::unordered_map<std::string, String> strings;
    std::unordered_map<std::string, Combo> combos;

    void set(const std::string &name, const std::string &value)
    {
        // Spins
        if(spins.find(name) != spins.end())
        {
            try
            {
                int n = std::stoi(value);
                spins[name].set(n);
            }
            catch(...)
            {
            }
        }
        // Checks
        else if(checks.find(name) != checks.end())
        {
            if(value == "true" || value == "True")
            {
                checks[name].set(true);
            }
            else
            {
                checks[name].set(false);
            }
        }
        // Strings
        else if(strings.find(name) != strings.end())
        {
            strings[name].set(value);
        }
        // Combos
        else if(combos.find(name) != combos.end())
        {
            combos[name].set(value);
        }
    }

    void print()
    {
        // Spins
        for(auto const &[key, entry] : spins)
        {
            std::cout << "option"
                      << " name " << key
                      << " type spin"
                      << " default " << entry.default_
                      << " min " << entry.min_
                      << " max " << entry.max_
                      << std::endl;
        }
        // Checks
        for(auto const &[key, entry] : checks)
        {
            std::cout << "option"
                      << " name " << key
                      << " type check"
                      << " default " << (entry.default_ ? "true" : "false")
                      << std::endl;
        }
        // Strings
        for(auto const &[key, entry] : strings)
        {
            std::cout << "option"
                      << " name " << key
                      << " type string"
                      << " default " << entry.default_
                      << std::endl;
        }
        // Combos
        for(auto const &[key, entry] : combos)
        {
            std::cout << "option"
                      << " name " << key
                      << " type combo"
                      << " default " << entry.default_
                      << std::endl;
        }
    }
}