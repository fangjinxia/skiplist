#ifndef RANDOM_H
#define RANDOM_H

#include <cstdint>

class random {
public:
    explict random(uint32_t s) : seed(s & 0x7fffffffu) {
        //avoid bad seed
        if(seed == 0 || seed == 2147483647L) {
            seed = 1;
        }
    }



private:
    uint32_t seed;
};


#endif // END_OF_RANDOM_H
