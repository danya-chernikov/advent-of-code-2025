#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <utility>
#include <unistd.h>

typedef unsigned long long  u_ll;
const std::string           delim = "-";

bool is_num_in_range(u_ll n, u_ll lint, u_ll rint)
{
    return (n >= lint && n <= rint);
}

int main(int argc, char **argv)
{
    std::string line{};
    size_t      cnt;
    std::vector< std::pair<u_ll, u_ll> > ranges;
    std::vector<u_ll> nums;

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

    while (std::getline(in, line))
    {
        std::string int1, int2;
        u_ll        i1, i2;

        if (!line.length())
            break;

        auto pos = line.find(delim);
        int1 = line.substr(0, pos);
        line.erase(0, pos + delim.length());
        int2 = line;
        std::stringstream ss1(int1);
        std::stringstream ss2(int2);
        ss1 >> i1;
        ss2 >> i2;

        ranges.push_back(std::make_pair(i1, i2));
    }

    /*for (int i = 0; i < ranges.size(); ++i)
    {
        std::cout << ranges[i].first << " " << ranges[i].second << std::endl;
    }
    std::cout << std::endl;
    for (u_ll i : nums)
    {
        std::cout << i << std::endl;
    }*/

    cnt = 0;
    for (int ni = 0; ni < nums.size(); ++ni)
    {
        for (int ri = 0; ri < ranges.size(); ++ri)
        {
            if (is_num_in_range(nums[ni], ranges[ri].first, ranges[ri].second))
            {
                ++cnt;
                break;
            }
        }
    }

    std::cout << "Number of available ingredient IDs: " << cnt << std::endl;

    in.close();
    return 0;
}
