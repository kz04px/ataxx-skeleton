#include <iostream>
#include "search.hpp"

// Perform a search as specified in the options
void search(const Position &pos, const SearchOptions options)
{
    switch(options.type)
    {
        case SearchType::Time:
            std::cout << "btime: " << options.btime << std::endl;
            std::cout << "wtime: " << options.wtime << std::endl;
            std::cout << "binc: " << options.binc << std::endl;
            std::cout << "winc: " << options.winc << std::endl;
            std::cout << "movestogo: " << options.movestogo << std::endl;
            break;
        case SearchType::Depth:
            std::cout << "depth: " << options.depth << std::endl;
            break;
        case SearchType::Nodes:
            std::cout << "nodes: " << options.nodes << std::endl;
            break;
        case SearchType::Infinite:
            std::cout << "infinite" << std::endl;
            break;
        default:
            return;
    }
}
