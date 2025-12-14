/* In this implementation we assume a box
 * can be moved from a circuit containing
 * another boxes (with size more than one),
 * but only into a box that has more elements
 * than the current (from which we're gonna
 * extract our box), otherwise it does not
 * make any sense.. why to add boxes from
 * larger circuits to smaller?
 * But the question is: what to do if we
 * encounter with two equal circles (that
 * have the same size)? Here first I'm just
 * gonna try to blindly move the box from
 * one to another (to whatever circuit: from
 * left to right or the other way.. let's say
 * from left to right) */
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

const std::string   delim       = ",";
constexpr int       ITER_NUM    = 1000;
using u_ll = unsigned long long;

class Junction_Box
{
public:
    Junction_Box(int id, int x, int y, int z)
    {
        this->id = id;
        this->x = x;
        this->y = y;
        this->z = z;
    }
    ~Junction_Box() {}

    int gid() const { return id; }
    int gx() const { return x; }
    int gy() const { return y; }
    int gz() const { return z; }

private:
    int id;
    int x;
    int y;
    int z;
};

class Distance
{
public:
    Distance(int id1, int id2, double dist)
    {
        this->id1 = id1;
        this->id2 = id2;
        this->dist = dist;
    }
    ~Distance() {}

    int get_id1() const { return id1; }
    int get_id2() const { return id2; }
    double get_dist() const { return dist; }

private:
    int     id1;
    int     id2;
    double  dist;
};

double dist(int x1, int y1, int z1, int x2, int y2, int z2)
{
    // Summands
    double  sx, sy, sz;

    sx = pow(static_cast<double>(x2) - static_cast<double>(x1), 2);
    sy = pow(static_cast<double>(y2) - static_cast<double>(y1), 2);
    sz = pow(static_cast<double>(z2) - static_cast<double>(z1), 2);
    return sqrt(sx + sy + sz);
}

/* Checks if two junction boxes are connected or not (if they are already
 * live in the same circuit */
bool boxes_connected(std::vector< std::list<Junction_Box> > &circuits,
                     int box_id1,
                     int box_id2)
{
    bool f_id1 = false; // Flag marking if first box was found
    bool f_id2 = false;

    for (size_t ci = 0; ci < circuits.size(); ++ci) // circuit index
    {
        for (std::list<Junction_Box>::iterator ni = circuits[ci].begin();
             ni != circuits[ci].end(); ++ni) // node iterator
        {
            if (box_id1 == (*ni).gid() && !f_id2) // box_id1 was found first
                f_id1 = true;
            else if (box_id1 == (*ni).gid() && f_id2) // we've already seen box_id2
                return true;

            if (box_id2 == (*ni).gid() && !f_id1)  // box_id2 was found first
                f_id2 = true;
            else if (box_id2 == (*ni).gid() && f_id1) // we've already seen box_id1
                return true;
        }
        f_id1 = false;
        f_id2 = false;
    }
    return false;
}

/* Returns the index of the circuit in the `circuits` vector that contains the
 * box with ID `box_id`. If the box is not found in any circuit, returns -1 */
int get_circuit_index(std::vector< std::list<Junction_Box> > &circuits,
                      int box_id)
{
    for (size_t ci = 0; ci < circuits.size(); ++ci) // Circuit index
    {
        for (std::list<Junction_Box>::iterator ni = circuits[ci].begin();
             ni != circuits[ci].end(); ++ni) // Node iterator
        {
            if ((*ni).gid() == box_id)
                return ci;
        }
    }
    return -1;
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

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<Junction_Box> boxes;
    int box_cnt = 0;
    std::string line;
    while (std::getline(in, line))
    {
        std::string scoords[3];
        int coords[3];
        for (int i = 0; i < 3; ++i)
        {
            auto pos = line.find(delim);
            scoords[i] = line.substr(0, pos);
            line.erase(0, pos + delim.length());
            std::stringstream ss(scoords[i]);
            ss >> coords[i];
        }
        Junction_Box box(box_cnt, coords[0], coords[1], coords[2]);
        boxes.push_back(box);
        ++box_cnt;
    }

    // Circuts (connections between boxes)
    std::vector< std::list<Junction_Box> > circuits;

    std::vector<Junction_Box> &b = boxes; // Just an alias

    // At first, we consider each box as a circuit consited of one element
    // so let's populate our junction boxes into `circuits`
    for (size_t bi = 0; bi < b.size(); ++bi)
    {
        std::list<Junction_Box> list;
        list.push_front(b[bi]);
        circuits.push_back(list);
    }

    int conn_cnt = 0;
    // The main loop
    while (circuits.size() > 1) // Until all boxes will be connected
    {
        if (conn_cnt == ITER_NUM - 1)
        {
            std::cout << "Ten connections have been made\n";
            break;
        }

        std::cout << "Boxes:\n";
        for (size_t bi = 0; bi < boxes.size(); ++bi)
        {
            std::cout << boxes[bi].gid() << '\t'
                      << boxes[bi].gx()  << '\t'
                      << boxes[bi].gy()  << '\t'
                      << boxes[bi].gz()  << '\n';
        }
        std::cout << std::endl;

        std::vector<Distance> min_dists;

        /* Loop where we're looking for the two boxes closest
         * to  each other and not yet connected into a circuit
         * together. We're populating a vector of the minimum
         * distances between each two boxes. For each box we
         * find its closest pair box and add the distance between
         * them into `min_dists` and then search for the lowest
         * value in `min_dists` */
        for (size_t i = 0; i < b.size(); ++i)
        {
            // Let's count distances between b[i] and all the others
            // Let's count all distances between a box from a circuit
            // and all other boxes from all other circuits except but
            // not including this box's neighbours
            std::vector<Distance> dists;
            // Let's get the circuit index of b[i]
            for (size_t j = 0; j < b.size(); ++j)
            {
                if (i != j && !boxes_connected(circuits, b[i].gid(), b[j].gid()))
                {
                    double d = dist(b[i].gx(), b[i].gy(), b[i].gz(),
                                    b[j].gx(), b[j].gy(), b[j].gz());
                    Distance d_obj(b[i].gid(), b[j].gid(), d);
                    dists.push_back(d_obj);
                }
            }

            //std::cout << "dists.size() = " << dists.size() << std::endl;

            // Let's find the minimum distance in `dists`
            int min_ind = 0;
            for (size_t di = 1; di < dists.size(); ++di)
            {
                if (dists[di].get_dist() < dists[min_ind].get_dist())
                    min_ind = di;
            }
            double  min_dist = dists[min_ind].get_dist();
            int     box_id1 = dists[min_ind].get_id1();
            int     box_id2 = dists[min_ind].get_id2();
            std::cout << "min_dist = "  << min_dist
                      << " between "    << box_id1 << ": ["
                      << b[box_id1].gx() << ", "
                      << b[box_id1].gy() << ", "
                      << b[box_id1].gz() << "] "
                      << " and "        << box_id2 << ": ["
                      << b[box_id2].gx() << ", "
                      << b[box_id2].gy() << ", "
                      << b[box_id2].gz() << "]\n";

            // First, let's check if box_id1 or box_id2 are already part of a circuit
            // if they're not (box_id1 is part of one circuit and box_id2 is part of another.
            // Of course, these circuits may contain more than one element)
            std::cout << std::endl;
            if (!boxes_connected(circuits, box_id1, box_id2))
            {
                std::cout << box_id1 << " and " << box_id2 << " are not connected\n";
                // If box_id1 and box_id2 belong to a different circuits
                // Let's add this minimum into `min_dists`
                min_dists.push_back(dists[min_ind]);
            }
            std::cout << std::endl;

        } // for (int i = 0; i < b.size(); ++i)

        std::cout << std::endl;
        std::cout << "min_dists[]:\n";
        for (size_t i = 0; i < min_dists.size(); ++i)
        {
            std::cout << min_dists[i].get_id1() << '\t'
                      << min_dists[i].get_id2() << '\t'
                      << min_dists[i].get_dist() << '\n';
        }
        std::cout << std::endl;

        // Now let's find the lowest distance in `min_dists`
        int min_ind = 0;
        for (size_t di = 1; di < min_dists.size(); ++di)
        {
            if (min_dists[di].get_dist() < min_dists[min_ind].get_dist())
                min_ind = di;
        }
        double  min_dist = min_dists[min_ind].get_dist();
        int     box_id1 = min_dists[min_ind].get_id1();
        int     box_id2 = min_dists[min_ind].get_id2();
        std::cout << "The closest boxes to each other are: "
                  << box_id1 << ": ["
                  << b[box_id1].gx() << ", "
                  << b[box_id1].gy() << ", "
                  << b[box_id1].gz() << "] "
                  << " and "
                  << box_id2 << ": ["
                  << b[box_id2].gx() << ", "
                  << b[box_id2].gy() << ", "
                  << b[box_id2].gz() << "] "
                  << " with distance of " << min_dist << "\n\n";

        /* If both boxes are located in circuits of size 1, we simply
          * form a new circuit by adding one to the other and eliminating
          * the circuit from which we extracted a box.
          *
          * If one box has size 1 and the other has a size greater than 1,
          * we add the smaller circuit to the larger one, eliminating the
          * smaller circuit.
          *
          * If both boxes belong to circuits of size greater than 1, we unite
          * them into a single circuit by adding the smaller circuit to the
          * larger one and then eliminating the smaller circuit.
          *
          * If both boxes are of equal size, it does not matter which circuit
          * is added to which. */

        // Let's find the first box, what is the index of
        // the circuit that contains it?
        // 'box_1_cind' means 'circuit index of the first box'
        int box_1_cind = get_circuit_index(circuits, box_id1);
        if (box_1_cind == -1)
        {
            std::cout << "Insanity! The box does not exist anywhere!\n";
            exit(1);
        }
        std::cout << "Circuit index of the first box is: "
                  << box_1_cind << std::endl;
        std::cout << "Circuit size of the first box is: "
                  << circuits[box_1_cind].size() << std::endl;

        // And the second box
        int box_2_cind = get_circuit_index(circuits, box_id2);
        if (box_2_cind == -1)
        {
            std::cout << "Insanity! The box does not exist anywhere!\n";
            exit(1);
        }
        std::cout << "Circuit index of the second box is: "
                  << box_2_cind << std::endl;
        std::cout << "Circuit size of the second box is: "
                  << circuits[box_2_cind].size() << std::endl;

        if (box_1_cind == box_2_cind)
        {
            std::cout << "Alert! Boxes are in the same circuit! Do nothing\n";
        }
        else // Otherwise let's make a connection
        {
            ++conn_cnt;
            // If first box circuit is equal or larger than the second
            if (circuits[box_1_cind].size() >= circuits[box_2_cind].size())
            {
                /* Let's merge these two circuits. We need to move all elements
                 * from the circuit that contains the box with index `box_2_cind`
                 * into the circuit that contains the box with index `box_1_cind`,
                 * and then remove the circuit from which the elements were moved */
                for (std::list<Junction_Box>::iterator ni = circuits[box_2_cind].begin();
                     ni != circuits[box_2_cind].end(); ++ni) // Node iterator
                    circuits[box_1_cind].push_back(*ni);
                circuits.erase(circuits.begin() + box_2_cind);
            }
            else // circuits[box_1_cind].size() < circuits[box_2_cind].size()
            {
                // Doing the same but for another circuit
                for (std::list<Junction_Box>::iterator ni = circuits[box_1_cind].begin();
                     ni != circuits[box_1_cind].end(); ++ni) // Node iterator
                    circuits[box_2_cind].push_back(*ni);
                circuits.erase(circuits.begin() + box_1_cind);
            }
        }

        // Now let's print our newly formed circuits
        std::cout << std::endl;
        for (size_t ci = 0; ci < circuits.size(); ++ci) // circuit index
        {
            std::cout << ci << ": ";
            for (std::list<Junction_Box>::iterator ni = circuits[ci].begin();
                 ni != circuits[ci].end(); ++ni) // node iterator
            {
                std::cout << "[ " << (*ni).gid()
                << ": " << (*ni).gx()
                << ", " << (*ni).gy()
                << ", " << (*ni).gz()
                << " ] <-> ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        std::cout << "############################################################\n";

        // End of main loop
    } // for (int it_ind = 0; it_ind < ITER_NUM; ++it_ind)

    // Finally let's multiple the sizes of the three largest circuits
    // Let's create a vector containing sizes of each circuit
    std::vector<size_t> sizes(circuits.size());
    for (size_t ci = 0; ci < circuits.size(); ++ci)
        sizes[ci] = circuits[ci].size();

    // Let's sort this array
    std::sort(sizes.begin(), sizes.end());

    std::cout << "Sorted circuit sizes:\n";
    for (size_t si = 0; si < sizes.size(); ++si)
        std::cout << sizes[si] << std::endl;
    std::cout << std::endl;

    u_ll mul_res = sizes[sizes.size() - 1]
                  * sizes[sizes.size() - 2]
                  * sizes[sizes.size() - 3];

    std::cout << "Multiplied together the sizes of the"
              << " three largest circuits produced after "
              << ITER_NUM << " iterations of our algorithm"
                             " we got " << mul_res << std::endl;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Time taken by function: "
         << duration.count() << " microseconds" << std::endl;

    in.close();
    return 0;
}
