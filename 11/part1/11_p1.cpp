/* https://www.youtube.com/watch?v=KqUlbUdgcko&t=164s
 * One of my favorite channels about graph theory btw
 * */
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
#include <chrono>
#include <map>
#include <set>

#include <cstring>
#include <cmath>
#include <cctype>
#include <ctime>

#include <unistd.h>

const std::string delim = " ";

using u_ll = unsigned long long;
using matrix = std::vector< std::vector<int> >;

void    print_matrix(matrix m);
matrix  mul_matrix(matrix &a, matrix &b);

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

    // Reading for the first time
    std::string line;
    size_t path_cnt = 0;
    int vid = 0; // Vertex ID
    std::map<std::string, int> vertices; // Unique vertices
    while (std::getline(in, line))
    {
        // First let's read the first vertex
        auto pos = line.find(delim);
        std::string fvert = line.substr(0, pos);
        line.erase(0, pos + delim.length());
        fvert.erase(fvert.end() - 1); // Elminate ':'
        if (vertices.find(fvert) == vertices.end()) // If there is no such vertex
        {
            vertices.insert(std::make_pair(fvert, vid));
            ++vid;
        }

        // Then let's read all neighbours of the vertex
        int vert_cnt = 0;
        std::string priv{};
        while (line.length() != 0)
        {
            auto pos = line.find(delim);
            std::string vert = line.substr(0, pos);
            if (vert_cnt > 0 && vert == priv)
                break;
            if (vertices.find(vert) == vertices.end()) // If there is no such vertex
            {
                vertices.insert(std::make_pair(vert, vid));
                ++vid;
            }
            priv = vert;
            ++vert_cnt;
        }
        ++path_cnt;
    }
    in.close();

    std::cout << "Vertex number is: " << vid << std::endl;
    std::cout << std::endl;
    for (const auto &v : vertices)
        std::cout << v.first << " => " << v.second << std::endl;
    std::cout << std::endl;

    int m_w = vid;
    int m_h = m_w;
    int start = vertices["you"];
    int end = vertices["out"];
    matrix m(m_w, std::vector<int>(m_h, 0));

    // Let's open the file again
    std::ifstream in2{argv[1]};
    if (!in2)
    {
        std::cerr << "Coundn't open input.txt\n";
        return 1;
    }

    // Reading again
    path_cnt = 0;
    while (std::getline(in2, line))
    {
        // First let's read the first vertex
        auto pos = line.find(delim);
        std::string fvert = line.substr(0, pos);
        line.erase(0, pos + delim.length());
        fvert.erase(fvert.end() - 1); // Elminate ':'
        int fvert_num = vertices[fvert];
        // Then let's read all neighbours of the vertex
        int vert_cnt = 0;
        std::string priv{};
        while (line.length() != 0)
        {
            auto pos = line.find(delim);
            std::string vert = line.substr(0, pos);
            if (vert_cnt > 0 && vert == priv)
                break;
            line.erase(0, pos + delim.length());
            int vert_num = vertices[vert];
            m[fvert_num][vert_num] = 1; // We have oriented graph!
            priv = vert;
            ++vert_cnt;
        }
        ++path_cnt;
    }
    in2.close();

    u_ll total_path = m[start][end];
    matrix res = m;
    std::cout << 1 << " : " << total_path << std::endl;
    for (int exp_i = 2; exp_i < vid; ++exp_i)
    {
        std::cout << exp_i << " : ";
        res = mul_matrix(res, m);
        std::cout << res[start][end] << std::endl;
        total_path += res[start][end];
    }

    std::cout << total_path << " paths lead from you to out\n";

    return 0;
}

void print_matrix(matrix m)
{
    std::cout << std::endl;
    for (int ri = 0; ri < m.size(); ++ri)
    {
        for (int ci = 0; ci < m[ri].size(); ++ci)
            std::cout << m[ri][ci] << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/* Multiplies two square matrices a and b */
matrix mul_matrix(matrix &a, matrix &b)
{
    matrix res(b[0].size(), std::vector<int>(a.size(), 0));

    for (size_t b_ci_res = 0; b_ci_res < b[0].size(); ++b_ci_res)
    {
        for (size_t b_ri_res = 0; b_ri_res < b.size(); ++b_ri_res)
        {

            for (size_t b_ri = 0, a_ci = 0; b_ri < b.size(); ++b_ri, ++a_ci)
            {
                res[b_ri_res][b_ci_res] += a[b_ri_res][a_ci] * b[b_ri][b_ci_res];
            }

        } // for (int b_ri = 0; b_ri < b.size(); ++b_ri)

    } // for (int b_ci = 0; b_ci = b.size[0]; ++b_ci)

    return res;
}

/*matrix a = {
    { -2, 1 },
    {  5, 4 }
};
matrix b = {
    { 3 },
    { 1 }
};

matrix a = {
    {  2, -3 },
    {  4, -6 }
};
matrix b = {
    { 9, -6 },
    { 6, -4 }
};

matrix a = {
    {  5, 8, -4 },
    {  6, 9, -5 },
    {  4, 7, -3 }
};
matrix b = {
    { 3,  2, 5 },
    { 4, -1, 3 },
    { 9,  6, 5 }
}

int start = 1;
int end = 10;
matrix m = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//0
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//1
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//2
    { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//3
    { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },//4
    { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 },//5
    { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 },//6
    { 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0 },//7
    { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },//8
    { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },//9
    { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0 }//10
};*/
