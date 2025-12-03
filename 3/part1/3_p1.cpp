#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <fstream>
#include <cmath>

int main(int argc, char **argv)
{
    std::string                 line{};
    std::vector<std::string>    ranges;
    unsigned long long          ids_sum;

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
        int tens_max = -1;
        int ones_max = -1;
        int f_last_max = 0;
        std::cout << line << ": ";
        int max_pos = 0;
        for (int i = 1; i < line.size(); ++i)
            if (line[max_pos] - '0' < line[i] - '0')
                max_pos = i;
        std::cout << "max_pos = " << max_pos << " ";
        /*if (max_pos == line.size() - 1)
        {
            ones_max = line[max_pos];
            f_last_max = 1;
        } 
        else
            tens_max = line[max_pos];*/
        int first_max_val = line[max_pos];
        line[max_pos] = '0';
        std::cout << " " << line << " ";
        int max_pos2 = 0;
        for (int i = 0; i < line.size() - 1; ++i)
        {
            if (line[i] - '0' < line[i + 1] - '0')
                max_pos2 = i + 1;
        }
        std::cout << "max_pos2 = " << max_pos2 << " ";
        if (max_pos2 < max_pos)
        {
            tens_max = line[max_pos2];
            ones_max = line[max_pos];
        }
        else
        {
            tens_max = line[max_pos];
            ones_max = line[max_pos2];
        }
        std::cout << tens_max << ones_max << std::endl;
    }

    in.close();
    return 0;
}
