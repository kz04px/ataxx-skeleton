#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <unordered_map>
#include <vector>

namespace Options
{
    // Integer value between min and max
    struct Spin
    {
        Spin()
        {
        }
        Spin(int min, int max, int d) : min_(min), max_(max), default_(d)
        {
            set(d);
        }
        void set(const int n)
        {
            if(n < min_)      {value_ = min_;}
            else if(n > max_) {value_ = max_;}
            else              {value_ = n;}
        }
        int get()
        {
            return value_;
        }
        int min_ = 0;
        int max_ = 0;
        int value_ = 0;
        int default_ = 0;
    };

    // Boolean value
    struct Check
    {
        Check()
        {
        }
        Check(bool d) : default_(d)
        {
            set(d);
        }
        void set(const bool n)
        {
            value_ = n;
        }
        bool get()
        {
            return value_;
        }
        bool value_ = false;
        bool default_ = false;
    };

    // String value
    struct String
    {
        String()
        {
        }
        String(const std::string &d) : default_(d)
        {
            set(d);
        }
        void set(const std::string &n)
        {
            value_ = n;
        }
        std::string get()
        {
            return value_;
        }
        std::string value_ = "";
        std::string default_ = "";
    };

    // String value from a list of values
    struct Combo
    {
        Combo()
        {
        }
        Combo(const std::string &d) : default_(d)
        {
            set(d);
        }
        void set(const std::string &n)
        {
            for(const auto &option : options_)
            {
                if(option == n)
                {
                    value_ = n;
                    return;
                }
            }
        }
        std::string get()
        {
            return value_;
        }
        std::string value_ = "";
        std::string default_ = "";
        std::vector<std::string> options_ = {};
    };

    extern std::unordered_map<std::string, Spin> spins;
    extern std::unordered_map<std::string, Check> checks;
    extern std::unordered_map<std::string, String> strings;
    extern std::unordered_map<std::string, Combo> combos;

    void set(const std::string &name, const std::string &value);
    void print();
}

#endif