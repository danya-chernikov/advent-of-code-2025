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
constexpr int       LINE_NUM    = 3;
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

    int line_cnt = 0;
    std::string lines[LINE_NUM];
    for (int li = 0; li < LINE_NUM; ++li)
    {
        std::getline(in, lines[li]);
    }
    std::string ops_line;
    std::getline(in, ops_line);

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

    int m_w = 0;
    int m_h = LINE_NUM + 1;
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
        m_w += digits_num(max);
        maxs.push_back(digits_num(max));
    }

    std::cout << "m_w = " << m_w << std::endl;
    std::cout << "m_h = " << m_h << std::endl;

    // Now let's create our matrix
    char **m = new char *[m_h];
    for (int i = 0; i < m_h; ++i)
        m[i] = new char [m_w];

    // First fill in the matrix with '0'
    // (That's why in the original data we don't have them!) 
    for (int i = 0; i < m_h; ++i)
        for (int j = 0; j < m_w; ++j)
            m[i][j] = '0';

    // Now let's fill in this matrix with digits
    for (int j = 0; j < LINE_NUM; ++j)
    {
        for (int i = 0; i < col_num; ++i)
        {
            std::string num = std::to_string(horiz[j][i]);
            for (int q = 0; q < num.length(); ++q)
            {

            }
        }
    }

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
