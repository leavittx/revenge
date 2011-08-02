#pragma once

#include "../globals.h"

class Waypoint
{
public:
	Vector3 m_startpos, m_endpos, m_position;

	void init(Vector3& position);
	void update();
	void draw();
	bool isDead();

	float m_timer;
	float m_life;
	float m_initialTime;
};
class Animal
{
public:
	Animal();
	~Animal();

	enum AnimalType
	{
		TYPE_NONE,
		TYPE_STRAIGHT,
		TYPE_CURVED,
		TYPE_DASHED,
	};

	Color4 m_color;

	Vector3 m_direction;
	Vector3 m_v1;
	Vector3 m_v2;

	float m_timer;
	float m_life;
	float m_speed;

	AnimalType m_type;

	static AnimalType getAnimalType();
	
	void init(const Vector3& position);
	void update();
	void draw(float alpha);
	void release();
	bool isDead();
};

enum TextureType
{
	TYPE_KANJI = 0,
	TYPE_KATAGANA = 1,
	TYPE_NUMBER = 2,
	TYPE_RANDOM = 3,
};

string getTextureName(TextureType type);

class Plant
{
public:
	Plant();
	~Plant();

	enum PlantType
	{
		TYPE_NONE,
		TYPE_FLOWER,
		TYPE_EQU,
		TYPE_DASHED,
		TYPE_DASHED_UP,
		TYPE_SYMBOL,
		TYPE_TOWER,
	};

	PlantType m_type;

	static void initStatic();
	void init(const Vector3 &position, PlantType type = TYPE_NONE);
	void update();
	void draw(float alpha);
	void drawMask();
	void release();
	void onBeat();
	void onBeat2();
	bool isDead();

	Color3 m_color;
	Vector3 m_position;
	float m_radius;
	float m_fade;
	float m_beat;
	float m_beat2;
	bool m_blink;

	//flower
	int m_petals;
	float m_petallength;
	float m_rotation;
	float m_rotateSpeed;

	//equ
	CatmullRomCurve *m_curve;
	static float *m_spectrum;

	//dashed
	Vector3 m_position2;
	Vector3 m_v1, m_v2;

	//tower
	float m_height;
	float m_currentHeight;
	float m_growSpeed;
	int m_floors;
	string m_textureName;
	Color3 m_color2;

	//symbol
	int m_symbolCount;
	float m_rowDirection;
	bool m_upwards;
	bool m_broken;
	vector<float> m_symbolFade;
	vector<float> m_symbolRotation;
	vector<float> m_rotationDelta;
	vector<bool> m_symbolEnabled;
	vector<string> m_symbolNames;

	static PlantType getPlantType();
	static vector<Plant*> *sm_plants;

	float m_timer;
	float m_startTimer;
	float m_life;

};


class Quad
{
public:
	Quad() {};
	~Quad() {};

	Vector3 v1, v2, v3, v4;
	Vector3 direction;
	float alpha;
};

class RainParticle
{
public:

	enum RainParticleState
	{
		STATE_NONE,
		STATE_RAINING,
		STATE_GROUND
	};
	Vector3 m_direction;
	Vector3 m_position;
	Color3 m_color;
	Vector3 m_v1, m_v2;
	Matrix m_rotation;
	Vector3 m_p1, m_p2;

	float m_length;
	float m_timer;
	float m_timerStart;
	float m_life;

	RainParticleState m_state;
};

class Cloud
{
public:
	Cloud();
	~Cloud();

	enum CloudType
	{
		TYPE_NONE,
		TYPE_DARK,
		TYPE_SYMBOL,
		TYPE_ROUND,
	};
	void init(const Vector3 &position);
	void update();
	void draw(float alpha, bool useTransparency);
	void drawGlow(float alpha);
	void drawShadow(float alpha);
	void release();
	bool isDead();

	static CloudType getCloudType();

	float getFade();
	float m_timer;
	float m_startTimer;
	float m_life;
	float m_height;
	float m_size;
	float m_transparency;
	Vector3 m_speed;
	vector<Quad*> m_quads;
	Color3 m_glowColor;
	Color3 m_symbolColor;
	vector<string> m_textureNames;

	CloudType m_type;

	static void updateRain();
	static void drawRain();
	static vector<RainParticle *> sm_rain;
	RainParticle *initRainParticle(Cloud *source);
	static bool updateRainParticle(RainParticle *rp);

	float m_rainTimer;
};

class CircuitElement
{
public:

	CircuitElement(){};
	~CircuitElement(){};

	enum CircuitType
	{
		TYPE_NONE,
		TYPE_CIRCLE,
		TYPE_LINE,
	};

	CircuitType m_type;
	Vector3 m_position;
	Color3 m_color;

	float m_timer;
	float m_startTimer;
	float m_life;
	float m_size;

	float m_spawnTimer;
	float m_dieDistance;

	Vector3 m_v1, m_v2;
	bool m_dying;

	static CircuitType getCircuitElementType();
	void init(Vector3 position, CircuitElement *parent);
	void update(Vector3 &cameraposition);
	void draw(float alpha);
	bool isDead();

};

class FadeParticle
{
public:
	Vector3 m_position;
	Vector3 m_speed;
	float m_size;
	float m_fade;
	float m_alpha;
	float m_timer;
	float m_startTimer;
	float m_black;
	float m_sizeIncrease;
	float m_sizeIncreased;
	
	int m_orientation;

	void update();
};

class Tree
{
public:
	Tree();
	~Tree();

	void init();
	void update();
	void draw(float alpha, float circuitalpha, float animalalpha, bool glow);
	void release();
	void onBeat();

	void drawRegular(float alpha, bool glow);
	void drawAnimals(float alpha, bool glow);
	void drawCircuit(float alpha, bool glow);
	void drawParticles(bool glow);

	Vector3 getCameraPoint();
	void addFireParticle(const Vector3& position);
	void addSmokeParticle(const Vector3& position, const Vector3& speed);

	void updateSmokeParticles(vector<FadeParticle*>& particles);
	void renderSmokeParticles(vector<FadeParticle*>& particles);

	void updateFireParticles(vector<FadeParticle*>& particles);
	void renderFireParticles(vector<FadeParticle*>& particles);

	list<Waypoint*> m_waypoints;
	CatmullRomCurve *m_curve;
	CatmullRomCurve *m_curveGrow;
	vector<Animal*> m_animals;

	vector<CircuitElement*> m_circuit;

	static Vector3 sm_cursor;
	float m_timer;
	float m_pos;
	float m_animalTimer;
	float m_circuitTimer;
	float m_particleTimer;

	bool m_onBeat;

	vector<FadeParticle *> m_particlesFire1;
	vector<FadeParticle *> m_particlesFire2;
	vector<FadeParticle *> m_particlesFire3;
	vector<FadeParticle *> m_particlesFire4;
	vector<FadeParticle *> m_particlesSmoke1;
	vector<FadeParticle *> m_particlesSmoke2;
	vector<FadeParticle *> m_particlesSmoke3;
	vector<FadeParticle *> m_particlesSmoke4;

};

class WindParticle
{
public:
	Vector3 m_direction;
	Vector3 m_position;
	Color3 m_color;
	Vector3 m_v1, m_v2;
	Matrix m_rotation;
	Vector3 m_p1, m_p2;

	float m_length;
	float m_timer;
	float m_timerStart;
	float m_life;
};

class TreeScene : public Scene
{
public:
	TreeScene() {};
	~TreeScene() {};
	
	void init();
	void update();
	void draw();
	void release();
	
private:

	void setCam();

	void drawDebug();
	void drawTree(bool glow);
	void drawShadows();
	void drawBackground();
	void drawClouds();
	void drawCloudsMask(bool useTransparency);
	void drawWind();

	float m_timer;
	vector<Tree*> m_trees;
	bool m_debug;

	float m_angle;
	float m_speed;

	float m_plantTimer;
	Plant *createNewPlant(Plant::PlantType type);

	void drawPlants(vector<Plant*> &plants);
	void drawPlantsMask(vector<Plant*> &plants);
	void updatePlants(vector<Plant*> &plants);
	vector<Plant*> m_plants;
	vector<Plant*> m_buildings;

	vector<Cloud*> m_clouds;
	float m_cloudTimer;

	vector<WindParticle *> m_wind;
	float m_windTimer;
	WindParticle *initWindParticle();
	bool updateWindParticle(WindParticle *wp);

	Vector3 m_lightPosition;

	Vector3 m_cursorDirection;
	Vector3 m_cursor;
	Vector3 m_cameraPosition;
	Vector3 m_cameraTarget;
	Vector3 m_cameraTarget2;
	float m_cameraRotation;
	float m_cameraZoom;
	float m_angleTurnVelocity;
	float m_angleTurnSpeed;
	float m_travelSpeedMod;
	float m_cameraRotationSpeed;
	float m_angleSpeed;
	float m_movementSpeed;
	float m_animationSpeed;
	float m_travelSpeed;
	float m_globalZoomAmount;
	float m_cameraDistance;


	
};
