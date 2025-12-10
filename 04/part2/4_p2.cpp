#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <unistd.h>

int main(int argc, char **argv)
{
    std::string line{};
    size_t      cnt;
    int         roll_cnt;
    size_t      rem_cnt;
    int         w;
    int         h;

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

    std::vector<std::string> v;
    while (std::getline(in, line))
        v.push_back(line);

    h = v.size();
    w = v[0].length();
    rem_cnt = 0;
    cnt = 1;
    while (cnt != 0)
    {
        cnt = 0;
        std::vector<int> r(w, 0);
        std::vector< std::vector<int> > m(h, r);
        for (int y = 0; y < h; ++y)
        {
            for (int x = 0; x < w; ++x)
            {
                roll_cnt = 0;
                if (v[y][x] == '@')
                {
                    // west
                    if (x - 1 >= 0)
                        if (v[y][x - 1] == '@')
                            ++roll_cnt;
                    // northwest
                    if (y - 1 >= 0 && x - 1 >= 0)
                        if (v[y - 1][x - 1] == '@')
                            ++roll_cnt;
                    // north
                    if (y - 1 >= 0)
                        if (v[y - 1][x] == '@')
                            ++roll_cnt;
                    // northeast
                    if (y - 1 >= 0 && x + 1 < w)
                        if (v[y - 1][x + 1] == '@')
                            ++roll_cnt;
                    // east
                    if (x + 1 < w)
                        if (v[y][x + 1] == '@')
                            ++roll_cnt;
                    // southeast
                    if (y + 1 < h && x + 1 < w)
                        if (v[y + 1][x + 1] == '@')
                            ++roll_cnt;
                    // south
                    if (y + 1 < h)
                        if (v[y + 1][x] == '@')
                            ++roll_cnt;
                    // southwest
                    if (y + 1 < h && x - 1 >= 0)
                        if (v[y + 1][x - 1] == '@')
                            ++roll_cnt;

                    if (roll_cnt < 4)
                    {
                        m[y][x] = 1;
                        ++cnt;
                    }

                } // if (v[y][x] == '@')

            } // for (int x = 0; x < w; ++x)

        } // for (int y = 0; y < h; ++y)

        rem_cnt += cnt;

        for (int y = 0; y < h; ++y)
        {
            for (int x = 0; x < w; ++x)
            {
                if (m[y][x])
                    v[y][x] = '.';
            }
        }

    } // while (cnt != 0)
    std::cout << "Total number of rolls we can remove: " << rem_cnt << std::endl;

    in.close();
    return 0;
}
