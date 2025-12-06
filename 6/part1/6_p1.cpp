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

    for (int i = 0; i < col_num; ++i)
    {
        unsigned long long col_res = horiz[0][i]; 
        for (int j = 1; j < LINE_NUM; ++j)
        {
            if (ops[i] == '+')
                col_res += horiz[j][i];
            else
                col_res *= horiz[j][i];
        }
        std::cout << "col_res = " << col_res << std::endl;
        grand_total += col_res;
    }

    std::cout << "\nGrand total is: " << grand_total << std::endl;

    delete [] ops;

    for (int i = 0; i < LINE_NUM; ++i)
        delete [] horiz[i];
    delete [] horiz;

    in.close();
    return 0;
}
