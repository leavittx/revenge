#pragma once
/*
#include "../globals.h"



class BasicParticle
{
public:
	Vector3 pos;
	Vector3 speed;
	Color3 color;
	float size;
	float maxenergy;
	float energy;
	float fade;
	float energydecrease;
	bool alive;

	//added stuff
};

class BasicParticleSystem
{
public:
	BasicParticleSystem();
	~BasicParticleSystem();

	void init();
	void update();
	void draw();
	void addParticle(Vector3& pos, Vector3& speed, Color3& color, float size, float energy, float fade, float energydecrease);
	void setBlendMode(GLuint source, GLuint dest);
	void setTexture(string texture);
	void setGravity(float gravity);
	void setGravity(Vector3& gravity);

	int getParticleCount();
	vector<BasicParticle>* getParticles();

private:
	GLuint m_sourceBlend, m_destBlend;
	string m_texture;
	Vector3 m_gravity;

	vector<BasicParticle> m_particles;
	float gravity;
};

















class WindParticle
{
public:
    float energy;
    float maxenergy;
    float fade;

	bool active;
    float size;
	Color3 color;
    Vector3 position;
    Vector3 direction;
};

class ParticleWind
{
public:
	ParticleWind();
	~ParticleWind();

	void update();
	void draw(float alphamod = 1.0f);
	void init(int maxcount = 500, int addcount = 15, float minsize = 0.04f, float maxsize = 0.10f, float minenergy = 0.05f, 
			  float maxenergy = 0.4f, float minfade = 0.2f, float maxfade = 0.4f, float deviationstrength = 2.0f, Vector3& globaldirection = Vector3(1, 0, 0),
			  Vector3& minpos = Vector3(-1, -1, -1), Vector3& maxpos = Vector3(1, 1, 1), bool preinit = true);
private:

	float m_minSize;
	float m_maxSize;
	float m_minEnergy;
	float m_maxEnergy;
	float m_minFade;
	float m_maxFade;
	float m_deviationStrength;
	Vector3 m_globalDirection;
	Vector3 m_minPos;
	Vector3 m_maxPos;

	int m_count;
	int m_addCount;
	int m_index;
	WindParticle *m_particles;

};



class SmokeParticle
{
public:
	Vector3 pos;
	Vector3 speed;
	Color3 color;
	float size;
	float maxenergy;
	float energy;
	float fade;
	bool alive;
};



enum ParticleSmokeSystemType
{
	PARTICLE_SMOKE_TYPE_NORMAL = 0,
	PARTICLE_SMOKE_TYPE_FADE = 1,
};


class ParticleSmoke
{
public:
	ParticleSmoke();
	~ParticleSmoke();

	void update();
	void draw();
	bool isAlive();
	void init(Vector3& position, 
			  string texture, 
			  Color3& mincolor,
			  Color3& maxcolor,
			  int mincount, 
			  int maxcount, 
			  float minsize, 
			  float maxsize, 
			  float minenergy, 
			  float maxenergy, 
			  float minfade, 
			  float maxfade, 
			  float spread, 
			  float minspeed, 
			  float maxspeed,
			  ParticleSmokeSystemType type);

private:
	ParticleSmokeSystemType m_type;
	bool m_alive;
	int m_count;
	string m_textureName;
	SmokeParticle *m_particles;

};

class ParticleSmokeSystem
{
public:
	ParticleSmokeSystem();
	~ParticleSmokeSystem();

	int getCount();
	void draw();
	void update();
	void init(Vector3& position, 
			  string texture, 
			  Color3& mincolor = Color3(0.5f, 0.5f, 0.5f),
			  Color3& maxcolor = Color3(0.5f, 0.5f, 0.5f),
			  int mincount = 300, 
			  int maxcount = 500, 
			  float minsize = 0.04f, 
			  float maxsize = 0.10f, 
			  float minenergy = 0.05f, 
			  float maxenergy = 0.4f, 
			  float minfade = 0.2f, 
			  float maxfade = 0.4f, 
			  float spread = 0.4f, 
			  float minspeed = 0.01f, 
			  float maxspeed = 0.04f,
			  ParticleSmokeSystemType type = PARTICLE_SMOKE_TYPE_NORMAL);

private:
	vector<ParticleSmoke*> m_smokes;
};




class ParticleUtils
{
public:

	static void update();
	//it is assumed that update() is called before calling project()
	static void project(Vector3& position, float size, Vector3 &v1, Vector3 &v2, Vector3& v3, Vector3 &v4);
	static Vector3 xr, yr, zr;

};


class ParticleSystem
{
public:
	//TODO
	ParticleSystem();
	~ParticleSystem();

};

*/