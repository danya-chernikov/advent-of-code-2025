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

enum Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

class Tile
{
public:
    Tile(int its_id, int its_x, int its_y) :
        id(its_id), x(its_x), y(its_y), f_visited(false) {}

    Tile(int its_x, int its_y) : id(0), x(its_x), y(its_y), f_visited(false) {}

    ~Tile() {}

    int gid() const { return id; }
    int gx() const { return x; }
    int gy() const { return y; }
    bool visited() const { return f_visited; }

    void set_visited(bool state) { f_visited = state; }

    friend std::ostream& operator<<(std::ostream& out, const Tile &t);

private:
    int id;
    int x;
    int y;
    bool f_visited;
};

/* Sets the tile with coordinates x and y as visited.
 * If a tile with such coordinates does not exist
 * returns -1 */
int set_tile_visited(std::vector<Tile> &tiles, int x, int y)
{
    for (size_t ti = 0; ti < tiles.size(); ++ti)
    {
        if (tiles[ti].gx() == x && tiles[ti].gy() == y)
        {
            tiles[ti].set_visited(true);
            return 1;
        }
    }
    return -1;
}

/* Checks if the tile with coordinates x and y
 * is visited or not. If a tile with such coordinates
 * does not exist sets `err` to 1. If all good `err` is 0 */
bool tile_visited(std::vector<Tile> &tiles, int x, int y, int *err)
{
    *err = 0;
    for (size_t ti = 0; ti < tiles.size(); ++ti)
    {
        if (tiles[ti].gx() == x && tiles[ti].gy() == y)
        {
            if (tiles[ti].visited())
                return true;
            else
                return false;
        }
    }
    *err = 1;
    return false;
}

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

std::pair<int, int> find_first_sharp(std::vector< std::vector<char> > &m,
                                     int m_w,
                                     int m_h);

bool above_sharp(std::vector< std::vector<char> > &m,
                 std::vector<Tile> &tiles,
                 int cur_x,
                 int cur_y);

bool below_sharp(std::vector< std::vector<char> > &m,
                 std::vector<Tile> &tiles,
                 int cur_x,
                 int cur_y,
                 int m_h);

bool right_sharp(std::vector< std::vector<char> > &m,
                 std::vector<Tile> &tiles,
                 int cur_x,
                 int cur_y,
                 int m_w);

bool left_sharp(std::vector< std::vector<char> > &m,
                std::vector<Tile> &tiles,
                int cur_x,
                int cur_y);

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

    // Let's find the largest x-coordinate
    int max_x = tiles[0].gx();
    for (size_t i = 1; i < tiles.size(); ++i)
    {
        int cur_x = tiles[i].gx();
        if (max_x < cur_x)
            max_x = cur_x;
    }

    // Let's find the largest y-coordinate
    int max_y = tiles[0].gy();
    for (size_t i = 1; i < tiles.size(); ++i)
    {
        int cur_y = tiles[i].gy();
        if (max_y < cur_y)
            max_y = cur_y;
    }

    // Let's create and fill in our matrix
    int m_w = max_x + 3;
    int m_h = max_y + 2;
    std::vector< std::vector<char> > m(m_h, std::vector<char> (m_w, '.'));
    for (Tile t : tiles)
        m[t.gy()][t.gx()] = '#';

    // Let's add green tiles along the perimeter 
    int fsx = -1; // first sharp x
    int fsy = -1;
    // Let's find the first '#' in our matrix
    std::pair<int, int> first_sharp = find_first_sharp(m, m_w, m_h);
    fsx = first_sharp.first;
    fsy = first_sharp.second;
    std::cout << "fsx = " << fsx << std::endl;
    std::cout << "fsy = " << fsy << std::endl;

    /* Now let's choose the direction. We start on the
     * first tile found on the map, which means there
     * are no tiles before it or above it. This means
     * we can move only to the right or down. We assume
     * there will always be one sharp on the right and
     * one sharp below, because according to the task
     * description, ALL red tiles are connected and form
     * a loop */

    Direction dir = RIGHT; // Let's just go to the right first
    // Go until meet another '#'
    int cur_x = fsx + 1;
    int cur_y = fsy;
    std::cout << "cur_x = " << cur_x << std::endl;
    std::cout << "cur_y = " << cur_y << std::endl;

    // We don't have to set our initial tile as visited!
    //set_tile_visited(tiles, cur_x, cur_y);
    //
    // At the same time we gonna mark sharps we already visited
    while (1) // Until we return to the initial point
    {
exit_loop:
        if (cur_x == fsx && cur_y == fsy)
            break;
        while (m[cur_y][cur_x] == '#')
        { 
            if (cur_x == fsx && cur_y == fsy)
                goto exit_loop;
            set_tile_visited(tiles, cur_x, cur_y); // Set this tile as visited
            // Change our direction depending on where located the next '#'
            // We cannot go in the same direction and our previous direction
            // was RIGHT, also we cannot go back.. it means our new direction
            // will be eight UP or DOWN
            if (dir == RIGHT || dir == LEFT) // If our previous direction was RIGHT
            {
                // Check whether above or below us exists another '#'
                bool res;
                if ((res = above_sharp(m, tiles, cur_x, cur_y)))
                {
                    dir = UP;
                    --cur_y; // One step up
                }
                else if ((res = below_sharp(m, tiles, cur_x, cur_y, m_h)))
                {
                    dir = DOWN;
                    ++cur_y; // One step down
                }
                if (!res)
                {
                    std::cout << "Alert! We cannot find where to go\n";
                    exit(1);
                }
            }
            else if (dir == UP || dir == DOWN) // If our prevous direction was DOWN
            {
                // Check whether on the left or on the right exists another '#'
                bool res;
                if ((res = right_sharp(m, tiles, cur_x, cur_y, m_w)))
                {
                    dir = RIGHT;
                    ++cur_x; // One step right
                }
                else if ((res = left_sharp(m, tiles, cur_x, cur_y)))
                {
                    dir = LEFT;
                    --cur_x; // One step left
                }
                if (!res)
                {
                    std::cout << "Alert! We cannot find where to go\n";
                    exit(1);
                }
            }
        } // while (m[cur_y][cur_x] == '#')

        m[cur_y][cur_x] = 'X';

        switch(dir)
        {
            case UP: --cur_y; break;
            case RIGHT: ++cur_x; break;
            case DOWN: ++cur_y; break;
            case LEFT: --cur_x; break;
        }

        /*std::cout << std::endl;
        for (int i = 0; i < m_h; ++i)
        {
            for (int j = 0; j < m_w; ++j)
                std::cout << m[i][j];
            std::cout << std::endl;
        }
        std::cout << std::endl;*/

        //sleep(1);
        
    } // while (cur_x != fsx && cur_y != fsy)
    
    // Let's set all visited flags to false
    for (size_t ti = 0; ti < tiles.size(); ++ti)
        tiles[ti].set_visited(false);

    std::cout << "Traversing...\n";
    // Now let's fill in the area we drew with green tiles
    // We have to just traverse perimeter
    dir = RIGHT;
    cur_x = fsx + 1;
    cur_y = fsy;
    while (1)
    {
exit_loop2:
        int inc_y = 0, dec_y = 0, inc_x = 0, dec_x = 0;
        if (cur_x == fsx && cur_y == fsy)
            break;
        while (m[cur_y][cur_x] == '#')
        { 
            if (cur_x == fsx && cur_y == fsy)
                goto exit_loop2;
            set_tile_visited(tiles, cur_x, cur_y);
            if (dir == RIGHT || dir == LEFT)
            {
                bool res;
                if ((res = above_sharp(m, tiles, cur_x, cur_y)))
                {
                    m[cur_y][cur_x + 1] = 'X';
                    dir = UP;
                    --cur_y;
                    dec_y = 1;
                }
                else if ((res = below_sharp(m, tiles, cur_x, cur_y, m_h)))
                {
                    m[cur_y][cur_x - 1] = 'X';
                    dir = DOWN;
                    ++cur_y;
                    inc_y = 1;
                }
                if (!res)
                {
                    std::cout << "Alert! We cannot find where to go\n";
                    exit(1);
                }
            }
            else if (dir == UP || dir == DOWN)
            {
                bool res;
                if ((res = right_sharp(m, tiles, cur_x, cur_y, m_w)))
                {
                    m[cur_y + 1][cur_x] = 'X';
                    dir = RIGHT;
                    ++cur_x;
                    inc_x = 1;
                }
                else if ((res = left_sharp(m, tiles, cur_x, cur_y)))
                {
                    m[cur_y - 1][cur_x] = 'X';
                    dir = LEFT;
                    --cur_x;
                    dec_x = 1;
                }
                if (!res)
                {
                    std::cout << "Alert! We cannot find where to go\n";
                    exit(1);
                }
            }
        } // while (m[cur_y][cur_x] == '#')
        
        int i = 1;
        if (dir == UP)
        {
            // If we go up, fill from left to right
            while (cur_x + i < m_w && m[cur_y][cur_x + i] == '.')
            {
                m[cur_y][cur_x + i] = 'X';
                ++i;
            }
            if (!dec_y)
                --cur_y;
        }
        else if (dir == RIGHT)
        {
            // If we go right, fill from top to bottom
            while (cur_y + i < m_h && m[cur_y + i][cur_x] == '.')
            {
                m[cur_y + i][cur_x] = 'X';
                ++i;
            }
            if (!inc_x)
                ++cur_x;
        }
        else if (dir == DOWN)
        {
            // If we go down, fill from right to left
            while (cur_x - i >= 0 && m[cur_y][cur_x - i] == '.')
            {
                m[cur_y][cur_x - i] = 'X';
                ++i;
            }
            if (!inc_y)
                ++cur_y;
        }
        else if (dir == LEFT)
        {
            // If we go left, fill from bottom to top
            while (cur_y - i >= 0 && m[cur_y - i][cur_x] == '.')
            {
                m[cur_y - i][cur_x] = 'X';
                ++i;
            }
            if (!dec_x)
                --cur_x;
        }

        /*char priv = m[cur_y][cur_x];
        m[cur_y][cur_x] = '*';
        std::cout << std::endl;
        for (int i = 0; i < m_h; ++i)
        {
            for (int j = 0; j < m_w; ++j)
                std::cout << m[i][j];
            std::cout << std::endl;
        }
        std::cout << std::endl;
        m[cur_y][cur_x] = priv;*/

        //sleep(1);
        
    } // while (cur_x != fsx && cur_y != fsy)

    /*std::cout << std::endl;
    for (int i = 0; i < m_h; ++i)
    {
        for (int j = 0; j < m_w; ++j)
            std::cout << m[i][j];
        std::cout << std::endl;
    }
    std::cout << std::endl;*/
    std::cout << "FINISHED\n";

    sleep(10);

    /* Let's optimize our previous solution a bit. Instead of
     * traversing every matrix cell, we will create two vectors:
     * one for columns, where we will store all the y-coordinates
     * of the sharp signs found in each column, and another for
     * rows, where we will store the x-coordinates of the sharp
     * signs found in each row */

    // Stores x-coordinates of all '#' 
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

    std::vector<Area *> areas;
    for (int ri = 0; ri < m_h; ++ri)
    {
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
                                bool f_good_area = true;
                                bool f_loop = true;
                                for (int rect_ri = ri; rect_ri <= rq; ++rect_ri)
                                {
                                    for (int rect_ci = far_sharp_x; rect_ci <= ci; ++rect_ci)
                                    {
                                        if (m[rect_ri][rect_ci] == '.')
                                        {
                                            f_good_area = false;
                                            f_loop = false;
                                            break;
                                        }
                                    }
                                    if (!f_loop)
                                        break;
                                }

                                if (f_good_area)
                                {
                                    // Let's count the area
                                    u_ll area = (static_cast<u_ll>(ci) - 
                                            static_cast<u_ll>(far_sharp_x) + 1) * 
                                        (static_cast<u_ll>(rq) - static_cast<u_ll>(ri) + 1);

                                    Tile tmp_tile1(ci, ri);
                                    Tile tmp_tile2(far_sharp_x, rq);
                                    Area *tmp_area = new Area(area, &tmp_tile1, &tmp_tile2);
                                    areas.push_back(tmp_area);
                                }
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
                                bool f_good_area = true;
                                bool f_loop = true;
                                for (int rect_ri = ri; rect_ri <= rq; ++rect_ri)
                                {
                                    for (int rect_ci = ci; rect_ci <= far_sharp_x; ++rect_ci)
                                    {
                                        if (m[rect_ri][rect_ci] == '.')
                                        {
                                            f_good_area = false;
                                            f_loop = false;
                                            break;
                                        }
                                    }
                                    if (!f_loop)
                                        break;
                                }
                                if (f_good_area)
                                {
                                    // Let's count the area
                                    u_ll area = (static_cast<u_ll>(far_sharp_x) - 
                                            static_cast<u_ll>(ci) + 1) * 
                                        (static_cast<u_ll>(rq) - static_cast<u_ll>(ri) + 1);
                                    Tile tmp_tile1(ci, ri);
                                    Tile tmp_tile2(far_sharp_x, rq);
                                    Area *tmp_area = new Area(area, &tmp_tile1, &tmp_tile2);
                                    areas.push_back(tmp_area);
                                }
                            }
                        }
                    }
                }
            }
        } // for (int ci = 0; ci < m_w; ++ci)
        
    } // for (int ri = 0; ri < m_h; ++ri)

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

std::pair<int, int> find_first_sharp(std::vector< std::vector <char> > &m,
                                     int m_w,
                                     int m_h)
{
    std::pair<int, int> coords;
    bool f_exit = false;
    for (int ri = 0; ri < m_h; ++ri)
    {
        for (int ci = 0; ci < m_w; ++ci)
        {
            if (m[ri][ci] == '#')
            {
                coords.first = ci; // first sharp x
                coords.second = ri; // first sharp y
                f_exit = true;
                break;
            }
        }
        if (f_exit)
            break;
    }
    return coords;
}

bool above_sharp(std::vector< std::vector<char> > &m,
                 std::vector<Tile> &tiles,
                 int cur_x,
                 int cur_y)
{
    int err;
    --cur_y;
    while (cur_y >= 0)
    {
        if (m[cur_y][cur_x] == '#' && !tile_visited(tiles, cur_x, cur_y, &err))
            return true;
        --cur_y;
    }
    return false;
}
bool below_sharp(std::vector< std::vector<char> > &m,
                 std::vector<Tile> &tiles,
                 int cur_x,
                 int cur_y,
                 int m_h)
{
    int err;
    ++cur_y;
    while (cur_y < m_h)
    {
        if (m[cur_y][cur_x] == '#' && !tile_visited(tiles, cur_x, cur_y, &err))
            return true;
        ++cur_y;
    }
    return false;
}
bool right_sharp(std::vector< std::vector<char> > &m,
                 std::vector<Tile> &tiles,
                 int cur_x,
                 int cur_y,
                 int m_w)
{
    int err;
    ++cur_x;
    while (cur_x < m_w)
    {
        if (m[cur_y][cur_x] == '#' && !tile_visited(tiles, cur_x, cur_y, &err))
            return true;
        ++cur_x;
    }
    return false;
}
bool left_sharp(std::vector< std::vector<char> > &m,
                std::vector<Tile> &tiles,
                int cur_x,
                int cur_y)
{
    int err;
    --cur_x;
    while (cur_x >= 0)
    {
        if (m[cur_y][cur_x] == '#' && !tile_visited(tiles, cur_x, cur_y, &err))
            return true;
        --cur_x;
    }
    return false;
}
