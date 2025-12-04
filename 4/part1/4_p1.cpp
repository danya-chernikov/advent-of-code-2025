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

    cnt = 0;
    h = v.size();
    w = v[0].length();
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
                    ++cnt;
            }
        }
    }
    std::cout << "Number of rolls we can access: " << cnt << std::endl;

    in.close();
    return 0;
}
