#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <fstream>
#include <cmath>
#include <algorithm>

int main(int argc, char **argv)
{
    std::string	line{};
	size_t		sum;
	size_t		res;

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

	sum = 0;
    while (std::getline(in, line))
    {
		res = 0;
		std::cout << line << std::endl;
		std::vector<int> digits;

		for (char c : line)
			digits.push_back(c - '0');
		
		if (digits.size() == 1)
		{
			res = digits[0];
		}
		else if (digits.size() == 2)
		{
			res = (10 * (*(digits.end() - 2))) + *(digits.end() - 1);
		}
		else
		{
			int	max_pos = 0;
			int	max_pos2 = 0;
			for (int i = 1; i < digits.size(); ++i)
			{
				if (digits[max_pos] < digits[i])
					max_pos = i;
			}
			if (max_pos == digits.size() - 1)
			{
				max_pos2 = 0;
				for (int i = 1; i < max_pos; ++i)
				{
					if (digits[max_pos2] < digits[i])
						max_pos2 = i;
				}
				res = (10 * digits[max_pos2]) + digits[max_pos];
			}
			else
			{
				max_pos2 = max_pos + 1;
				for (int i = max_pos + 2; i < digits.size(); ++i)
				{
					if (digits[max_pos2] < digits[i])
						max_pos2 = i;
				}
				res = (10 * digits[max_pos]) + digits[max_pos2];
			}
		}
		sum += res;
    }
	std::cout << "Total joltage is: " << sum << std::endl;

    in.close();
    return 0;
}
