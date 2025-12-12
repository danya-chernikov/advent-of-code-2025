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

int dac, fft;

void dfs(matrix &graph,
         int v,
         int v_end,
         std::vector<bool> &visited,
         std::vector< std::vector<int> > &paths,
         size_t &path_cnt);

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
    int start = vertices["srv"];
    int end = vertices["out"];
    dac = vertices["dac"];
    fft = vertices["fft"];
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
            /*if (fvert_num == fft || vert_num == fft || fvert_num == dac || vert_num == dac)
            {
                std::cout << "m[fvert_num][vert_num] = " << "m[" << fvert_num << "]["
                    << vert_num << "] = 1\n";
                sleep(1);
            }*/
            m[fvert_num][vert_num] = 1; // We have oriented graph!
            priv = vert;
            ++vert_cnt;
        }
        ++path_cnt;
    }
    in2.close();

    /*for (int ri = 0; ri < m_h; ++ri)
    {
        for (int ci = 0; ci < m_w; ++ci)
            std::cout << m[ri][ci];
        std::cout << std::endl;
    }*/

    u_ll paths_num = 0;
    std::vector<bool> visited(m_w, 0);
    std::vector< std::vector<int> > paths(32000);

    path_cnt = 0;
    dfs(m, start, end, visited, paths, path_cnt);

    std::cout << "path_cnt = " << path_cnt << std::endl;
    std::cout << std::endl;
    for (size_t pi = 0; pi < path_cnt; ++pi)
    {
        for (size_t vi = 0; vi < paths[pi].size(); ++vi)
            std::cout << paths[pi][vi] << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;

    for (size_t pi = 0; pi < path_cnt; ++pi)
    {
        bool f_dac = false;
        bool f_fft = false;
        for (size_t vi = 0; vi < paths[pi].size(); ++vi)
        {
            if (paths[pi][vi] == dac)
                f_dac = true;
            else if (paths[pi][vi] == fft)
                f_fft = true;
        }
        if (f_dac && f_fft)
            ++paths_num;
    }

    std::cout << paths_num << " paths visit both dac and fft\n";

    return 0;
}

/*void dfs(matrix &graph,
         int v,
         int v_end,
         std::vector<bool> &visited,
         std::vector< std::vector<int> > &paths,
         size_t &path_cnt)
{
    if (v == v_end)
    {
        //visited[v] = false; ?
        // Just copy all content of paths[path_cnt] in paths[path_cnt + 1]
        // exept the last vertex visited
        for (size_t i = 0; i < paths[path_cnt].size(); ++i)
            paths[path_cnt + 1].push_back(paths[path_cnt][i]);
        ++path_cnt;
        return;
    }
    if (v==dac||v==fft)
    {
        std::cout << "FFFFFFF!\n";
        exit(1);
    }
    visited[v] = true;
    paths[path_cnt].push_back(v);
    for (size_t to = 0; to < graph.size(); ++to)
    {
        if (graph[v][to] && !visited[to])
            dfs(graph, to, v_end, visited, paths, path_cnt);
    }
    visited[v] = false;
    paths[path_cnt].pop_back();
}*/

void dfs(matrix &graph,
         int v,
         int v_end,
         std::vector<bool> &visited,
         std::vector< std::vector<int> > &paths,
         size_t &path_cnt)
{
    std::cout << v << std::endl;
    if (v==dac||v==fft)
    {
        std::cout << "FFFFFFF!\n";
        exit(1);
    }
    visited[v] = true;
    for (size_t to = 0; to < graph.size(); ++to)
    {
        if (graph[v][to] && !visited[to])
            dfs(graph, to, v_end, visited, paths, path_cnt);
    }
}
