#pragma once

#include "../globals.h"

/*
    A class that contains mathematics-related static helper methods. 

*/

class Math
{
public:
    Math(unsigned long s = 5489UL) {};
    ~Math() {};

    //some ugly template functions for stuff
    template <class T> static T lerp(const float t, const T &a, const T &b)
    {
        return a + (b - a) * t;
    };
    template <class T> static T clamp(const T &t, const T &a, const T &b)
    {
        if (t < a)
            return a;
        if (t > b)
            return b;
        else return t;
    };
    template <class T> static float calcPos(const T &t, const T &a, const T& b)
    {
        if (t < a)
            return 0.0f;
        if (t > b)
            return 1.0f;
        return ((float)(t - a)) / (b - a);
    };


    //phi = [0, pii], theta = [0, 2*pii], radius = [0, inf]
    static Vector3 sphereToCartesian(float radius, float phi, float theta);
    //palauttaa Vectorn, jossa on halutut karteesiset koordinaatit pallokoordinaatistossa
    //v = [radius, phi, theta]. Ei huomioi, miss segmentiss ollaan joten muista tarkistaa
    //kulmat!
    static Vector3 cartesianToSphere(float x, float y, float z);

    //numeerinen integrointi Simpsonin menetelmll
    static float integrateSimpson(float startpoint, float endpoint, int steps, float(*function)(float));
    static float calcPosFloat(float value, float limit1, float limit2);
    static float calcPosInt(int value, int limit1, int limit2);

    // Smooth step algo
    static float calcSmoothStep(float value, float limit1, float limit2);
    // palauttaa [0 - max]
    static float randFloat(float max = 1.0f);
    // palauttaa [min, max]
    static float randBetween(float min, float max);
    // palauttaa [min, max]
    static int randIntBetween(int min, int max);
    static float randBetween(Range& range);

    // Vector( [-0.5f, 0.5f]*kerroin
    static Vector3 randVector(float dx = 1.0f, float dy = 1.0f, float dz = 1.0f);

    static Vector3 randVectSphere(float radius = 1.0f);

    // Mersenne twister
    // initializes state[n] with a seed
    void init_genrand(unsigned long s);

    // generates a random number on [0,0xffffffff]-interval 
    unsigned long genrand_int32();

    // generates a random number on [0,0x7fffffff]-interval 
    long genrand_int31();

    // generates a random number on [0,1]-real-interval 
    float genrand_real1();

    // generates a random number on [0,1)-real-interval 
    float genrand_real2();

    // generates a random number on (0,1)-real-interval
    float genrand_real3();

    // generates a random number on [0,1) with 53-bit resolution
    float genrand_res53();

private:
    // Mersenne twister
    enum MT_ENUM
    {
        N = 624
    };

    void next_state();
    unsigned long state[N]; 
    int left;
    unsigned long *next;

};

