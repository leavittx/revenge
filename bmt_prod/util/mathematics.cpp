#include "mathematics.h"

// Mersenne twister defines
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UMASK 0x80000000UL /* most significant w-r bits */
#define LMASK 0x7fffffffUL /* least significant r bits */
#define MIXBITS(u,v) ( ((u) & UMASK) | ((v) & LMASK) )
#define TWIST(u,v) ((MIXBITS(u,v) >> 1) ^ ((v)&1UL ? MATRIX_A : 0UL))


Vector3 Math::sphereToCartesian(float radius, float phi, float theta)
{
	float sp = (float)sin(phi);
	float cp = (float)cos(phi);
	float st = (float)sin(theta);
	float ct = (float)cos(theta);
	Vector3 value = Vector3(radius*sp*ct, radius*sp*st, radius*cp);
	return value;
}
Vector3 Math::cartesianToSphere(float x, float y, float z)
{
	float rho = (float)sqrt(x*x+y*y+z*z);
	if (fabsf(rho) < 0.00001f)
		return Vector3(0,0,0);
	/*
	float phi = (float)acos(z/rho);
 float theta = (float)acos(x/(rho*sin(phi)));
*/

	float phi = atan2f(y, x);
	float theta = acosf(z / rho);
	Vector3 value = Vector3(rho, theta, phi);
	return value;
}
float Math::integrateSimpson(float startpoint, float endpoint, int steps, float(*function)(float))
{
	float val = 0.0f;
	float h = (endpoint-startpoint)/steps;
	val += function(startpoint);
	val += function(endpoint);

	for (int i=0;i<steps-1;i++)
	{
		if (!(i&1)) //even
			val += 4*function(h+i*h);
		else //odd
			val += 2*function(h+i*h);
	}
	val *= h/3.0f;
	return val;
}

float Math::calcPosFloat(float value, float limit1, float limit2)
{
	if (value <= limit1)
		return 0;
	if (value >= limit2)
		return 1;
	else
		return (float)(value-limit1)/(limit2-limit1);
}
float Math::calcPosInt(int value, int limit1, int limit2)
{
	if (value <= limit1)
		return 0;
	if (value >= limit2)
		return 1;
	else
		return ((float)(value-limit1))/(limit2-limit1);
}

float Math::calcSmoothStep(float value, float limit1, float limit2)
{ 
	if (value < limit1) return 0;
	if (value >= limit2) return 1;
	float x = clamp<float>((value - limit1)/(limit2 - limit1),0,1);
	return (x*x * (3 - 2*x));
}

float Math::randFloat(float max)
{
	return (float)((rand()%RAND_MAX)/(float)RAND_MAX) * max;
}

float Math::randBetween(float min, float max)
{
	return min + randFloat() * (max - min);
}
float Math::randBetween(Range &r)
{
	return randBetween(r.getMinimum(), r.getMaximum());
}
int Math::randIntBetween(int min, int max)
{
	if (min > max)
	{
		return max + (rand() & (min - max));
	}
	else if (min == max)
	{
		return min;
	}
	else
	{
		return min + (rand() & (max - min));
	}

}

Vector3 Math::randVector(float dx, float dy, float dz)
{
	return Vector3(dx * (randFloat() * 2 - 1.0f),
				   dy * (randFloat() * 2 - 1.0f),
				   dz * (randFloat() * 2 - 1.0f));
}

Vector3 Math::randVectSphere(float radius)
{
	float rho = randFloat()*3.141592f;
	float theta = randFloat()*3.141592f*2;

	return sphereToCartesian(radius, theta, rho);
}

// Mersenne twister
void Math::init_genrand(unsigned long s)
{
	int j;
	state[0]= s & 0xffffffffUL;
	for (j=1; j<N; j++) {
		state[j] = (1812433253UL * (state[j-1] ^ (state[j-1] >> 30)) + j);
		/* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
		/* In the previous versions, MSBs of the seed affect   */
		/* only MSBs of the array state[].                        */
		/* 2002/01/09 modified by Makoto Matsumoto             */
		state[j] &= 0xffffffffUL;  /* for >32 bit machines */
	}
	left = 1; //initf = 1;
}

void Math::next_state()
{
	unsigned long *p=state;
	int j;

	/* if init_genrand() has not been called, */
	/* a default initial seed is used         */
	/* initf is disabled, the class explicitly needs to be initialized
	 * via a constructor which leaves the object to a known state.
	 * moved the default seed to the header. -- Jetro Lauha 2004-10-07
	 */
	//if (initf==0) init_genrand(5489UL);

	left = N;
	next = state;

	for (j=N-M+1; --j; p++)
		*p = p[M] ^ TWIST(p[0], p[1]);

	for (j=M; --j; p++)
		*p = p[M-N] ^ TWIST(p[0], p[1]);

	*p = p[M-N] ^ TWIST(p[0], state[0]);
}

/* generates a random number on [0,0xffffffff]-interval */
unsigned long Math::genrand_int32(void)
{
	unsigned long y;

	if (--left == 0) next_state();
	y = *next++;

	/* Tempering */
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680UL;
	y ^= (y << 15) & 0xefc60000UL;
	y ^= (y >> 18);

	return y;
}

/* generates a random number on [0,0x7fffffff]-interval */
long Math::genrand_int31(void)
{
	unsigned long y;

	if (--left == 0) next_state();
	y = *next++;

	/* Tempering */
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680UL;
	y ^= (y << 15) & 0xefc60000UL;
	y ^= (y >> 18);

	return (long)(y>>1);
}

/* generates a random number on [0,1]-real-interval */
float Math::genrand_real1(void)
{
	unsigned long y;

	if (--left == 0) next_state();
	y = *next++;

	/* Tempering */
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680UL;
	y ^= (y << 15) & 0xefc60000UL;
	y ^= (y >> 18);

	return (float)y * (1.0f/4294967295.0f);
	/* divided by 2^32-1 */
}

/* generates a random number on [0,1)-real-interval */
float Math::genrand_real2(void)
{
	unsigned long y;

	if (--left == 0) next_state();
	y = *next++;

	/* Tempering */
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680UL;
	y ^= (y << 15) & 0xefc60000UL;
	y ^= (y >> 18);

	return (float)y * (1.0f/4294967296.0f);
	/* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
float Math::genrand_real3(void)
{
	unsigned long y;

	if (--left == 0) next_state();
	y = *next++;

	/* Tempering */
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680UL;
	y ^= (y << 15) & 0xefc60000UL;
	y ^= (y >> 18);

	return ((float)y + 0.5f) * (1.0f/4294967296.0f);
	/* divided by 2^32 */
}

/* generates a random number on [0,1) with 53-bit resolution*/
float Math::genrand_res53(void) 
{ 
	unsigned long a=genrand_int32()>>5, b=genrand_int32()>>6;
	return(a*67108864.0f+b)*(1.0f/9007199254740992.f);
} 
