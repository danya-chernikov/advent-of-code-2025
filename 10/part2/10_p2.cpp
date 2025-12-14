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

using u_ll = unsigned long long;

const std::string delim = " ";
const std::string delim2 = ",";
constexpr int MAX_EXP = 63;

class Machine
{
public:
    Machine(int its_id,
            int its_ind,
            int its_ind_width,
            size_t its_btn_num,
            std::vector< std::vector<int> > &btns,
            size_t its_jlt_num,
            std::vector<int> jlts) :
        id(its_id),
        indicator(its_ind),
        ind_width(its_ind_width),
        btn_num(its_btn_num),
        buttons(btns),
        jlt_num(its_jlt_num),
        joltages(jlts) {}

    ~Machine() {}

    int gid() const { return id; }
    int get_ind() const { return indicator; }
    int get_ind_width() const { return ind_width; }
    size_t get_btn_num() const { return btn_num; }
    size_t get_jlt_num() const { return jlt_num; }
    std::vector< std::vector<int> > &get_buttons() { return buttons; }
    std::vector<int> &get_joltages() { return joltages; }

    friend std::ostream& operator<<(std::ostream& out, const Machine &m);

private:
    int     id;
    int     indicator;
    int     ind_width;
    size_t  btn_num;
    size_t  jlt_num;

    std::vector< std::vector<int> > buttons;
    std::vector<int> joltages;
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
    out << " { ";
    for (size_t jlt_i = 0; jlt_i < m.jlt_num; ++jlt_i)
        out << m.joltages[jlt_i] << " ";
    out << " }";
    return out;
}

std::string dec_to_bin(int num);

int get_bits_num(int num);

void press_btns(std::vector< std::vector<int> > &btns,
                size_t btns_num,
                size_t jlt_num,
                std::vector<int> &jlts,
                u_ll scheme,
                int coef);

/* Comapres joltages values of two joltages' arrays assuming arrays'
 * sizes are equal */
bool joltages_equal(std::vector<int> &jlts1, std::vector<int> &jlts2);

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
        std::vector< std::vector<int> > btns; // temporary buttons
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
        // If we are here, `btn` contains the joltages
        // Let's get all joltages
        // Eliminate '{' nad '}' from our joltages
        btn.erase(btn.begin());
        btn.erase(btn.end() - 1);
        std::vector<int> jlts; // temporary joltages
        std::string priv_jolt{};
        size_t jlt_cnt = 0;
        while (btn.length() != 0)
        {
            int joltage_num;
            auto jolt_pos = btn.find(delim2);
            std::string joltage = btn.substr(0, jolt_pos);
            priv_jolt = joltage;
            std::stringstream ss(joltage);
            ss >> joltage_num;
            jlts.push_back(joltage_num);
            btn.erase(0, jolt_pos + delim2.length());
            ++jlt_cnt;
            if (jolt_pos == std::string::npos)
                break;
        }
        // Create corresponding machine object
        Machine m_tmp(m_cnt, ind_num, ind_width, btn_cnt, btns, jlt_cnt, jlts);
        machines.push_back(m_tmp);
        ++m_cnt;
    }
    in.close();
    for (const Machine &m : machines)
        std::cout << m << std::endl;

    int total_press = 0;
    for (size_t mi = 0; mi < machines.size(); ++mi)
    {
        std::cout << "mi = " << mi << std::endl;
        std::vector<int> press_num;
        // Numbers of presses when we got `real_ind`

        std::vector<int> &real_jlts = machines[mi].get_joltages();
        std::vector <std::vector<int> > &btns = machines[mi].get_buttons();
        size_t btns_num = machines[mi].get_btn_num();
        std::cout << "btns_num = " << btns_num << std::endl;
        size_t jlt_num = machines[mi].get_jlt_num();
        //size_t coef = MAX_EXP / btns_num; // 2 * coef must NOT exceed MAX_EXP
        size_t coef = 1;
        std::cout << "coef = " << coef << std::endl;
        u_ll max_scheme = pow(2, btns_num * coef);
        std::cout << "btns_num * coef = " << btns_num * coef << std::endl;
        std::cout << "max_scheme = " << max_scheme << std::endl;
        sleep(1);
        // The order of pressing buttons does not matter!
        for (int i = 1; i < 10; ++i)
        {
            std::cout << "i = " << i << std::endl;
            sleep(1);
            bool f_jlts_equal = false;
            for (int j = 0; j < i; ++j)
            {
                std::cout << "j = " << j << std::endl;
                for (u_ll scheme = 1; scheme < max_scheme; ++scheme)
                {
                    std::vector<int> test_jlts(jlt_num, 0);
                    std::cout << "scheme = " << scheme << std::endl;
                    //std::cout << "scheme = " << scheme << std::endl;
                    press_btns(btns, btns_num, jlt_num, test_jlts, scheme, coef);
                    std::cout << "test_jlts = {";
                    for (size_t q = 0; q < test_jlts.size(); ++q)
                        std::cout << test_jlts[q] << " ";
                    std::cout << "}\n";
                    sleep(1);
                    if (joltages_equal(test_jlts, real_jlts))
                    {
                        std::cout << "joltages are equal!\n";exit(1);
                        // Number of buttons we pressed is the number of 1 bits in `scheme`
                        int bits_num = get_bits_num(scheme);
                        std::cout << "bits_num = " << bits_num << std::endl;
                        press_num.push_back(bits_num);
                        f_jlts_equal = true;
                        break;
                    }
                }
                if (f_jlts_equal)
                    break;
            } // for (int j = 0; j < i; ++j)
            if (f_jlts_equal)
                break;
        } // for (int i = 1; i < 10; ++i)

        // Now let's just find the miminum in `press_num`
        //int min = *min_element(press_num.begin(), press_num.end());

        if (press_num.size() > 0)
        {
            int min = press_num[0];
            for (size_t i = 1; i < press_num.size(); ++i)
            {
                if (min > press_num[i])
                    min = press_num[i];
            }
            total_press += min;
        }

    } // for (size_t mi = 0; mi < machines.size(); ++mi)

    std::cout  << "Fewest button presses: " << total_press << std::endl;

    return 0;
}

bool joltages_equal(std::vector<int> &jlts1, std::vector<int> &jlts2)
{
    for (size_t i = 0; i < jlts1.size(); ++i)
        if (jlts1[i] != jlts2[i])
            return false;
    return true;
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
                size_t jlt_num,
                std::vector<int> &jlts,
                u_ll scheme,
                int coef)
{
    // We traverse `btns_num` bits in `scheme`
    // from less-significant to the most most
    for (size_t i = 0; i < btns_num; ++i)
    {
        if (scheme & 1) // If bit is set
        {
            // Traverse all indexes inside the button
            for (size_t bi = 0; bi < btns[i].size(); ++bi)
                ++jlts[btns[i][bi]];
        }
        scheme >>= 1; // Go to the next bit
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
