#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <fstream>
#include <cmath>
#include <algorithm>

constexpr int MIN_LINE_SIZE = 12;

int main(int argc, char **argv)
{
    std::string	line{};
	size_t		total_joltage;

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

	total_joltage = 0;
    while (std::getline(in, line))
    {
		std::vector<int> digits;

		size_t	joltage;
		int		it_cnt;
		int		cur_pos;
		int		chosen;
		int		skip;
		size_t	rest;

		int		max;
		int		max_pos;

		joltage = 0;
		cur_pos = 0;
		chosen = 0;
		skip = 0;
		rest = digits.size();

		std::cout << "line = " << line << std::endl;

		// Add all digits to the vector
		for (char c : line)
			digits.push_back(c - '0');

		if (digits.size() < MIN_LINE_SIZE)
		{
			std::cerr << "Line cannot be less than " << MIN_LINE_SIZE << std::endl;
			return 1;
		}
		if (digits.size() == MIN_LINE_SIZE)
		{
			for(int di = 0; di < digits.size(); ++di)
			{
				joltage += static_cast<size_t>(digits[di])
					* static_cast<size_t>(pow(10.0, static_cast<double>(MIN_LINE_SIZE - 1 - di)));
			}
			std::cout << "joltage = " << joltage << "\n\n";
			total_joltage += joltage;
			continue;
		}

		it_cnt = 0;
		while (it_cnt < digits.size())
		{
			std::cout << "\n#################### " << it_cnt + 1 << " ITERATION\n\n";

			// Let's find the first maximum in the interval
			// [cur_pos; digits.size() - (MIN_LINE_SIZE - chosen)]
			max_pos = cur_pos;	
			for (int di = cur_pos + 1; di <= digits.size() - (MIN_LINE_SIZE - chosen); ++di)
			{
				if (digits[max_pos] < digits[di])
					max_pos = di;
			}
			max = digits[max_pos];
			std::cout << "max = " << max << std::endl;
			std::cout << "max_pos = " << max_pos << std::endl;

			// Let's skip all digits in [cur_pos; max_pos)
			skip += max_pos - cur_pos;
			std::cout << "skip = max_pos - cur_pos = "
				<< max_pos << " - " << cur_pos << " = " << skip << std::endl;

			// Add found maximum digit to the current joltage
			joltage += static_cast<size_t>(max)
				* static_cast<size_t>(pow(10.0, static_cast<double>(MIN_LINE_SIZE - chosen)));

			// Increment number of digits we 
			++chosen;
			std::cout << "chosen = " << chosen << std::endl;

			// Update current position
			cur_pos = max_pos + 1;
			std::cout << "cur_pos = max_pos + 1 = "
				<< max_pos << " + 1 = " << cur_pos << std::endl;

			// Count how many digits remain to process
			rest = digits.size() - skip - chosen;
			std::cout << "rest = digits.size() - skip - chosen = "
				<< digits.size() << " - " << skip << " - " << chosen
				<< " = " << rest << std::endl;

			if (chosen == MIN_LINE_SIZE)
			{
				std::cout << "chosen == MIN_LINE_SIZE\n";
				std::cout << "break\n";
				break;
			}

			if (chosen + rest == MIN_LINE_SIZE)
			{
				std::cout << "chosen + rest == MIN_LINE_SIZE\n";
				std::cout << "Add all remaining digits\n";
				for (int di = cur_pos; di < line.size(); ++di)
				{
					joltage += static_cast<size_t>(digits[di])
						* static_cast<size_t>(pow(10.0, static_cast<double>(MIN_LINE_SIZE - chosen)));
					++chosen;
				}
				std::cout << "break\n";
				break;
			}

			++it_cnt;
		}
		std::cout << "joltage = " << joltage << "\n\n";
		total_joltage += joltage;
    }
	std::cout << "Total joltage = " << total_joltage << std::endl;

    in.close();
    return 0;
}
