#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <utility>
#include <forward_list>
#include <list>
#include <array>
#include <bitset>

#include <cstring>
#include <cmath>
#include <cctype>

#include <unistd.h>

const std::string delim = " ";
const std::string delim2 = ",";
constexpr int MAX_BTNS_NUM = 20;

class Machine
{
public:
    Machine(int its_id,
            int its_ind,
            int its_ind_width,
            size_t its_btn_num,
            std::vector< std::vector<int> > &btns) :
        id(its_id),
        indicator(its_ind),
        ind_width(its_ind_width),
        btn_num(its_btn_num),
        buttons(btns) {}

    ~Machine() {}

    int gid() const { return id; }
    int get_ind() const { return indicator; }
    int get_ind_width() const { return ind_width; }
    size_t get_btn_num() const { return btn_num; }
    std::vector< std::vector<int> > &get_buttons() { return buttons; }

    friend std::ostream& operator<<(std::ostream& out, const Machine &m);

private:
    int     id;
    int     indicator;
    int     ind_width;
    size_t  btn_num;

    std::vector< std::vector<int> > buttons;
};

std::ostream& operator<<(std::ostream& out, const Machine &m)
{
    out << m.indicator << " ";
    for (size_t btn_i = 0; btn_i < m.btn_num; ++btn_i)
    {
        out << "( ";
        for (size_t bit_i = 0; bit_i < m.buttons[btn_i].size(); ++bit_i)
            out << m.buttons[btn_i][bit_i] << " ";
        out << ")";
    }
    return out;
}

std::string dec_to_bin(int num);

int get_bits_num(int num);

void press_btns(std::vector< std::vector<int> > &buttons,
                size_t btns_num,
                int ind_width,
                int *num,
                int scheme);

void press_btn(std::vector< std::vector<int> > &btns,
               size_t btns_num,
               int ind_width,
               int btn_ind,
               int *num);

bool its_joltage(std::string str);

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

    std::vector<Machine> machines;
    
    std::string line;
    size_t m_cnt = 0; // Machine counter
    // Process each machine's line
    while (std::getline(in, line))
    {
        // First let's get the indicator light diagram
        auto ind_pos = line.find(delim);
        // Get the indicator substring
        std::string ind = line.substr(0, ind_pos);
        line.erase(0, ind_pos + delim.length());
        // Eliminate '[' and ']' from the indicator string
        ind.erase(ind.begin());
        ind.erase(ind.end() - 1);
        int ind_width = ind.length();
        // Compose the desired indicator number from its description
        // Let's say '#' will be 1, and '.' will be 0
        // so indicator value is just a number
        int ind_num = 0;
        int j = 0; 
        for (int i = ind.length() - 1; i >= 0; --i)
        {
            if (ind[i] == '#')
                ind_num |= (1 << j);
            ++j;
        }
        
        // Now let's handle buttons. We always have at least one button
        // In the first part of the task we just ignore joltages
        std::vector< std::vector<int> > btns;
        size_t btn_cnt = 0;
        // Get the first button
        auto btn_pos = line.find(delim);
        // The `rest` it's a substring containing buttons and joltage values
        std::string btn = line.substr(0, btn_pos);
        line.erase(0, btn_pos + delim.length());
        // Let's get all other buttons
        while (!its_joltage(btn))
        {
            ++btn_cnt;
            // Eliminate '(' nad ')' from the button
            btn.erase(btn.begin());
            btn.erase(btn.end() - 1);
            // Now let's divide the button by ',' to get current button's lights
            size_t light_cnt = 0;
            std::string priv_light{};
            while (btn.length() != 0)
            {
                int light_num;
                auto light_pos = btn.find(delim2);
                std::string light = btn.substr(0, light_pos);
                // If the string hasn't changed since previous iteration
                if (light_cnt > 0 && priv_light == light)
                    break;
                priv_light = light;
                std::stringstream ss(light);
                ss >> light_num;
                btns.push_back(std::vector<int> ());
                btns[btn_cnt - 1].push_back(light_num);
                btn.erase(0, light_pos + delim2.length());
                ++light_cnt;
            }
            // Let's get the next button
            auto btn_pos = line.find(delim);
            btn = line.substr(0, btn_pos);
            line.erase(0, btn_pos + delim.length());
        }
        
        // Create corresponding machine object
        Machine m_tmp(m_cnt, ind_num, ind_width, btn_cnt, btns);
        machines.push_back(m_tmp);

        ++m_cnt;
    }

    int total_press = 0;

    for (size_t mi = 0; mi < machines.size(); ++mi)
    {
        std::vector<int> press_num;
        // Numbers of presses when we got `real_ind`

        int real_ind = machines[mi].get_ind();
        int max_scheme = pow(2, machines[mi].get_btn_num());

        for (int scheme = 1; scheme < max_scheme; ++scheme)
        {
            size_t btn_num = machines[mi].get_btn_num();
            int ind_width = machines[mi].get_ind_width();
            std::vector <std::vector<int> > &btns = machines[mi].get_buttons();

            int test_ind = 0;
            press_btns(btns, btn_num, ind_width, &test_ind, scheme);

            if (test_ind == real_ind)
            {
                // Number of buttons we pressed is the number of 1 bits in `scheme`
                int bits_num = get_bits_num(scheme);
                press_num.push_back(bits_num);
            }
        }
        // Now let's just find the miminum in `press_num`

        int min = press_num[0];
        for (size_t i = 1; i < press_num.size(); ++i)
        {
            if (min > press_num[i])
                min = press_num[i];
        }
        total_press += min;

    } // for (size_t mi = 0; mi < machines.size(); ++mi)

    std::cout  << "Fewest button presses: " << total_press << std::endl;

    in.close();
    return 0;
}

std::string dec_to_bin(int num)
{
    std::string res;

    for (int i = (sizeof(int) * 8) - 1; i >= 0; --i)
        (num & (1 << i)) ? res += '1' : res += '0';
    return res;
}

/* Returns number of bits
 * in number `num` */
int get_bits_num(int num)
{
    int cnt = 0;

    for (size_t bi = 0; bi < sizeof (int) * 8; ++bi)
    {
        if (num & 1)
            ++cnt;
        num >>= 1;
    }
    return cnt;
}

/* Presses all the buttons
 * according to `scheme`. Bits
 * in `scheme` indicate which
 * buttons to press.
 * For example:
 * We have 6 buttons. Each button has
 * it's own index in the buttons array
 * If we use int of course there can not be more than 32 buttons
 *
 * scheme = 0 0 0 0 0 0 0 0    0 0 0 0 0 0 0 0    0 0 0 0 0 0 0 0    0 0 1 1 0 1 1 1
 *          |                  |                  |                  |
 *         31                 23                 15                  7
 *
 * scheme = 55
 *
 * We should press buttons with indexes: [0], [1], [2], [4], [5]
 * */
void press_btns(std::vector< std::vector<int> > &btns,
                size_t btns_num,
                int ind_width,
                int *num,
                int scheme)
{
    // We traverse `btns_num` bits in `scheme` from less-significant to the most most
    for (size_t i = 0; i < btns_num; ++i)
    {
        if (scheme & 1) // If bit is set
            press_btn(btns, btns_num, ind_width, i, num);
        scheme >>= 1; // Go to the next bit
    }
}

void press_btn(std::vector< std::vector<int> > &btns,
               size_t btns_num,
               int ind_width,
               int btn_ind,
               int *num)
{
    for (size_t bi = 0; bi < btns[btn_ind].size(); ++bi)
    {
        int bit_ind = static_cast<int>(ind_width) - btns[btn_ind][bi] - 1;
        if (*num & (1 << bit_ind)) // if bit is set
            *num &= ~((unsigned int)1 << bit_ind); // unset it (make it 0)
        else // if bit is unset
            *num |= ((unsigned int)1 << bit_ind); // set it (make it 1)
    }
}

/* Checks whether the string `str`
 * contains a curly brace, which
 * indicates that the string represents
 * joltage values and should be ignored
 * in this part of the task */
bool its_joltage(std::string str)
{
    for (char c : str)
        if (c == '{')
            return true;
    return false;
}
