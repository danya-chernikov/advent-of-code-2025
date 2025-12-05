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

int main(int argc, char **argv)
{
    std::string line{};
    std::vector< std::pair<u_ll, u_ll> > r;

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

        r.push_back(std::make_pair(i1, i2));
    }
    in.close();

    std::sort(r.begin(), r.end(), [](auto &left, auto &right) {
        return left.first < right.first;
    });

    // First, let's remove repeating intervals
    r.erase(std::unique(r.begin(), r.end()), r.end());

    for (int i = 0; i < r.size(); ++i)
        std::cout << r[i].first << " " << r[i].second << std::endl;
    std::cout << std::endl;

    bool    f_overlay   = false;
    bool    f_long_int  = false;
    int     li_ind      = -1;
    size_t  cnt         = 0;

    cnt += r[0].second - r[0].first + 1;
    for (int ri = 1; ri < r.size(); ++ri)
    {
        // It means r[i] is inside r[i - 1]
        // and we have to ignore r[i]
        if (r[ri].second <= r[ri - 1].second)
        {
            f_long_int = true;
            // Let's go furhter skipping all other
            // intervals located inside r[i - 1]

            // Long interval index
            // (`long interval` is an
            // interval containing at
            // least one other interval
            // entirely)
            li_ind = ri - 1;
            int rj;
            for (rj = ri + 1; r[rj].second <= r[li_ind].second && rj < r.size(); ++rj) {}
            ri = rj;
            //cnt += r[li_ind].second - r[li_ind].first + 1;
            if (rj == r.size()) // We just reached the end
            {
                std::cout << "The last interval\n";
                break;
            }
        }
        // If we are here it means r[ri].second > r[li_ind].second
        
        // If the previous interval r[ri] overlaped with was a long interval
        if (f_long_int)
        {
            if (r[ri].first < r[li_ind].second)
                f_overlay = true;
            else if (r[ri].first == r[li_ind].second)
            {
                --cnt;
                f_overlay = false;
            }
            else
                f_overlay = false;

            cnt += r[ri].second - r[ri].first + 1;
            if (f_overlay)
                cnt -= r[li_ind].second - r[ri].first + 1;
        }
        else
        {
            if (r[ri].first < r[ri - 1].second)
                f_overlay = true;
            else if (r[ri].first == r[ri - 1].second)
            {
                --cnt;
                f_overlay = false;
            }
            else
                f_overlay = false;

            cnt += r[ri].second - r[ri].first + 1;
            if (f_overlay)
                cnt -= r[ri - 1].second - r[ri].first + 1;
        }

        if (f_long_int && r[ri].second > r[li_ind].second)
            f_long_int = false;

    }
    
    std::cout << "Number of fresh ingredient IDs: " << cnt << std::endl;

    return 0;
}
