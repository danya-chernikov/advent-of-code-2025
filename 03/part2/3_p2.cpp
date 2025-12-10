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
    std::string         line{};
	unsigned long long  total_joltage;

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
					* static_cast<size_t>(
                    pow(10.0, static_cast<double>(MIN_LINE_SIZE - di - 1))
                    );
			}
			total_joltage += joltage;
			continue;
		}

		it_cnt = 0;
		while (it_cnt < digits.size())
		{
			// Let's find the first maximum in the interval
			// [cur_pos; digits.size() - (MIN_LINE_SIZE - chosen)]
			max_pos = cur_pos;	
			for (int di = cur_pos + 1; di <= digits.size() - (MIN_LINE_SIZE - chosen); ++di)
			{
				if (digits[max_pos] < digits[di])
					max_pos = di;
			}
			max = digits[max_pos];

			// Let's skip all digits in [cur_pos; max_pos)
			skip += max_pos - cur_pos;

			// Add found maximum digit to the current joltage
			joltage += static_cast<size_t>(max)
				* static_cast<size_t>(
                pow(10.0, static_cast<double>(MIN_LINE_SIZE - chosen - 1))
                );

			++chosen;

			// Update current position
			cur_pos = max_pos + 1;

			// Count how many digits remain to process
			rest = digits.size() - skip - chosen;

			if (chosen == MIN_LINE_SIZE)
				break;

			if (chosen + rest == MIN_LINE_SIZE)
			{
				for (int di = cur_pos; di < line.size(); ++di)
				{
					joltage += static_cast<size_t>(digits[di])
						* static_cast<size_t>(
                        pow(10.0, static_cast<double>(MIN_LINE_SIZE - chosen - 1))
                        );
					++chosen;
				}
				break;
			}

			++it_cnt;
		}
		total_joltage += joltage;
    }
	std::cout << "Total joltage = " << total_joltage << std::endl;

    in.close();
    return 0;
}
