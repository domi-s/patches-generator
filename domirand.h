#ifndef DOMIRAND_H_INCLUDED
#define DOMIRAND_H_INCLUDED

using namespace std;

class DomiRand {
public:
    DomiRand(){}
    void Init(int seed) {
        state = seed;
    }

    // Generate a random number between 0 and Max (exclusive)
    int generateRandom(int Max) {
        state = (1ll * a * state + c) % m;
        return state % Max;
    }

private:
    int state;
    const int a = 1664525;
    const int c = 1013904223;
    const int m = 2147483647;
};

#endif // DOMIRAND_H_INCLUDED
