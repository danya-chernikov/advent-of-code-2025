#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <utility>
#include <forward_list>
#include <list>
#include <array>
#include <bitset>
#include <chrono>
#include <map>
#include <set>

#include <cstring>
#include <cmath>
#include <cctype>
#include <ctime>

#include <unistd.h>

const std::string delim = " ";

using u_ll = unsigned long long;
using matrix = std::vector< std::vector<int> >;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file>\n";
        return 1;
    }

    std::ifstream in{argv[1]};
    if (!in)
    {
        std::cerr << "Coundn't open input.txt\n";
        return 1;
    }

    std::string line;
    while (std::getline(in, line))
    {

    }
    in.close();

    // Let's generate all permutations of the fixed-size sequence of elements
    std::vector<int> elems = { 1, 2, 3, 4 }; // initial permutation (must be sorted)
    
    do
    {
        std::cout << elems[0] << elems[1] << elems[2] << elems[3] << std::endl;
    } while (std::next_permutation(elems.begin(), elems.end()));

    int total_regions = 0;

    std::cout << total_regions << " regions can fit all of the presents listed\n";

    return 0;
}
