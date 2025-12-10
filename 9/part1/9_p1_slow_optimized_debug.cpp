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

#include <cstring>
#include <cmath>
#include <cctype>

#include <unistd.h>

using u_ll = long long;

const std::string delim = ",";

class Tile
{
public:
    Tile(int its_id, int its_x, int its_y) :
        id(its_id), x(its_x), y(its_y) {}

    Tile(int its_x, int its_y) : id(0), x(its_x), y(its_y) {}

    ~Tile() {}

    int gid() const { return id; }
    int gx() const { return x; }
    int gy() const { return y; }

    friend std::ostream& operator<<(std::ostream& out, const Tile &t);

private:
    int id;
    int x;
    int y;
};

std::ostream& operator<<(std::ostream& out, const Tile &t)
{
    out << t.id << '\t' << t.x << '\t' << t.y;
    return out;
}

class Area
{
public:
    Area(u_ll area, Tile *t1, Tile *t2)
    {
        this->area = area;
        this->t1 = new Tile(t1->gid(), t1->gx(), t1->gy());
        this->t2 = new Tile(t2->gid(), t2->gx(), t2->gy());
    }
    ~Area()
    {
        delete t1;
        delete t2;
    }

    Tile *get_t1() const { return t1; }
    Tile *get_t2() const { return t2; }
    u_ll get_area() const { return area; }

    friend std::ostream& operator<<(std::ostream& out, const Area &a);

private:
    u_ll area;
    Tile *t1;
    Tile *t2;
};

std::ostream& operator<<(std::ostream& out, const Area &a)
{
    out << a.area << " : [ " << *a.get_t1() << " ] , [ " << *a.get_t2() << " ]";
    return out;
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

    std::vector<Tile> tiles;
    std::string line;
    int tile_cnt = 0;
    while (std::getline(in, line))
    {
        std::string scoords[2];
        int coords[2];
        for (int i = 0; i < 2; ++i)
        {
            auto pos = line.find(delim);
            scoords[i] = line.substr(0, pos);
            line.erase(0, pos + delim.length());
            std::stringstream ss(scoords[i]);
            ss >> coords[i];
        }
        Tile t(tile_cnt, coords[0], coords[1]);
        tiles.push_back(t);
        ++tile_cnt;
    }

    for (Tile t : tiles)
    {
        std::cout << t << std::endl;
    }

    // Let's find the largest x-coordinate
    int max_x = tiles[0].gx();
    for (size_t i = 1; i < tiles.size(); ++i)
    {
        int cur_x = tiles[i].gx();
        if (max_x < cur_x)
            max_x = cur_x;
    }
    std::cout << "max_x = " << max_x << std::endl;

    // Let's find the largest y-coordinate
    int max_y = tiles[0].gy();
    for (size_t i = 1; i < tiles.size(); ++i)
    {
        int cur_y = tiles[i].gy();
        if (max_y < cur_y)
            max_y = cur_y;
    }
    std::cout << "max_y = " << max_y << std::endl;

    // Let's create and fill in our matrix
    int m_w = max_x + 3;
    int m_h = max_y + 2;
    std::cout << "Filling in the matrix...\n";
    std::vector< std::vector<char> > m(m_h, std::vector<char> (m_w, '.'));
    for (Tile t : tiles)
        m[t.gy()][t.gx()] = '#';

    /* Let's optimize our previous solution a bit. Instead of
     * traversing every matrix cell, we will create two vectors:
     * one for columns, where we will store all the y-coordinates
     * of the sharp signs found in each column, and another for
     * rows, where we will store the x-coordinates of the sharp
     * signs found in each row */

    // Stores x-coordinates of all '#' 
    std::cout << "Searching for sharps coordinates...\n";
    std::vector< std::vector<int> > rows_sharps(m_h, std::vector<int>());
    for (int ri = 0; ri < m_h; ++ri)
    {
        for (int ci = 0; ci < m_w; ++ci)
        {
            if (m[ri][ci] == '#')
            {
                rows_sharps[ri].push_back(ci);
                break;
            }
        }
        for (int ci = m_w - 1; ci >= 0; --ci)
        {
            if (m[ri][ci] == '#')
            {
                rows_sharps[ri].push_back(ci);
                break;
            }
        }
    }

    for (int i = 0; i < m_h; ++i)
    {
        for (int j = 0; j < rows_sharps[i].size(); ++j)
            std::cout << rows_sharps[i][j] << " ";
        std::cout << std::endl;
    }
 
    std::cout << "Doing main thing...\n";
    std::vector<Area *> areas;
    for (int ri = 0; ri < m_h; ++ri)
    {
        //std::cout << ri << std::endl;
        for (int ci = 0; ci < m_w; ++ci)
        {
            if (m[ri][ci] == '#')
            {
                // First go to the left
                for (int rq = ri; rq < m_h; ++rq)
                {
                    // Search for the farthest '#'
                    if (rows_sharps[rq].size() != 0)
                    {
                        // far_sharp_x contains the coordinate of the farthest '#'
                        int far_sharp_x = rows_sharps[rq].front();
                        if (far_sharp_x == ci && rq == ri)
                            continue;
                        else
                        {
                            if (far_sharp_x <= ci)
                            {
                                // Let's count the area
                                u_ll area = (static_cast<u_ll>(ci) - 
                                        static_cast<u_ll>(far_sharp_x) + 1) * 
                                    (static_cast<u_ll>(rq) - static_cast<u_ll>(ri) + 1);
                                if (area < 0)
                                {
                                    std::cout << "area = (" << ci << " - " << far_sharp_x
                                        << " + 1) * (" << rq << " - " << ri << " + 1) = "
                                        << (ci - far_sharp_x + 1) << " * " << (rq - ri + 1)
                                        << " = " << area << std::endl;
                                    std::cout << "sizeof(area) = " << sizeof (area) << std::endl;
                                    std::cout << "When going to left\n";
                                    std::cout << "far_sharp_x <= ci\n";
                                    std::cout << "Area is negative!\n";
                                    std::cout << "area = " << area << std::endl;
                                    std::cout << "ci = " << ci << std::endl;
                                    std::cout << "far_sharp_x = " << far_sharp_x << std::endl;
                                    std::cout << "rq = " << rq << std::endl;
                                    std::cout << "ri = " << ri << std::endl;
                                    for (Area *a : areas)
                                        delete a;
                                    exit(1);
                                }
                                else if (area > 10000000000)
                                {
                                    // How is it possible?
                                    std::cout << "area = (" << ci << " - " << far_sharp_x
                                        << " + 1) * (" << rq << " - " << ri << " + 1) = "
                                        << (ci - far_sharp_x + 1) << " * " << (rq - ri + 1)
                                        << " = " << area << std::endl;
                                    std::cout << "sizeof(area) = " << sizeof (area) << std::endl;
                                    std::cout << "When going to left\n";
                                    std::cout << "far_sharp_x <= ci\n";
                                    std::cout << "Area is too high!\n";
                                    std::cout << "area = " << area << std::endl;
                                    std::cout << "ci = " << ci << std::endl;
                                    std::cout << "far_sharp_x = " << far_sharp_x << std::endl;
                                    std::cout << "rq = " << rq << std::endl;
                                    std::cout << "ri = " << ri << std::endl;
                                    for (Area *a : areas)
                                        delete a;
                                    exit(1);
                                }
                                Tile tmp_tile1(ci, ri);
                                Tile tmp_tile2(far_sharp_x, rq);
                                Area *tmp_area = new Area(area, &tmp_tile1, &tmp_tile2);
                                areas.push_back(tmp_area);
                            }
                        }
                    }
                }
                
                // Then go to the right 
                for (int rq = ri; rq < m_h; ++rq)
                {
                    // Search for the farthest '#'
                    if (rows_sharps[rq].size() != 0)
                    {
                        // far_sharp_x contains the coordinate of the farthest '#'
                        int far_sharp_x = rows_sharps[rq].back();
                        if (far_sharp_x == ci && rq == ri)
                            continue;
                        else
                        {
                            if (far_sharp_x >= ci)
                            {
                                // Let's count the area
                                u_ll area = (static_cast<u_ll>(far_sharp_x) - 
                                        static_cast<u_ll>(ci) + 1) * 
                                    (static_cast<u_ll>(rq) - static_cast<u_ll>(ri) + 1);
                                if (area < 0)
                                {
                                    std::cout << "area = (" << far_sharp_x << " - " << ci
                                        << " + 1) * (" << rq << " - " << ri << " + 1) = "
                                        << (far_sharp_x - ci + 1) << " * " << (rq - ri + 1)
                                        << " = " << area << std::endl;
                                    std::cout << "sizeof(area) = " << sizeof (area) << std::endl;
                                    std::cout << "When going to right\n";
                                    std::cout << "far_sharp_x >= ci\n";
                                    std::cout << "Area is negative!\n";
                                    std::cout << "area = " << area << std::endl;
                                    std::cout << "ci = " << ci << std::endl;
                                    std::cout << "far_sharp_x = " << far_sharp_x << std::endl;
                                    std::cout << "rq = " << rq << std::endl;
                                    std::cout << "ri = " << ri << std::endl;
                                    for (Area *a : areas)
                                        delete a;
                                    exit(1);
                                }
                                else if (area > 10000000000)
                                {
                                    // How is it possible?
                                    std::cout << "area = (" << far_sharp_x << " - " << ci
                                        << " + 1) * (" << rq << " - " << ri << " + 1) = "
                                        << (far_sharp_x - ci + 1) << " * " << (rq - ri + 1)
                                        << " = " << area << std::endl;
                                    std::cout << "sizeof(area) = " << sizeof (area) << std::endl;
                                    std::cout << "When going to right\n";
                                    std::cout << "far_sharp_x >= ci\n";
                                    std::cout << "Area is too high!\n";
                                    std::cout << "area = " << area << std::endl;
                                    std::cout << "ci = " << ci << std::endl;
                                    std::cout << "far_sharp_x = " << far_sharp_x << std::endl;
                                    std::cout << "rq = " << rq << std::endl;
                                    std::cout << "ri = " << ri << std::endl;
                                    for (Area *a : areas)
                                        delete a;
                                    exit(1);
                                }
                                Tile tmp_tile1(ci, ri);
                                Tile tmp_tile2(far_sharp_x, rq);
                                Area *tmp_area = new Area(area, &tmp_tile1, &tmp_tile2);
                                areas.push_back(tmp_area);
                            }
                        }
                    }
                }
            }
        } // for (int ci = 0; ci < m_w; ++ci)
        
    } // for (int ri = 0; ri < m_h; ++ri)

    /*for (Area *a : areas)
        std::cout << *a << std::endl;*/

    // Now let's just find a maximum area in `areas`
    u_ll max_area = areas[0]->get_area();
    for (size_t ai = 1; ai < areas.size(); ++ai)
    {
        u_ll cur_area = areas[ai]->get_area();
        if (max_area < cur_area)
            max_area = cur_area;
    }
    std::cout << "The largest area is: " << max_area << std::endl;

    for (Area *a : areas)
        delete a;

    in.close();
    return 0;
}
