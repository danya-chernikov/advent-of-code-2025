#include <iostream>
#include <cmath>

std::string dec_to_bin(int num)
{
    std::string res = "";

    for (int i = pow(2, sizeof(int)) - 1; i >= 0; --i)
        (num & (1 << i)) ? res += '1' : res += '0';
    return res;
}

int main()
{
    int num = 1022;
    std::cout << dec_to_bin(num) << std::endl;
    for (int i = 0; i < 100; ++i)
    {
        int bit_ind;
        std::cout << "Enter bit index: ";
        std::cin >> bit_ind;
        
        if (num & (1 << bit_ind)) // if bit is set
            num &= ~((unsigned int)1 << bit_ind); // unset it (make it 0)
        else // if bit is unset
            num |= ((unsigned int)1 << bit_ind); // set it (make it 1)
            
        std::cout << dec_to_bin(num) << std::endl;
    }
    return 0;
}
