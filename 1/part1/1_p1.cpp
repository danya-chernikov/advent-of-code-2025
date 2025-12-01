#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

const int DIAL_MOD = 99;

enum DIRECT { LEFT, RIGHT, NONE };

int main(int argc, char **argv)
{
    int         passwd;
    int         pos;
    int         dist;
    DIRECT      dir;
    std::string line{};

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

    dir = NONE;
    passwd = 0;
    dist = 0;
    pos = 50;
    while (std::getline(in, line))
    {
        if (line[0] == 'L')
            dir = LEFT;
        else if (line[0] == 'R')
            dir = RIGHT;
        line.erase(0, 1);
        dist = std::stoi(line);
        if (dir == LEFT)
        {
            if (pos - dist < 0)
            {
                if (pos - (dist % (DIAL_MOD + 1)) < 0)
                    pos = (DIAL_MOD + 1) - abs(pos - (dist % (DIAL_MOD + 1)));
                else
                    pos = pos - (dist % (DIAL_MOD + 1));
            }
            else
                pos -= dist;
        }
        else if (dir == RIGHT)
        {
            if (pos + dist > DIAL_MOD)
                pos = (pos + dist) % (DIAL_MOD + 1);
            else
                pos += dist;
        }
        if (pos == 0)
            ++passwd;
    }
    std::cout << "Password is: " << passwd << std::endl;
    in.close();
    return 0;
}
