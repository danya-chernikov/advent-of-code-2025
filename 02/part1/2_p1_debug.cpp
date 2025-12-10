#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <fstream>
#include <cmath>

const std::string delim1 = ",";
const std::string delim2 = "-";

int digits_num(size_t num)
{
    int dgt_cnt = 0;
    
    if (num == 0)
        return 1;
    while (num != 0)
    {
        num /= 10;
        ++dgt_cnt;
    }
    return dgt_cnt;
}

/* On error returns -1 */
int get_pos_digit(size_t num, int pos)
{
    int dnum;

    dnum = digits_num(num);
    if (pos < 0 || pos > dnum - 1)
        return -1;
    for (int i = 0; i < dnum - pos - 1; ++i)
        num /= 10;
    return (num % 10);
}

int main(int argc, char **argv)
{
    std::string                 line{};
    std::vector<std::string>    ranges;
    size_t                      ids_sum;

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
        auto pos = line.find(delim1);
        while (pos != std::string::npos)
        {
            ranges.push_back(line.substr(0, pos));
            line.erase(0, pos + delim1.length());
            pos = line.find(delim1);
        }
        ranges.push_back(line);
    }
    ids_sum = 0;
    for (auto ri = ranges.begin(); ri != ranges.end(); ++ri)
    {
        std::string num1, num2;
        std::string s = *ri;
        auto pos = s.find(delim2); 
        num1 = s.substr(0, pos);
        s.erase(0, pos + delim2.length());
        num2 = s;
        size_t n1, n2;
        std::stringstream ss1(num1);
        std::stringstream ss2(num2);
        ss1 >> n1;
        ss2 >> n2;
        std::cout << n1 << " " << n2 << std::endl;
        for (size_t ni = n1; ni <= n2; ++ni) // `ni` is a number in the range
        {
            size_t test_num = ni;
            std::cout << "\ttest_num = " << test_num << std::endl;
            int dnum = digits_num(test_num);
            if (dnum % 2 == 0)
            {
                size_t fnp = 0, snp = 0; // the first and second number's halves
                for (int di = dnum / 2, exp = di - 1; di < dnum; ++di, --exp)
                {
                    snp += get_pos_digit(test_num, di)
                        * static_cast<size_t>(pow(10.0, static_cast<double>(exp)));
                }
                for (int di = 0; di < dnum / 2; ++di)
                    test_num /= 10;
                fnp = test_num;
                std::cout << "\t\tfnp = " << fnp << std::endl;
                std::cout << "\t\tsnp = " << snp << std::endl;
                if (fnp == snp)
                    ids_sum += ni;
            }

        } // for (size_t ni = n1; ni <= n2; ++ni)

    } // for (auto ri = ranges.begin(); ri != ranges.end(); ++ri)

    std::cout << "Invalid IDs' sum: " << ids_sum << std::endl;

    in.close();
    return 0;
}
