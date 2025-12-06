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

const std::string   delim       = "\t";
constexpr int       LINE_NUM    = 4;
constexpr int       MAX_NUM_LEN = 20000;

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

    unsigned long long grand_total = 0;

    int m_w = 0;
    int m_h = LINE_NUM + 1;

    // Now let's create our matrix
    char **m = new char *[m_h];

    std::string lines[LINE_NUM];
    for (int li = 0; li < LINE_NUM; ++li)
    {
        std::getline(in, lines[li]);
        m_w = lines[li].length() + 1;
        m[li] = new char [m_w];
        for (int ci = 0; ci < m_w - 1; ++ci)
            m[li][ci] = lines[li][ci];
        m[li][m_w] = '\0';
    }
    m[LINE_NUM] = new char [m_w];
    std::string ops_line;
    std::getline(in, ops_line);
    for (int ci = 0; ci < m_w - 1; ++ci)
        m[LINE_NUM][ci] = ops_line[ci];
    m[LINE_NUM][m_w] = '\0';

    std::cout << std::endl;
    for (int i = 0; i < m_h; ++i)
    {
        for (int j = 0; j <= m_w; ++j)
            std::cout << m[i][j];
        std::cout << std::endl;
    }
    std::cout << std::endl;

    int **horiz = new int *[LINE_NUM];
    for (int i = 0; i < LINE_NUM; ++i)
        horiz[i] = new int [MAX_NUM_LEN];

    for (int i = 0; i < LINE_NUM; ++i)
    {
        for (int j = 0; j < MAX_NUM_LEN; ++j)
            horiz[i][j] = -1;
    }
    for (int li = 0; li < LINE_NUM; ++li)
    {
        int num_cnt = 0;
        int ci = 0;
        while (ci < lines[li].length())
        {
            char snum[10] = "";
            while (ci < lines[li].length() && !isdigit(lines[li][ci]))
                ++ci;
            int i = 0;
            while (ci < lines[li].length() && isdigit(lines[li][ci]))
            {
                snum[i] = lines[li][ci];
                ++ci;
                ++i;
            }
            snum[i] = '\0';
            while (ci < lines[li].length() && !isdigit(lines[li][ci]))
                ++ci;
            std::string s(snum);
            std::stringstream ss(s);
            int num;
            ss >> num;
            horiz[li][num_cnt] = num;
            ++num_cnt;
        }
    }

    int col_num = 0;
    while (horiz[0][col_num] != -1)
        ++col_num;

    std::cout << "col_num = " << col_num << std::endl;

    // Let's parse operators
    char *ops = new char [col_num];
    int ci = 0;
    int op_ind = 0;
    while (ci < ops_line.length())
    {
        while (ci < ops_line.length() && ops_line[ci] != '+' && ops_line[ci] != '*')
            ++ci;
        ops[op_ind] = ops_line[ci++];
        while (ci < ops_line.length() && ops_line[ci] != '+' && ops_line[ci] != '*')
            ++ci;
        ++op_ind;
    }

    for (int i = 0; i < LINE_NUM; ++i)
    {
        for (int j = 0; horiz[i][j] != -1; ++j)
            std::cout << horiz[i][j] << '\t';
        std::cout << std::endl;
    }

    for (int i = 0; i < col_num; ++i)
        std::cout << ops[i] << '\t';
    std::cout << std::endl;

    // Maximum number's length for each column
    std::vector<int> maxs;

    /* Let's find a maximum number in each horiz[][j],
    // count the number of digits in this maximum,
    // and create a new matrix of chars whose width
    // consists of all maximums' `length`s, where
    // `length` is the number of digits in a number.
    // Height of this matrix will be just LINE_NUM + 1
    // to include as the last row duplicated '*' and
    // '+' for each corresponding column of horiz[][j] */
    for (int i = 0; i < col_num; ++i)
    {
        int max_pos = 0;     
        int max;
        for (int j = 1; j < LINE_NUM; ++j)
        {
            if (horiz[max_pos][i] < horiz[j][i])
                max_pos = j;
            max = horiz[max_pos][i];
        }
        // Let's count number of digits in this maximum number
        // and add this to the matrix length
        maxs.push_back(digits_num(max));
    }

    std::cout << "m_w = " << m_w << std::endl;
    std::cout << "m_h = " << m_h << std::endl;

    int cl_cnt = 0; // column counter
    for (int i = 0; i <= m_w; ++i)
    {
        char op = m[LINE_NUM][i];
        int j;
        for (j = 0; j < maxs[cl_cnt]; ++j)
            m[LINE_NUM][i + j] = op;
        i += j;
        ++cl_cnt;
    }

    std::cout << std::endl;
    for (int i = 0; i < m_h; ++i)
    {
        for (int j = 0; j < m_w; ++j)
            std::cout << m[i][j];
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // Now let's count
    unsigned long long problem_res = 0;
    int num = 0;
    int exp = LINE_NUM - 1;
    // Let's substitute number of spaces at the bottom from the exponent
    int nzero_btm = 0;
    for (int i = m_h - 2; i >= 0; --i)
    {
        if (m[i][m_w - 2] != ' ')
            break;
        ++nzero_btm;
    }
    std::cout << "nzero_btm = " << nzero_btm << std::endl;
    exp -= nzero_btm;
    for (int i = 0; i < m_h - 1; ++i)
    {
        int digit = m[i][m_w - 2] - '0';
        if (m[i][m_w - 2] != ' ')
            num += (digit) * static_cast<int>(pow(10.0, static_cast<double>(exp)));
        std::cout << m[i][m_w - 2] << std::endl;
        --exp;
    }
    std::cout << "num = " << num << std::endl;
    problem_res = num;
    int f_next = 0;
    for (int j = m_w - 3; j >= 0; --j)
    {
        unsigned long num = 0;
        char op = m[m_h - 1][j];
        int exp = LINE_NUM - 1;
        int nzero_btm = 0;
        for (int i = m_h - 2; i >= 0; --i)
        {
            if (m[i][j] != ' ')
                break;
            ++nzero_btm;
        }
        std::cout << "nzero_btm = " << nzero_btm << std::endl;
        exp -= nzero_btm;
        if (f_next)
        {
            f_next = 0;
            if (op == '*')
                problem_res = 1;
            else
                problem_res = 0;
        }
        for (int i = 0; i < m_h - 1; ++i)
        {
            int digit = m[i][j] - '0';
            if (m[i][j] != ' ')
                num += (digit) * static_cast<int>(pow(10.0, static_cast<double>(exp)));
            std::cout << m[i][j] << std::endl;
            --exp;
        }
        std::cout << "num = " << num << std::endl;
        if (num != 0)
        {
            if (op == '*')
                problem_res *= num;
            else
                problem_res += num;
        }
        if (num == 0)
        {
            f_next = 1;
            std::cout << "problem_res = " << problem_res << std::endl;
            grand_total += problem_res;
        }
    }
    std::cout << "problem_res = " << problem_res << std::endl;
    grand_total += problem_res;

    std::cout << "\nGrand total is: " << grand_total << std::endl;

    for (int i = 0; i < m_h; ++i)
        delete [] m[i];
    delete [] m;

    delete [] ops;

    for (int i = 0; i < LINE_NUM; ++i)
        delete [] horiz[i];
    delete [] horiz;

    in.close();
    return 0;
}
