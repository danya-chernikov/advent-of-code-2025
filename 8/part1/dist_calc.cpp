#include <iostream>
#include <cmath>

double dist(int x1, int y1, int z1, int x2, int y2, int z2)
{
    // Summands
    double  sx, sy, sz;

    sx = pow(static_cast<double>(x2) - static_cast<double>(x1), 2);
    sy = pow(static_cast<double>(y2) - static_cast<double>(y1), 2);
    sz = pow(static_cast<double>(z2) - static_cast<double>(z1), 2);
    return sqrt(sx + sy + sz);
}

int main()
{
    int x1, y1, z1, x2, y2, z2;
    for (int i = 0; i < 1000; ++i)
    {
        std::cout << "Enter coordinates of the first point: ";
        std::cin >> x1 >> y1 >> z1;
        std::cout << "Enter coordinates of the second point: ";
        std::cin >> x2 >> y2 >> z2;
        std::cout << "Result is: " << dist(x1, y1, z1, x2, y2, z2) << std::endl;
    }
    return 0;
}
