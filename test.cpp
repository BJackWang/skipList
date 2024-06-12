#include <cstdlib>
#include <iostream>

int main(void) {
    for (int i = 0; i < 1e8; i++) {
        if (rand() / double(RAND_MAX) == 1) {
            std::cout << 0 << std::endl;
            std::cout << i;
            break;
        }
    }
    return 0;
}