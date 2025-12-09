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

using u_ll = unsigned long long;

const std::string delim = ",";

struct Point
{
    int x;
    int y;
};

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

    // Let's find the point with the largest x coordinate
    Point max_x; // Point with the largest x coordinate
    int max_x_pos = 0;
    for (size_t i = 1; i < tiles.size(); ++i)
    {
        if (tiles[max_x_pos].gx() < tiles[i].gx())
            max_x_pos = i;
    }
    max_x.x = tiles[max_x_pos].gx();
    max_x.y = tiles[max_x_pos].gy();
    std::cout << "Point with max x: { "
        << max_x.x << ", " << max_x.y << " }" << std::endl;

    // Let's find the point with the smallest x coordinate
    Point min_x;
    int min_x_pos = 0;
    for (size_t i = 1; i < tiles.size(); ++i)
    {
        if (tiles[min_x_pos].gx() > tiles[i].gx())
            min_x_pos = i;
    }
    min_x.x = tiles[min_x_pos].gx();
    min_x.y = tiles[min_x_pos].gy();
    std::cout << "Point with min x: { "
        << min_x.x << ", " << min_x.y << " }" << std::endl;

    // Let's find the point with the largest y coordinate
    Point max_y;
    int max_y_pos = 0;
    for (size_t i = 1; i < tiles.size(); ++i)
    {
        if (tiles[max_y_pos].gy() < tiles[i].gy())
            max_y_pos = i;
    }
    max_y.x = tiles[max_y_pos].gx();
    max_y.y = tiles[max_y_pos].gy();
    std::cout << "Point with max y: { "
        << max_y.x << ", " << max_y.y << " }" << std::endl;

    // Let's find the point with the smallest y coordinate
    Point min_y;
    int min_y_pos = 0;
    for (size_t i = 1; i < tiles.size(); ++i)
    {
        if (tiles[min_y_pos].gy() > tiles[i].gy())
            min_y_pos = i;
    }
    min_y.x = tiles[min_y_pos].gx();
    min_y.y = tiles[min_y_pos].gy();
    std::cout << "Point with min y: { "
        << min_y.x << ", " << min_y.y << " }" << std::endl;

    u_ll max_area = 0;
    std::cout << "The largest area is: " << max_area << std::endl;

    Point p[4];
    p[0] = max_x;
    p[1] = min_x;
    p[2] = max_y;
    p[3] = min_y;

    std::vector<u_ll> areas;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (i != j)
            {
                u_ll area = (abs(p[i].x - p[j].x) + 1) * (abs(p[i].y - p[j].y) + 1);
                areas.push_back(area);
            }
        }
    }

    std::cout << "areas.size() = " << areas.size() << std::endl;

    for (u_ll a : areas)
        std::cout << a << std::endl;

    std::cout << "The largest area is: "
        << *max_element(areas.begin(), areas.end()) << std::endl;

    in.close();
    return 0;
}
