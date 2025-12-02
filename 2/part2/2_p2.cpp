#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <fstream>
#include <cmath>

const std::string delim1 = ",";
const std::string delim2 = "-";

int digits_num(unsigned long long num)
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

/* Returns the digit located in position
 * `pos` in the number `num`. On error
 * returns -1 */
int get_pos_digit(unsigned long long num, int pos)
{
    int dnum;

    dnum = digits_num(num);
    if (pos < 0 || pos > dnum - 1)
        return -1;
    for (int i = 0; i < dnum - pos - 1; ++i)
        num /= 10;
    return (num % 10);
}

int check_all_digits_equal(unsigned long long num)
{
    int priv;

    priv = get_pos_digit(num, 0);
    for (int i = 1; i < digits_num(num); ++i)
    {
        if (priv != get_pos_digit(num, i))
            return 0;
    }
    return 1;
}

/* Returns 0 if number consists of odd digits amount */
int div_num_two_parts(unsigned long long num, size_t &fnp, size_t &snp)
{
    int dnum = digits_num(num);
    fnp = 0, snp = 0;
    if (dnum % 2 == 0)
    {
        for (int di = dnum / 2, exp = di - 1; di < dnum; ++di, --exp)
        {
            snp += get_pos_digit(num, di)
                * static_cast<size_t>(pow(10.0, static_cast<double>(exp)));
        }
        for (int di = 0; di < dnum / 2; ++di)
            num /= 10;
        fnp = num;
        return 1;
    }
    return 0;
}

/* Returns 0 in case of an error */
int div_num_three_parts(unsigned long long num, size_t &fnp, size_t &snp, size_t &tnp)
{
    int dnum = digits_num(num);
    fnp = 0, snp = 0, tnp = 0;
    if (dnum % 3 == 0)
    {
        for (int di = 2 * (dnum / 3), exp = (dnum / 3) - 1; di < dnum; ++di, --exp)
        {
            tnp += get_pos_digit(num, di)
                * static_cast<size_t>( pow(10.0, static_cast<double>(exp)) );
        }
        for (int di = 0; di < dnum / 3; ++di)
            num /= 10;
        div_num_two_parts(num, fnp, snp);
        return 1;
    }
    return 0;
}

int div_num_common(unsigned long long num, size_t *parts, int parts_num)
{
    int dnum = digits_num(num);
    memset(parts, 0, sizeof (size_t) * parts_num);
    if (dnum % parts_num != 0)
        return 0;
    for (int pi = parts_num - 1; pi >= 1; --pi)
    {
        for (int di = pi * (dnum / parts_num), exp = (dnum / parts_num) - 1; di < dnum; di++, --exp)
        {
            parts[pi] += get_pos_digit(num, di)
                     * static_cast<size_t>( pow(10.0, static_cast<double>(exp)) );
        }
        for (int di = 0; di < dnum / parts_num; ++di)
            num /= 10;
    }
    parts[0] = num;
    return 1;
}

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
        for (size_t ni = n1; ni <= n2; ++ni) // `ni` is a number in the range
        {
            // the first, second and third number's parts
            size_t fnp = 0, snp = 0, tnp = 0;
    
            // "Now, an ID is invalid if
            // it is made only of some
            // sequence of digits
            // repeated AT LEAST twice"
            if (digits_num(ni) == 1)
                continue;
            // First divide into halfs
            if (div_num_two_parts(ni, fnp, snp))
            {
                if (fnp == snp)
                {
                    ids_sum += ni;
                    continue;
                }
            }
            // Then try to divide into three equal parts
            if (div_num_three_parts(ni, fnp, snp, tnp))
            {
                if (fnp == snp && snp == tnp)
                {
                    ids_sum += ni;
                    continue;
                }
            }

            // Then try to divide into five equal parts
            size_t *p = new size_t [5];
            if (div_num_common(ni, p, 5))
            {
                if (p[0] == p[1] &&
                    p[1] == p[2] &&
                    p[2] == p[3] &&
                    p[3] == p[4])
                {
                    ids_sum += ni;
                    continue;
                }
            }
            delete [] p;
            p = nullptr;

            // Then try to divide into seven equal parts
            p = new size_t [7];
            if (div_num_common(ni, p, 7))
            {
                if (p[0] == p[1] &&
                    p[1] == p[2] &&
                    p[2] == p[3] &&
                    p[3] == p[4] &&
                    p[4] == p[5] &&
                    p[5] == p[6])
                {
                    ids_sum += ni;
                    continue;
                }
            }
            delete [] p;
            p = nullptr;

            // Check if all number's digits are equal
            if (check_all_digits_equal(ni))
            {
                ids_sum += ni;
            }

        } // for (size_t ni = n1; ni <= n2; ++ni)

    } // for (auto ri = ranges.begin(); ri != ranges.end(); ++ri)

    std::cout << "Invalid IDs' sum: " << ids_sum << std::endl;

    in.close();
    return 0;
}
