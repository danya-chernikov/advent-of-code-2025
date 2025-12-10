/* In this implementation we assume
 * a box may be moved from one circuit
 * to another only if this box is single
 * (lives solo) in its own circuit. We
 * consider circuits that have more than
 * one element already full-fledged, i.e.
 * we can move only boxes from circuits of
 * size one to another circuit of size one
 * or greater size, but we cannot for example
 * move a box from a circuit that contains
 * two elements anywhere cause we consider
 * this circuit as already formed */
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
constexpr int       ITER_NUM    = 10;

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

    for (int ci = 0; ci < circuits.size(); ++ci) // circuit index
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
    for (int ci = 0; ci < circuits.size(); ++ci) // circuit index
    {
        for (std::list<Junction_Box>::iterator ni = circuits[ci].begin();
            ni != circuits[ci].end(); ++ni) // node iterator
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
    for (int bi = 0; bi < b.size(); ++bi)
    {
        std::list<Junction_Box> list;
        list.push_front(b[bi]);
        circuits.push_back(list);
    }

    // The main loop
    for (int it_ind = 0; it_ind < ITER_NUM; ++it_ind)
    {

        std::cout << "Boxes:\n";
        for (int bi = 0; bi < boxes.size(); ++bi)
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
        for (int i = 0; i < b.size(); ++i)
        {
            // Let's count distances between b[i] and all the others
            std::vector<Distance> dists;
            for (int j = 0; j < b.size(); ++j)
            {
                if (i != j)
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
            for (int di = 1; di < dists.size(); ++di)
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
        for (int i = 0; i < min_dists.size(); ++i)
        {
            std::cout << min_dists[i].get_id1() << '\t'
                      << min_dists[i].get_id2() << '\t'
                      << min_dists[i].get_dist() << '\n';
        }
        std::cout << std::endl;

        // Now let's find the lowest distance in `min_dists`
        int min_ind = 0;
        for (int di = 1; di < min_dists.size(); ++di)
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

        /* And only now are we going to connect these 
         * two boxes (or just one of them) to another
         * circuit.
         *
         * THE QUESTION IS: How do we determine which
         * circuit each found box should be added to?
         *
         * THE ANSWER IS: We know that our boxes belong to
         * different circuits. This means the box located
         * in the smaller circuit should be added to the
         * larger one, because we wanna save resources.
         * BUT WHAT IF CIRCUITS HAVE THE SAME SIZE?
         * */
        
        // Let's find the first box, what is the index of
        // the circuit that contains it?
        // 'box_1_cind' means 'circuit index of the first box'
        int box_1_cind = get_circuit_index(circuits, box_id1);
        if (box_1_cind == -1)
        {
            std::cout << "Insanity! The box does not exist anywhere!\n";
            exit(1);
        }
        std::cout << "Circuit index of the first box is: " << box_1_cind << std::endl;
        // And the second box
        int box_2_cind = get_circuit_index(circuits, box_id2);
        if (box_2_cind == -1)
        {
            std::cout << "Insanity! The box does not exist anywhere!\n";
            exit(1);
        }
        std::cout << "Circuit index of the second box is: " << box_2_cind << std::endl;

        // Let's assume if both boxes already have more than one neighbour we do nothing
        // cause these are already full-fledged circuits
        // We're gonna add the new box to a circuit only if this new box is single in
        // their own circuit
        
        if (circuits[box_1_cind].size() > 1 && circuits[box_2_cind].size() > 1)
        {
            std::cout << "We do nothing cause both these circuits were already completed\n";
        }
        else // It means one of the circuits is lonely
        {
            // If both are lonely
            if (circuits[box_1_cind].size() == 1 && circuits[box_2_cind].size() >= 1)
            {
                // One of the boxes is gonna visit another and left its own house (circuit)
                circuits[box_2_cind].push_front(circuits[box_1_cind].front());
                circuits[box_1_cind].pop_front();
                circuits.erase(circuits.begin() + box_1_cind);
            }
            else if (circuits[box_1_cind].size() > 1 && circuits[box_2_cind].size() == 1)
            {
                circuits[box_1_cind].push_front(circuits[box_2_cind].front());
                circuits[box_2_cind].pop_front();
                circuits.erase(circuits.begin() + box_2_cind);
            }
            else
            {
                std::cout << "Impossible thing had happen\n";
                exit(1);
            }
        }

        // Now let's print our newly formed circuits 
        std::cout << std::endl;
        for (int ci = 0; ci < circuits.size(); ++ci) // circuit index
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
    std::vector<int> sizes(circuits.size());
    for (int ci = 0; ci < circuits.size(); ++ci)
        sizes[ci] = circuits[ci].size();

    // Let's sort this array
    std::sort(sizes.begin(), sizes.end());

    std::cout << "Sorted circuit sizes:\n";
    for (int si = 0; si < sizes.size(); ++si)
        std::cout << sizes[si] << std::endl;
    std::cout << std::endl;

    int mul_res = sizes[sizes.size() - 1]
                * sizes[sizes.size() - 2]
                * sizes[sizes.size() - 3];

    std::cout << "Multiplied together the sizes of the"
              << " three largest circuits produced after "
              << ITER_NUM << " iterations of our algorithm"
              " we got " << mul_res << std::endl;

    in.close();
    return 0;
}
