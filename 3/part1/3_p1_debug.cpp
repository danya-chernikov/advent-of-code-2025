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

		// Add all digits to the vector
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
			// Let's find the first maximum number first
			// By 'first' we imply also, if for example
			// we have: 4732893159
			// the index of the msximum number will be 5
			int	max_pos = 0;
			int	max_pos2 = 0;
			for (int i = 1; i < digits.size(); ++i)
			{
				if (digits[max_pos] < digits[i])
					max_pos = i;
			}
			std::cout << "\tmax_pos = " << max_pos << std::endl;
			// If max_pos is the last index, we have
			// to move left to find the next maximum
			// It's guaranteed that in this case this
			// maximum number is unique in all bank
			if (max_pos == digits.size() - 1)
			{
				std::cout << "\tdigits[max_pos] is on the last index\n";
				std::cout << "\tgoing to the left...\n";
				max_pos2 = 0;
				for (int i = 1; i < max_pos; ++i)
				{
					if (digits[max_pos2] < digits[i])
						max_pos2 = i;
				}
				std::cout << "\tmax_pos2 = " << max_pos2 << std::endl;
				res = (10 * digits[max_pos2]) + digits[max_pos];
			}
			// It's still the first founded maximum
			// but it may have duplicates, if it has
			// at least one duplicate it will be located
			// somewhere on the right, in case if it hasn't
			// any duplicate, we still just have to go on
			// the right to find the next maximum
			// Otherwise, we have to move right
			else
			{
				std::cout << "\tdigits[max_pos] is somewhere is the middle\n";
				std::cout << "\tgoing to the right...\n";
				max_pos2 = max_pos + 1;
				for (int i = max_pos + 2; i < digits.size(); ++i)
				{
					if (digits[max_pos2] < digits[i])
						max_pos2 = i;
				}
				std::cout << "\tmax_pos2 = " << max_pos2 << std::endl;
				res = (10 * digits[max_pos]) + digits[max_pos2];
			}
		}
		std::cout << "\tres = " << res << std::endl;
		sum += res;
    }
	std::cout << "sum = " << sum << std::endl;

    in.close();
    return 0;
}
