#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <fstream>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <utility>
#include <unistd.h>

constexpr char SPLIT    = '^';
constexpr char EMPTY    = '.';
constexpr char START    = 'S';
constexpr char MARK     = '|';

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

    int split_cnt = 0;
    int w;
    int h;
    int line_cnt = 0;
    std::string line;
    while (std::getline(in, line))
    {
        w = line.length();
        ++line_cnt;
    }
    h = line_cnt;

    if (h > 1)
    {
        std::vector< std::vector<char> > m(h, std::vector<char> (w, 0));

        in.close();
        in.open(argv[1], std::fstream::in);
        if (!in)
        {
            std::cerr << "Coundn't open input.txt\n";
            return 1;
        }

        line_cnt = 0;
        while (std::getline(in, line))
        {
            for (int i = 0; i < line.length(); ++i)
                m[line_cnt][i] = line[i];
            ++line_cnt;
        }

        for (int i = 0; i < h; ++i)
        {
            for (int j = 0; j < w; ++j)
                std::cout << m[i][j];
            std::cout << std::endl;
        }
        std::cout << std::endl;

        // First, let's find start point of the beam
        int s_x = -1;
        int s_y = -1;
        for (int ri = 0; ri < h; ++ri)
        {
            for (int ci = 0; ci < w; ++ci)
            {
                if (m[ri][ci] == START)
                {
                    s_x = ci;
                    s_y = ri;
                    break;
                }
            }
        }
        if (s_x == -1 || s_y == -1)
        {
            std::cerr << "Splitter wasn't found\n";
            in.close();
            return 1;
        }
        std::cout << "s_x = " << s_x << std::endl;
        std::cout << "s_y = " << s_y << std::endl;

        // Let's create another matrix where we'll be marking
        // which splitters we have already counted
        std::vector< std::vector<int> > v(h, std::vector<int>(w, 0));

        if (s_y < h - 1)
        {
            if (m[s_y + 1][s_x] == EMPTY)
            {
                m[s_y + 1][s_x] = MARK; // mark this cell
            }
            for (int ri = s_y + 1; ri < h; ++ri)
            {
                std::cout << "ri = " << ri << std::endl;
                for (int ci = 0; ci < w; ++ci)
                {
                    if (m[ri][ci] == MARK)
                    {
                        if (ri + 1 < h)
                        {
                            if (m[ri + 1][ci] == EMPTY)
                                m[ri + 1][ci] = MARK;
                            else if (m[ri + 1][ci] == SPLIT && !v[ri + 1][ci])
                            {
                                v[ri + 1][ci] = 1;
                                if (ci - 1 >= 0)
                                    m[ri + 1][ci - 1] = MARK;
                                if (ci + 1 < w) 
                                    m[ri + 1][ci + 1] = MARK;
                                ++split_cnt;
                            }
                        }
                    }
                    else if (m[ri][ci] == SPLIT && !v[ri][ci])
                    {
                        if (m[ri - 1][ci] == MARK)
                        {
                            v[ri][ci] = 1;
                            if (ci - 1 >= 0)
                                m[s_y + 1][s_x - 1] = MARK;
                            if (s_x + 1 < w) 
                                m[s_y + 1][s_x + 1] = MARK;
                            ++split_cnt;
                        }
                    }

                }

                for (int i = 0; i < h; ++i)
                {
                    for (int j = 0; j < w; ++j)
                        std::cout << m[i][j];
                    std::cout << std::endl;
                }
                std::cout << std::endl;

                for (int i = 0; i < h; ++i)
                {
                    for (int j = 0; j < w; ++j)
                        std::cout << v[i][j];
                    std::cout << std::endl;
                }
                std::cout << std::endl;

            }
        }
        // otherwise the beam will not be split at all

    }
    // otherwise the beam will not be split at all

    std::cout << "The beam will be split " << split_cnt << " times\n";

    in.close();
    return 0;
}
