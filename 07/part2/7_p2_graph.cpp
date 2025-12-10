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

constexpr char SPLIT    = '^';
constexpr char EMPTY    = '.';
constexpr char START    = 'S';
constexpr char MARK     = '|';
constexpr char VISIT    = '*';

class Vertex
{
public:
    Vertex(int num, int x, int y)
    {
        this->num = num;
        this->x = x;
        this->y = y;
    }
    ~Vertex() {}

    int get_num() const { return num; }
    int get_x() const { return x; }
    int get_y() const { return y; }

private:
    int num;
    int x;
    int y;
};

int get_vert_by_coords(std::vector<Vertex> &v, int x, int y);

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

    int w;
    int h;
    int line_cnt = 0;
    std::string line;
    while (std::getline(in, line))
    {
        w = line.length();
        ++line_cnt;
    }
    h = line_cnt;

    unsigned long long tl_cnt = 0; // timelines counter
    int split_cnt = 0;

    std::vector< std::vector<char> > m(h, std::vector<char> (w, 0));

    in.close();
    in.open(argv[1], std::fstream::in);
    if (!in)
    {
        std::cerr << "Coundn't open input.txt\n";
        return 1;
    }

    line_cnt = 0;
    while (std::getline(in, line))
    {
        for (int i = 0; i < line.length(); ++i)
            m[line_cnt][i] = line[i];
        ++line_cnt;
    }

    /*for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
            std::cout << m[i][j];
        std::cout << std::endl;
    }
    std::cout << std::endl;*/

    // First, let's find start point of the beam
    int s_x = -1;
    int s_y = -1;
    for (int ri = 0; ri < h; ++ri)
    {
        for (int ci = 0; ci < w; ++ci)
        {
            if (m[ri][ci] == START)
            {
                s_x = ci;
                s_y = ri;
                break;
            }
        }
    }
    if (s_x == -1 || s_y == -1)
    {
        std::cerr << "Splitter wasn't found\n";
        in.close();
        return 1;
    }

    // Let's create another matrix where we'll be marking
    // which splitters we have already counted
    std::vector< std::vector<int> > v(h, std::vector<int>(w, 0));

    if (m[s_y + 1][s_x] == EMPTY)
    {
        m[s_y + 1][s_x] = MARK; // mark this cell
    }
    for (int ri = s_y + 1; ri < h; ++ri)
    {
        for (int ci = 0; ci < w; ++ci)
        {
            if (m[ri][ci] == MARK)
            {
                if (ri + 1 < h)
                {
                    if (m[ri + 1][ci] == EMPTY)
                        m[ri + 1][ci] = MARK;
                    else if (m[ri + 1][ci] == SPLIT && !v[ri + 1][ci])
                    {
                        v[ri + 1][ci] = 1;
                        if (ci - 1 >= 0)
                            m[ri + 1][ci - 1] = MARK;
                        if (ci + 1 < w) 
                            m[ri + 1][ci + 1] = MARK;
                        ++split_cnt;
                    }
                }
            }
            else if (m[ri][ci] == SPLIT && !v[ri][ci])
            {
                if (m[ri - 1][ci] == MARK)
                {
                    v[ri][ci] = 1;
                    if (ci - 1 >= 0)
                        m[s_y + 1][s_x - 1] = MARK;
                    if (s_x + 1 < w) 
                        m[s_y + 1][s_x + 1] = MARK;
                    ++split_cnt;
                }
            }

        } // for (int ci = 0; ci < w; ++ci)

    } // for (int ri = s_y + 1; ri < h; ++ri)

    // Let's count number of exits (number of '|' at the last row)
    int exits_num = 0;
    for (int i = 0; i < w; ++i)
        if (m[h - 1][i] == '|')
            ++exits_num;
    std::cout << "exits_num = " << exits_num << std::endl;
    sleep(5);

    // Let's create vector of vertices and populate it
    std::vector <Vertex> vertices;
    int v_cnt = 0; // found vertex counter
    for (int ri = 2; ri < h; ri += 2)
    {
        for (int ci = 0; ci < w; ++ci)
        {
            if (m[ri][ci] == '^' && m[ri - 1][ci] == '|')
            {
                Vertex v(v_cnt, ci, ri);
                vertices.push_back(v);
                ++v_cnt; 
            }
        }
    }
    for (int ci = 0; ci < w; ++ci)
    {
        if (m[h - 1][ci] == '|')
        {
            Vertex v(v_cnt, ci, h - 1);
            vertices.push_back(v);
            ++v_cnt; 
        }
    }
    std::cout << "Vertices:\n";
    for (int vi = 0; vi < vertices.size(); ++vi)
    {
        std::cout << vertices[vi].get_num()  << '\t'
                  << vertices[vi].get_x()   << '\t'
                  << vertices[vi].get_y()   << std::endl;
    }
    std::cout << std::endl;
    sleep(20);

    // Let's create the graph and populate it
    int v_num = split_cnt + exits_num;
    std::cout << "v_num = " << v_num << std::endl;
    sleep(10);
    std::vector< std::vector <int> > g(v_num, std::vector <int> (v_num, 0));
    for (int ri = 2; ri < h; ++ri)
    {
        for (int ci = 0; ci < w; ++ci)
        {
            if (m[ri][ci] == '^')
            {
                int parent = get_vert_by_coords(vertices, ci, ri);
                if (parent != -1)
                {
                    int q = 0;
                    while (ri + q < h && m[ri + q][ci - 1] != '^')
                        ++q;
                    if (ri + q == h)
                        --q;
                    int l_child = get_vert_by_coords(vertices, ci - 1, ri + q);
                    if (l_child == -1)
                    {
                        std::cout << "l_child == -1!\n";
                        exit(1);
                    }

                    q = 0;
                    while (ri + q < h && m[ri + q][ci + 1] != '^')
                        ++q;
                    if (ri + q == h)
                        --q;
                    int r_child = get_vert_by_coords(vertices, ci + 1, ri + q);
                    if (r_child == -1)
                    {
                        std::cout << "r_child == -1!\n";
                        exit(1);
                    }
                    
                    g[parent][l_child] = 1;
                    //g[l_child][parent] = 1;
                    g[parent][r_child] = 1;
                    //g[r_child][parent] = 1;
                }
            }
        }
    }
    std::cout << "Graph:\n";
    for (int ri = 0; ri < v_num; ++ri)
    {
        for (int ci = 0; ci < v_num; ++ci)
            std::cout << g[ri][ci];
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::vector<long long> routes(v_num, 0);
    routes[1] = 1;
    routes[2] = 1;

    // Now finally let's count number of routes in the graph
    for (int ri = 1; ri < v_num; ++ri)
    {
        for (int ci = ri; ci < v_num; ++ci)
        {
            if (g[ri][ci] > 0)
            {
                std::cout << "We can reach " << ci << " from " << ri << std::endl;
                // Let's add to ci the `power` of ri
                routes[ci] += routes[ri];
                g[ci][ri] += routes[ci];
            }
        }
    }

    std::cout << "Routes:\n";
    for (int i = 0; i < v_num; ++i)
        std::cout << routes[i] << std::endl;

    std::cout << std::endl;
    std::cout << "Graph:\n";
    for (int ri = 0; ri < v_num; ++ri)
    {
        for (int ci = 0; ci < v_num; ++ci)
            std::cout << g[ri][ci];
        std::cout << std::endl;
    }
    std::cout << std::endl;

    for (int i = split_cnt; i < v_num; ++i)
        tl_cnt += routes[i];
    
    // Initial position
    int x = s_x;
    int y = s_y + 1;

    std::cout << "The beam will be split " << tl_cnt << " times\n";

    in.close();
    return 0;
}

/* Returns -1 if there is no vertex with such coordinates */
int get_vert_by_coords(std::vector<Vertex> &v, int x, int y)
{
    for (int vi = 0; vi < v.size(); ++vi)
    {
        if (v[vi].get_x() == x && v[vi].get_y() == y)
            return v[vi].get_num();
    }
    return -1;
}
