/*
	Note: this whole thing started out from a simple test (that's why it's called Tree.cpp). That's also why the code looks
	like this and yes, it needs serious refactoring but it's a demo made in a few weeks that changed direction during the
	development so what do you expect? :)

	Also, a bit more dynamic randomization stuff added to this in the last minute. Yay..
*/

#include <algorithm>
#include "tree.h"

//for random seed on linux
#if !defined(_WIN32)
#include <time.h>
#endif

using namespace std;

Tree *g_particleTree = 0; //bad bad bad!!!!! :D

static float colorprobmod = 0.0f;
static float colorswitchspeed = 0.0f;
static float colorcoeff1 = 0.0f;
static float colorcoeff2 = 0.0f;
static float colorcoeff3 = 0.0f;
static float colorcoeff4 = 0.0f;

Color3 getColor(bool forceRandom)
{
	if (g_system.getRandomized())
	{
		if (colorprobmod < 0.01f)
		{
			//initialize randomized color values
			colorprobmod = Math::randBetween(0.3f, 0.8f);
			colorswitchspeed = Math::randBetween(0.0001f * 0.2f, 0.0001f * 8.0f);
			colorcoeff1 = Math::randBetween(0.3f, 4.0f);
			colorcoeff2 = Math::randBetween(0.3f, 4.0f);
			colorcoeff3 = Math::randBetween(0.3f, 4.0f);
			colorcoeff4 = Math::randBetween(0.3f, 4.0f);
		}
		float randomprob = g_system.event("colorize").getValue() * colorprobmod;
		if (Math::randFloat() < randomprob || forceRandom)
		{
			return Color3(Math::randFloat(), Math::randFloat(), Math::randFloat());
		}
		else
		{
			float t = g_system.getTime() * colorswitchspeed;

			float r = 0.5f + 0.5f * sinf(t);
			float g = 0.5f + 0.5f * (0.5f * cosf(t*colorcoeff1) + 0.5f * sinf(t * colorcoeff2));
			float b = 0.5f + 0.5f * (0.5f * sinf(t*colorcoeff3) + 0.5f * cosf(t*colorcoeff4));
			return Color3(r, g, b);
		}
	}
	else
	{
		float randomprob = g_system.event("colorize").getValue() * 0.5f;
		if (Math::randFloat() < randomprob || forceRandom)
		{
			return Color3(Math::randFloat(), Math::randFloat(), Math::randFloat());
		}
		else
		{
			float t = g_system.getTime() * 0.0001f;

			float r = 0.5f + 0.5f * sinf(t);
			float g = 0.5f + 0.5f * (0.5f * cosf(t*0.89f) + 0.5f * sinf(t * 1.1f));
			float b = 0.5f + 0.5f * (0.5f * sinf(t*1.4f) + 0.5f * cosf(t*1.7f));
			return Color3(r, g, b);
		}
	}
}

static Color3 circuitcolor = Color3(0, 0, 0);
static bool circuitcolorinit = false;
static bool circuitsdifferentcolor = false;

Color3 getCircuitColor()
{
	if (g_system.getRandomized())
	{
		if (!circuitcolorinit)
		{
			circuitcolor = Color3(Math::randBetween(0.3f, 1.0f),
								  Math::randBetween(0.3f, 1.0f),
								  Math::randBetween(0.3f, 1.0f)).normalize();
			if (Math::randFloat() < 0.5f)
			{
				circuitsdifferentcolor = true;
			}
			circuitcolorinit = true;
		}
		if (circuitsdifferentcolor)
		{
			circuitcolor = Color3(Math::randBetween(0.3f, 1.0f),
								  Math::randBetween(0.3f, 1.0f),
								  Math::randBetween(0.3f, 1.0f)).normalize();
		}
		return circuitcolor;
	}
	else
	{
		return Color3(0.6f, 0.9f, 1.0f);
	}
}
Color3 getWindColor()
{
	return Color3(1,1,1);
}

void Waypoint::init(Vector3& position)
{
	m_timer = g_params.getFloat("waypointinitialtime");
	m_initialTime = m_timer;
	m_startpos = position;
	m_endpos = position + Math::randVectSphere() * 0.1f;
}
void Waypoint::draw()
{
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor4f(1, 1, 1, m_timer);
	glVertex3fv((float *)&m_position);
//	glColor4f(1, 0, 0, m_timer);
//	glVertex3fv((float *)&m_startpos);
//	glColor4f(0, 0, 1, m_timer);
//	glVertex3fv((float *)&m_endpos);
	glEnd();

}
void Waypoint::update()
{
	m_timer -= 0.01f;
	m_life = m_timer / m_initialTime;
	m_position = m_startpos + (m_endpos - m_startpos) * m_life;
}
bool Waypoint::isDead()
{
	return m_timer < 0.0f;
}


CircuitElement::CircuitType CircuitElement::getCircuitElementType()
{
	return TYPE_CIRCLE;
}
void CircuitElement::draw(float alpha)
{
	float fadein = ((1.0f - m_life) < 0.5f) ? (min((1.0f - m_life) * 5, 1.0f)) : min(1.0f, m_life * 5.0f);
	float ca = sinf(m_life * 3.141592f);
	m_color.useWithAlpha(ca);

	if (m_dying)
	{
//		glColor4f(1, 0, 0, ca);
	}

	switch(m_type)
	{
		case TYPE_NONE:
			break;
		case TYPE_CIRCLE:
		{
			const int count = 30;

			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < count; i++)
			{
				float t = i / (float)(count - 1);
				float a = t * 2 * 3.141592f * fadein;
				Vector3 p = m_position + Vector3(sinf(a), cosf(a), 0.0f) * m_size;
				glVertex3fv((float *)&p);
			}
			glEnd();

		} break;

		case TYPE_LINE:
		{
			glBegin(GL_LINES);
			glVertex3fv((float *)&m_v1);
#ifdef _MSC_VER
			glVertex3fv((float *)&(m_v1 + (m_v2 - m_v1) * fadein));
#else
                        Vector3 tmp = m_v1 + (m_v2 - m_v1) * fadein;
                        glVertex3fv((float *)&(tmp));
#endif
			glEnd();

		} break;
	}
}
void CircuitElement::init(Vector3 position, CircuitElement *parent)
{
	m_color = getCircuitColor();

	if (parent == 0)
	{
		m_type = getCircuitElementType();
	}
	else
	{
		if (parent->m_type == TYPE_CIRCLE)
			m_type = TYPE_LINE;
		else if (parent->m_type == TYPE_LINE)
			m_type = TYPE_CIRCLE;

	}

	m_timer = g_params.getRange("circuittime").getRandomValue();
	m_startTimer = m_timer;
	switch(m_type)
	{
		case TYPE_CIRCLE:
		{
			m_size = g_params.getRange("circuitcirclesize").getRandomValue();
			if (parent == 0)
			{
				float d = g_params.getRange("circuitrange").getRandomValue();
				float a = Math::randFloat() * 2 * 3.141592f;
				m_position = position + Vector3(sinf(a), cosf(a), 0.0f) * d;
			}
			else
			{
				//pallo tulee tikun päähän
				Vector3 parentdir = (parent->m_v2 - parent->m_v1).normalize();
				m_position = parent->m_v2 + parentdir * m_size * 0.5f;
			}

		} break;

		case TYPE_LINE:
		{
			if (parent == 0)
			{
				float d = g_params.getRange("circuitrange").getRandomValue();
				float a = Math::randFloat() * 2 * 3.141592f;
				m_position = position + Vector3(sinf(a), cosf(a), 0.0f) * d;

				float l = g_params.getRange("circuitlinelength").getRandomValue();

				Matrix r = Matrix::rotation(0, 0, Math::randFloat()*2*3.141592f);
				m_v1 = m_position + Vector3(-l, 0, 0) * r;
				m_v2 = m_position + Vector3( l, 0, 0) * r;
			}
			else
			{
				if (parent->m_type == TYPE_CIRCLE)
				{
					float a = Math::randFloat() * 2 * 3.141592f;
					Vector3 dir = (parent->m_position - position).normalize();//Vector3(sinf(a), cosf(a), 0.0f); //unit

					dir *= Matrix::rotation(0, 0, Math::randBetween(-0.1f, 0.1f));

					float l = g_params.getRange("circuitlinelength").getRandomValue();
					m_v1 = parent->m_position + dir * parent->m_size * 0.5f;
					m_v2 = m_v1 + dir * l;
				}
			}
		} break;
	}
	m_life = m_timer / m_startTimer;
	m_spawnTimer = g_params.getRange("circuitspawntime").getRandomValue();
	m_dying = false;

	m_dieDistance = g_params.getFloat("circuitdiedistance");

}
void CircuitElement::update(Vector3& cameraposition)
{
	m_timer -= 0.01f;
	m_life = m_timer / m_startTimer;

	m_spawnTimer -= 0.01f;

	if (m_type == TYPE_CIRCLE)
	{
		if ((m_position - cameraposition).length() > m_dieDistance)
		{
			m_dying = true;
		}

	}
	else if (m_type == TYPE_LINE)
	{
		if ((m_v2 - cameraposition).length() > m_dieDistance)
		{
			m_dying = true;
		}
	}

}

bool CircuitElement::isDead()
{
	return m_timer < 0.0f;
}



Vector3 Tree::sm_cursor = Vector3(0, 0, 0);

Tree::Tree()
{
}
Tree::~Tree()
{
}
void Tree::init()
{
	m_waypoints.clear();
	m_curve = new CatmullRomCurve();
	m_curveGrow = new CatmullRomCurve();

	for (int i = 0; i < 4; i++)
	{
		Waypoint *w = new Waypoint();
		w->init(sm_cursor);
		m_waypoints.push_back(w);
	}
	m_timer = 0.0f;

	m_pos = 0.0f;
	m_animals.clear();
	m_animalTimer = 0.0f;

	m_circuit.clear();
	m_circuitTimer = 0.0f;

	m_particlesFire1.clear();
	m_particlesFire2.clear();
	m_particlesFire3.clear();
	m_particlesFire4.clear();
	m_particlesSmoke1.clear();
	m_particlesSmoke2.clear();
	m_particlesSmoke3.clear();
	m_particlesSmoke4.clear();
	m_particleTimer = 0.0f;
}

void FadeParticle::update()
{
	m_timer -= 0.01f;
	m_timer = max(0.0f, m_timer);
	m_alpha = m_fade * powf(sinf((m_timer / m_startTimer)*3.141592f), 3.0f);
	m_size += m_sizeIncrease;
	m_position += m_speed;
}

void Tree::addFireParticle(const Vector3& position)
{
//	if (g_system.event("firegoesout").hasPassed())
//		return;

	float r = Math::randFloat();
	if (r < g_system.event("emitparticles").getValue())
	{
		FadeParticle *p = new FadeParticle();
		p->m_position = position;
		p->m_timer = g_params.getRange("particletime").getRandomValue();
		p->m_startTimer = p->m_timer;
		p->m_size = g_params.getRange("particlesize").getRandomValue();
		p->m_fade = g_params.getRange("particlefade").getRandomValue();
		p->m_sizeIncrease = 0.0f;

		float a = Math::randFloat()*2*3.141592f;
		p->m_speed = Vector3(sinf(a), cosf(a), 0.0f) * g_params.getRange("particlespeed").getRandomValue();
		p->m_speed.z = g_params.getRange("particlerise").getRandomValue();
		switch(rand()%4)
		{
			case 0: m_particlesFire1.push_back(p); break;
			case 1: m_particlesFire2.push_back(p); break;
			case 2: m_particlesFire3.push_back(p); break;
			case 3: m_particlesFire4.push_back(p); break;
		}
	}
}

void Tree::addSmokeParticle(const Vector3& position, const Vector3& speed)
{
	FadeParticle *s = new FadeParticle();
	s->m_position = position;
	s->m_timer = g_params.getRange("particlesmoketime").getRandomValue();
	s->m_startTimer = s->m_timer;
	s->m_alpha = 0.0f;
	s->m_black = g_params.getRange("particleblack").getRandomValue();
	s->m_fade = g_params.getRange("particlesmokefade").getRandomValue();
	s->m_size = g_params.getRange("particlesmokesize").getRandomValue();//p->m_size;
	s->m_sizeIncrease = g_params.getRange("particlesmokesizeincrease").getRandomValue();
	s->m_speed = speed;
	s->m_orientation = rand()%4;

//	Matrix rot = Matrix::rotation(0, 0, Math::randFloat()*2*3.141592f);
//	s->m_v1 = Vector3(-s->m_size, -s->m_size, 0) * rot;
//	s->m_v2 = Vector3( s->m_size, -s->m_size, 0) * rot;
//	s->m_v3 = Vector3( s->m_size,  s->m_size, 0) * rot;
//	s->m_v4 = Vector3(-s->m_size,  s->m_size, 0) * rot;

	switch(rand()%4)
	{
		case 0: m_particlesSmoke1.push_back(s); break;
		case 1: m_particlesSmoke2.push_back(s); break;
		case 2: m_particlesSmoke3.push_back(s); break;
		case 3: m_particlesSmoke4.push_back(s); break;
	}
}


void Tree::onBeat()
{
	m_onBeat = true;
}

void Tree::updateSmokeParticles(vector<FadeParticle*>& particles)
{
	for (vector<FadeParticle*>::iterator it = particles.begin(); it != particles.end(); )
	{
		FadeParticle* p = *it;

		if (p->m_timer < 0.001f)
		{
			delete p;
			it = particles.erase(it);
		}
		else
		{
			p->update();
			it++;
		}
	}
}

void Tree::updateFireParticles(vector<FadeParticle*>& particles)
{
	for (vector<FadeParticle*>::iterator it = particles.begin(); it != particles.end(); )
	{
		FadeParticle* p = *it;

		if (p->m_timer < 0.001f)
		{
			addSmokeParticle(p->m_position, p->m_speed);
			delete p;
			it = particles.erase(it);
		}
		else
		{
			p->update();
			it++;
		}
	}
}

void Tree::update()
{
	for (list<Waypoint*>::iterator it = m_waypoints.begin(); it != m_waypoints.end(); )
	{
		Waypoint* w = *it;
		w->update();
		if (w->isDead())
		{
			delete w;
			it = m_waypoints.erase(it);
		}
		else
		{
			it++;
		}
	}

	m_curve->begin();

	int waypointCount = m_waypoints.size();
	int count = 0;
	for (list<Waypoint*>::iterator it = m_waypoints.begin(); it != m_waypoints.end(); it++)
	{
		Waypoint* w = *it;
		m_curve->addPoint(w->m_position);

		count++;
		if (count > waypointCount - 2)
			break;
	}
	m_curve->end();


	list<Waypoint*>::iterator it2 = m_waypoints.end();
	it2--;
	it2--;
	it2--;
	it2--;

	m_curveGrow->begin();
	for (list<Waypoint*>::iterator it = it2; it != m_waypoints.end(); it++)
	{
		Waypoint* w = *it;
		m_curveGrow->addPoint(w->m_position);
	}
	m_curveGrow->end();

	m_timer += 0.01f;
	const float ADD_THRESHOLD = g_params.getFloat("waypointaddthreshold");

	while (m_timer > ADD_THRESHOLD)
	{
		//new waypoints needed

		Waypoint *last = (m_waypoints.back());
		Waypoint *w = new Waypoint();

		Range spread = g_params.getRange("waypointspread");
		Vector3 delta = (Vector3(0, 1, 0) * Matrix::rotation(0, 0, (rand()%4)*0.50f * 3.141592f))*spread.getRandomValue();

		Vector3 npos = sm_cursor + delta;// + Math::randVectSphere() * g_params.getRange("waypointspread").getRandomValue();
		w->init(npos);
		m_waypoints.push_back(w);

		m_timer -= ADD_THRESHOLD;
	}

	m_pos = m_timer / ADD_THRESHOLD;
	m_pos = min(m_pos, 1.0f);

	for (vector<Animal*>::iterator it = m_animals.begin(); it != m_animals.end(); )
	{
		Animal* t = *it;

		if (t->isDead())
		{
			if (!g_system.event("firegoesout").hasPassed())
			{
				addFireParticle(t->m_v2);
			}
			delete t;
			it = m_animals.erase(it);
		}
		else
		{
			t->update();
			it++;
		}
	}

	m_animalTimer += 0.01f;

	const float ANIMAL_THRESHOLD = g_params.getFloat("animalthreshold");
	if (m_animalTimer > ANIMAL_THRESHOLD)
	{
		if (!g_system.event("animalfadeout").hasPassed())
		{
			Animal *a = new Animal();
			a->init(m_curve->getValue(Math::randFloat()));
			m_animals.push_back(a);
		}
	}

	float diedistance = g_params.getFloat("circuitdiedistance");

	bool beat = m_onBeat;
	m_onBeat = false;
	vector<CircuitElement*> spawn;
	float doubleprobability = g_params.getFloat("circuitdoubleprobability");
	int gap = g_params.getInt("circuitmax");

	bool circuitfull = (int)m_circuit.size() > gap;

	for (vector<CircuitElement*>::iterator it = m_circuit.begin(); it != m_circuit.end(); )
	{
		CircuitElement* t = *it;

		if (t->isDead())
		{
			if (t->m_type == CircuitElement::TYPE_CIRCLE)
			{
				addFireParticle(t->m_position);
				addFireParticle(t->m_position);
				addFireParticle(t->m_position);
				addFireParticle(t->m_position);
			}
			else if (t->m_type == CircuitElement::TYPE_LINE)
			{
				addFireParticle(t->m_v2);
				addFireParticle(t->m_v2);
				addFireParticle(t->m_v2);
				addFireParticle(t->m_v2);
			}
			delete t;
			it = m_circuit.erase(it);
		}
		else
		{
			Vector3 camerapos = m_curveGrow->getValue(m_pos);
			t->update(camerapos);
/*
			if ((t->m_position - ).length() > diedistance)
			{
				t->m_dying = true;
			}
*/
			if (beat && !t->m_dying && !circuitfull)//t->m_spawnTimer < 0.0f && !t->m_dying)
			{
//				t->m_spawnTimer = g_params.getRange("circuitspawntime").getRandomValue();
				int add = 1;//(Math::randFloat() < doubleprobability) ? 2 : 1;
				for (int i = 0; i < add; i++)
				{
					CircuitElement *c = new CircuitElement();
					c->init(camerapos, t);
					spawn.push_back(c);
				}
			}
			it++;
		}
	}

	m_circuit.insert(m_circuit.end(), spawn.begin(), spawn.end());
	m_circuitTimer += 0.01f;

	const float CIRCUIT_THRESHOLD = g_params.getFloat("circuitthreshold");
	while (m_circuitTimer > CIRCUIT_THRESHOLD)
	{
		if (g_system.event("circuitfadein").hasPassed())
		{
			int add = g_params.getInt("circuitadd");
			for (int i = 0; i < add; i++)
			{
				CircuitElement *e = new CircuitElement();
				e->init(m_curve->getValue(0.99f), 0);
				m_circuit.push_back(e);
			}
		}
		m_circuitTimer -= CIRCUIT_THRESHOLD;
	}

	updateSmokeParticles(m_particlesSmoke1);
	updateSmokeParticles(m_particlesSmoke2);
	updateSmokeParticles(m_particlesSmoke3);
	updateSmokeParticles(m_particlesSmoke4);

	updateFireParticles(m_particlesFire1);
	updateFireParticles(m_particlesFire2);
	updateFireParticles(m_particlesFire3);
	updateFireParticles(m_particlesFire4);

	m_particleTimer += 0.01f;

	const float PARTICLE_THRESHOLD = g_params.getFloat("particlethreshold");
	while (m_particleTimer > PARTICLE_THRESHOLD)
	{

		float r = Math::randFloat()*1.001f;
		bool emit = r < g_system.event("emitparticles").getValue();
		if (emit && !g_system.event("firegoesout").hasPassed())
		{
			int add = (int)g_params.getRange("particleadd").getRandomValue();
			for (int i = 0; i < add; i++)
			{
				float t = Math::randFloat();
				addFireParticle(m_curve->getValue(t));
			}
		}
		m_particleTimer -= PARTICLE_THRESHOLD;
	}

}
void Tree::draw(float alpha, float circuitalpha, float animalalpha, bool glow)
{
	drawAnimals(animalalpha, glow);
	drawRegular(alpha, glow);
	drawCircuit(circuitalpha, glow);
}


void Tree::renderSmokeParticles(vector<FadeParticle*>& particles)
{
	Vector3 xr, yr, zr;
	glUtil::antiRotate(xr, yr, zr);

	glBegin(GL_QUADS);
	for (vector<FadeParticle*>::iterator it = particles.begin(); it != particles.end(); it++)
	{
		FadeParticle *p = *it;
		glColor4f(p->m_black,p->m_black, p->m_black, p->m_alpha);

		float size = p->m_size;
		Vector3& v = p->m_position;

//		Matrix rot = Matrix::rotation(0, 0, (p->m_timer / p->m_startTimer)*10);
//		Vector3 xr2 = xr * rot;
//		Vector3 yr2 = yr * rot;

		Vector3 v1 = v - xr * size - yr * size;// + p->m_v1;
		Vector3 v2 = v + xr * size - yr * size;// + p->m_v2;
		Vector3 v3 = v + xr * size + yr * size;// + p->m_v3;
		Vector3 v4 = v - xr * size + yr * size;// + p->m_v4;

		switch(p->m_orientation)
		{
			case 0:
			{
				glTexCoord2f(0, 0);
				glVertex3fv((float *)&v1);
				glTexCoord2f(1, 0);
				glVertex3fv((float *)&v2);
				glTexCoord2f(1, 1);
				glVertex3fv((float *)&v3);
				glTexCoord2f(0, 1);
				glVertex3fv((float *)&v4);
			} break;
			case 1:
			{
				glTexCoord2f(0, 0);
				glVertex3fv((float *)&v2);
				glTexCoord2f(1, 0);
				glVertex3fv((float *)&v3);
				glTexCoord2f(1, 1);
				glVertex3fv((float *)&v4);
				glTexCoord2f(0, 1);
				glVertex3fv((float *)&v1);
			} break;
			case 2:
			{
				glTexCoord2f(0, 0);
				glVertex3fv((float *)&v3);
				glTexCoord2f(1, 0);
				glVertex3fv((float *)&v4);
				glTexCoord2f(1, 1);
				glVertex3fv((float *)&v1);
				glTexCoord2f(0, 1);
				glVertex3fv((float *)&v2);
			} break;
			case 3:
			{
				glTexCoord2f(0, 0);
				glVertex3fv((float *)&v4);
				glTexCoord2f(1, 0);
				glVertex3fv((float *)&v1);
				glTexCoord2f(1, 1);
				glVertex3fv((float *)&v2);
				glTexCoord2f(0, 1);
				glVertex3fv((float *)&v3);
			} break;

		}
	}
	glEnd();
}

void Tree::renderFireParticles(vector<FadeParticle*>& particles)
{
	Vector3 xr, yr, zr;
	glUtil::antiRotate(xr, yr, zr);
	glBegin(GL_QUADS);
	for (vector<FadeParticle*>::iterator it = particles.begin(); it != particles.end(); it++)
	{
		FadeParticle *p = *it;
		glColor4f(1,1,1,p->m_alpha);

		float size = p->m_size;
		Vector3& v = p->m_position;

		Vector3 v1 = v - xr * size - yr * size;
		Vector3 v2 = v + xr * size - yr * size;
		Vector3 v3 = v + xr * size + yr * size;
		Vector3 v4 = v - xr * size + yr * size;

		glTexCoord2f(0, 0);
		glVertex3fv((float *)&v1);
		glTexCoord2f(1, 0);
		glVertex3fv((float *)&v2);
		glTexCoord2f(1, 1);
		glVertex3fv((float *)&v3);
		glTexCoord2f(0, 1);
		glVertex3fv((float *)&v4);


	}
	glEnd();
}
void Tree::drawRegular(float alpha, bool glow)
{
//	if (alpha < 0.000001f)
//		return;

//	if (m_particlesFire.size() == 0 && m_particlesSmoke.size() == 0)
//		return;

	Vector3 xr, yr, zr;
	glUtil::antiRotate(xr, yr, zr);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(0);

	if (glow)
	{
                g_textures.bindTexture("magellan1.png", GL_TEXTURE0_ARB);
		renderFireParticles(m_particlesFire1);
                g_textures.bindTexture("magellan1.png", GL_TEXTURE0_ARB);
		renderFireParticles(m_particlesFire2);
                g_textures.bindTexture("magellan6.png", GL_TEXTURE0_ARB);
		renderFireParticles(m_particlesFire3);
                g_textures.bindTexture("magellan4.png", GL_TEXTURE0_ARB);
		renderFireParticles(m_particlesFire4);
	}
	if (!glow)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                g_textures.bindTexture("smoke1.png", GL_TEXTURE0_ARB);
		renderSmokeParticles(m_particlesSmoke1);
                g_textures.bindTexture("smoke2.png", GL_TEXTURE0_ARB);
		renderSmokeParticles(m_particlesSmoke2);
                g_textures.bindTexture("smoke3.png", GL_TEXTURE0_ARB);
		renderSmokeParticles(m_particlesSmoke3);
                g_textures.bindTexture("smoke4.png", GL_TEXTURE0_ARB);
		renderSmokeParticles(m_particlesSmoke4);
	}

/*
	for (float t = 0.0f; t < alpha; t+= 0.1f)
	{
		const float size = 0.56f;
		glColor4f(1,1,1,alpha * (0.5f + 0.5f * sinf(t * 20)));
		Vector3 v = m_curve->getValue(t);

		Vector3 v1 = v - xr * size - yr * size;
		Vector3 v2 = v + xr * size - yr * size;
		Vector3 v3 = v + xr * size + yr * size;
		Vector3 v4 = v - xr * size + yr * size;

		glTexCoord2f(0, 0);
		glVertex3fv((float *)&v1);
		glTexCoord2f(1, 0);
		glVertex3fv((float *)&v2);
		glTexCoord2f(1, 1);
		glVertex3fv((float *)&v3);
		glTexCoord2f(0, 1);
		glVertex3fv((float *)&v4);
	}

*/

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(1);
/*


	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glColor4f(1,1,1,1);

	glLineWidth(2.0f);
	glEnable(GL_LINE_SMOOTH);

	const float STEP_SIZE = 0.00001f + g_system.event("treequantfadein").getValue() * g_params.getFloat("treequantvalue");

	glBegin(GL_LINE_STRIP);
	for (float t = 0.0f; t < alpha; t += 0.005f)
	{
		glColor4f(1,1,1,alpha * (0.5f + 0.5f * sinf(t * 20)));
		Vector3 v = m_curve->getValue(t);

		v.x = v.x - fmodf(v.x, STEP_SIZE);
		v.y = v.y - fmodf(v.y, STEP_SIZE);
		v.z = v.z - fmodf(v.z, STEP_SIZE);
		glVertex3fv((float *)&v);
	}
	glEnd();
	if (alpha > 0.99f)
	{
		glBegin(GL_LINES);
		for (float t = 0.0f; t < m_pos; t += 0.01f)
		{
			float norm_t = 1 - t / m_pos;
			glColor4f(1,1,1,norm_t*alpha);
			glVertex3fv((float *)&m_curveGrow->getValue(t));
		}
		glEnd();
	}
*/
}
void Tree::drawAnimals(float alpha, bool glow)
{
	if (glow)
	{
		glLineWidth(3.0f);
		glEnable(GL_LINE_SMOOTH);

		float animalalpha = g_system.event("animalfadein").getValue();
		for (vector<Animal*>::iterator it = m_animals.begin(); it != m_animals.end(); it++)
		{
			Animal* t = *it;
			t->draw(animalalpha);
		}
		glDisable(GL_LINE_SMOOTH);
	}
}

void Tree::drawCircuit(float alpha, bool glow)
{
	if (glow)
	{
		glLineWidth(1.0f);
		glDisable(GL_TEXTURE_2D);

		glEnable(GL_LINE_SMOOTH);
		float circuitalpha = g_system.event("circuitfadein").getValue();
		for (vector<CircuitElement*>::iterator it = m_circuit.begin(); it != m_circuit.end(); it++)
		{
			CircuitElement* t = *it;
			t->draw(circuitalpha);
		}
		glDisable(GL_LINE_SMOOTH);
	}
}
Vector3 Tree::getCameraPoint()
{
	return m_curveGrow->getValue(m_pos);
}

void Tree::release()
{
}


Animal::Animal()
{
}
Animal::~Animal()
{
}
void Animal::draw(float alpha)
{
	if (m_type == TYPE_STRAIGHT)
	{
		glBegin(GL_LINES);
//		glColor4fv((float *)&m_color);
		m_color.setModulation(alpha);
		m_color.use();
		glVertex3fv((float *)&m_v1);
		glVertex3fv((float *)&m_v2);
		glEnd();
	}
	else if (m_type == TYPE_CURVED)
	{
	}
	else if (m_type == TYPE_DASHED)
	{
		Primitives::dashedLine(m_v1, m_v2, 0.15f, 0.5f);
	}
}


Animal::AnimalType Animal::getAnimalType()
{
	AnimalType type = TYPE_NONE;
	float dashed = g_system.event("dashedanimalstart").getValue();
	float r = Math::randFloat();

	const float proportion = 0.1f * dashed;
	if (r < proportion)
	{
		type = TYPE_DASHED;
	}
	else
	{
		type = TYPE_STRAIGHT;
	}

	return type;
}

void Animal::init(const Vector3& position)
{
	m_type = getAnimalType();

	if (m_type == TYPE_STRAIGHT)
	{
		m_life = g_params.getRange("animallife").getRandomValue();
		m_speed = g_params.getRange("animalspeed").getRandomValue();
		m_color = Color4(Math::randFloat(), Math::randFloat(), Math::randFloat(), Math::randFloat());

		m_direction = (Vector3(0, 1, 0) * Matrix::rotation(0, 0, (rand()%4)*0.50f * 3.141592f));
		m_v1 = position;
		m_v2 = m_v1 + m_direction * g_params.getRange("animalsize").getRandomValue();
	}
	else if (m_type == TYPE_CURVED)
	{

	}
	else if (m_type == TYPE_DASHED)
	{
		m_life = g_params.getRange("animallife").getRandomValue();
		m_speed = g_params.getRange("animalspeed").getRandomValue();
		m_color = Color4(Math::randFloat(), Math::randFloat(), Math::randFloat(), Math::randFloat());

		m_direction = (Vector3(0, 1, 0) * Matrix::rotation(0, 0, (rand()%4)*0.50f * 3.141592f));
		m_v1 = position;
		m_v2 = m_v1 + m_direction * g_params.getRange("animalsize").getRandomValue();
	}
}
void Animal::release()
{
}
void Animal::update()
{
	m_life -= 0.01f;

	if (m_type == TYPE_STRAIGHT)
	{
		m_v1 += m_direction * m_speed;
		m_v2 += m_direction * m_speed;
	}
	else if (m_type == TYPE_CURVED)
	{
	}
	else if (m_type == TYPE_DASHED)
	{
		//v1 stays where it's at
		m_v2 += m_direction * m_speed;
	}
}
bool Animal::isDead()
{
	return m_life < 0.0f;
}




float *Plant::m_spectrum = 0;
vector<Plant*> *Plant::sm_plants = 0;

Plant::Plant()
{
}

Plant::~Plant()
{
}

void Plant::initStatic()
{
	m_spectrum = new float[512];
	for (int i = 0; i < 512; i++)
	{
		m_spectrum[i] = 0.0f;
	}
	if (sm_plants != 0)
	{
		sm_plants->clear();
	}
}


static bool planttypeinit = false;
static bool canhasflower = true;
static bool canhassymbol = true;
static bool canhasequ = true;
static bool canhasup = true;
static bool canhastower = true;
static bool towersinstart = false;

Plant::PlantType Plant::getPlantType()
{
	//can has what?
	if (!planttypeinit)
	{
		if (g_system.getRandomized())
		{
			canhasflower = Math::randFloat() < 0.8f;
			canhassymbol = Math::randFloat() < 0.8f;
			canhasequ = Math::randFloat() < 0.8f;
			canhasup = Math::randFloat() < 0.8f;
			canhastower = Math::randFloat() < 0.8f;
			towersinstart = Math::randFloat() < 0.2f;
		}
		else
		{
		}
		planttypeinit = true;
		canhasflower = true;
		canhassymbol = true;
		canhasequ = true;
		canhasup = true;
		canhastower = true;
		towersinstart = false;
	}
	bool flower = !g_system.event("flowerfadeout").hasPassed() && canhasflower;
	bool symbol = g_system.event("symbolstart").hasPassed() && canhassymbol;
	bool equ = g_system.event("equplantfadein").hasPassed() && canhasequ;
	bool up = g_system.event("upplantfadein").hasPassed() && canhasup;
	bool tower = (g_system.event("towerfadein").hasPassed() || towersinstart) && canhastower;

hahaevilgotohere:
	PlantType type = TYPE_FLOWER;

	if (symbol)
	{
		if (Math::randFloat() < 0.95f)
		{
			type = TYPE_SYMBOL;
		}
	}

	if (equ)
	{
		if (Math::randFloat() < 0.2f)
		{
			type = TYPE_EQU;
		}
	}
	if (tower)
	{
		if (Math::randFloat() < 0.25f)
		{
			type = TYPE_TOWER;
		}
	}
	if (type == TYPE_FLOWER && !canhasflower)
	{
		if (!canhastower && !canhasup && !canhassymbol && !canhasequ)
		{
			canhastower = true;
		}
		goto hahaevilgotohere;
	}

/*
	if (g_system.event("flowerfadeout").hasPassed())
	{
		type = TYPE_SYMBOL;
	}

	if (Math::randFloat() < 0.3f)
	{
		type = TYPE_DASHED;
		if (g_system.event("upplantfadein").hasPassed())
		{
			//if (Math::randFloat() < 0.3f)
			{
				type = TYPE_DASHED_UP;
			}
		}
	}
	else if (Math::randFloat() < 0.5f)
	{
		type = TYPE_SYMBOL;

	}

	if (g_system.event("equplantfadein").hasPassed())
	{
		if (Math::randFloat() < 0.25f)
		{
			type = TYPE_EQU;
		}
	}
*/
	return type;
}

string getTextureName(TextureType type)
{
	if (type == TYPE_RANDOM)
	{
		float r = Math::randFloat();
		if (r < 0.33f)
		{
			type = TYPE_KATAGANA;
		}
		else if (r < 0.66f)
		{
			type = TYPE_KANJI;
		}
		else
		{
			type = TYPE_NUMBER;
		}
	}


	if (type == TYPE_NUMBER)
	{
		int index = rand()%4;
		switch(index)
		{
			case 0: return "2.png";
			case 1: return "0.png";
			case 2: return "1.png";
			case 3: return "9.png";
		}
	}
	else if (type == TYPE_KATAGANA)
	{
		const int KATAGANA_COUNT = 25;
		int num = rand()%KATAGANA_COUNT;

		stringstream ss;
		ss << num;
		return "katagana" + ss.str() + ".png";
	}
	else if (type == TYPE_KANJI)
	{
		const int KANJI_COUNT = 15;
		int num = rand()%KANJI_COUNT;

		stringstream ss;
		ss << num;
		return "kanji" + ss.str() + ".png";
	}
	return "makecompilerhappy";
}
void Plant::init(const Vector3 &position, PlantType type)
{
	if (type == TYPE_NONE)
	{
		//get random
		m_type = getPlantType();
	}
	else
	{
		m_type = type;

	}
	m_rotation = 0.0f;
	m_beat = -666.0f;
	m_beat2 = 0.0f;
	m_blink = false;

	if (m_type == TYPE_FLOWER)
	{
		m_startTimer = g_params.getRange("plantflowertime").getRandomValue();
		m_timer = m_startTimer;
		m_radius = g_params.getRange("plantflowerradius").getRandomValue();
		m_fade = g_params.getRange("plantflowerfade").getRandomValue();
		m_petals = (int)(g_params.getRange("plantflowerpetals").getRandomValue());
		m_petallength = g_params.getRange("plantflowerpetallength").getRandomValue();
		m_rotateSpeed = g_params.getRange("plantflowerrotationspeed").getRandomValue();

		m_color = getColor(false);//Color3(Math::randFloat(), Math::randFloat(), Math::randFloat());
		m_position = position;
	}
	else if (m_type == TYPE_TOWER)
	{
		m_startTimer = g_params.getRange("planttowertime").getRandomValue();
		m_timer = m_startTimer;
		m_radius = g_params.getRange("planttowerradius").getRandomValue();
		m_fade = g_params.getRange("planttowerfade").getRandomValue();
		m_height = g_params.getRange("planttowerheight").getRandomValue();
		m_floors = (int)g_params.getRange("planttowerfloors").getRandomValue();
		m_growSpeed = g_params.getRange("planttowergrowspeed").getRandomValue();
		m_currentHeight = 0.0f;
		stringstream ss;
		ss << "kanji" << (8 + rand()%7) << ".png"; //hack to fix resources
		m_textureName = ss.str();//getTextureName(TYPE_KANJI);
		m_color = getColor(true);
		if (Math::randFloat() < 0.4f)
		{
			m_color2 = getColor(true);
		}
		else
		{
			m_color2 = m_color;
		}

		int tries = 0;

		//... nice
		bool tooclose = false;
		m_position = position;
		do
		{
			tooclose = false;
			const float MIN_LENGTH = 2.0f;
			Vector3 pushaway;
			float length = 666.0f;

			for (vector<Plant*>::iterator it = sm_plants->begin(); it != sm_plants->end(); it++)
			{
				Plant* p = *it;
				if (p->m_type == TYPE_TOWER)
				{
					Vector3 dir = m_position - p->m_position;
					float l = dir.length();
					if (l < MIN_LENGTH && l < length && l > 0.0000666f)
					{
						tooclose = true ;
						pushaway = dir.normalize() * (MIN_LENGTH / l) * 1.2f;
						length = l;
					}
				}
			}

			if (tooclose)
			{
				m_position += pushaway;
			}
			tries++;
		} while (tries < 10 && tooclose);

	}
	else if (m_type == TYPE_SYMBOL)
	{
		m_startTimer = g_params.getRange("plantsymboltime").getRandomValue();
		m_timer = m_startTimer;
		m_fade = g_params.getRange("plantsymbolfade").getRandomValue();
		m_radius = g_params.getRange("plantsymbolsize").getRandomValue();
		m_color = getColor(false);
		m_position = position;
		m_rotation = (rand()%4)*0.5f * 3.141592f;
		m_broken = false;

		if (Math::randFloat() < g_params.getFloat("symbolrowprobability"))
		{
			m_symbolCount = (int)g_params.getRange("symbolrowcount").getRandomValue();
			if (Math::randFloat() < g_params.getFloat("symbolupwardsprobability") && !g_system.event("plantstartagain").hasPassed())
			{
				m_upwards = true;
			}
			if (Math::randFloat() < 0.2f)
				m_broken = true;
		}
		else
		{
			m_symbolCount = 2;
			m_upwards = false;
		}

		m_blink = Math::randFloat() < 0.3f;

		TextureType type = TYPE_KANJI;


		if (m_symbolCount > 2)
		{
			float r = Math::randFloat();
			if (r < 0.33f)
			{
				type = TYPE_KATAGANA;
			}
			else if (r < 0.66f)
			{
				type = TYPE_KANJI;
			}
			else
			{
				type = TYPE_NUMBER;
			}
		}

		for (int i = 0; i < m_symbolCount; i++)
		{
			m_symbolFade.push_back(Math::randBetween(0.3f, 1.0f));
			m_symbolRotation.push_back(Math::randBetween(0, 2*3.141592f));
			m_symbolNames.push_back(getTextureName(type));
			m_symbolEnabled.push_back(true);
			m_rotationDelta.push_back(g_params.getRange("symbolrotation").getRandomValue());
		}
		m_rowDirection = (rand()%4)*0.5f * 3.141592f;
	}
	else if (m_type == TYPE_DASHED || m_type == TYPE_DASHED_UP)
	{
		m_startTimer = g_params.getRange("plantdashedtime").getRandomValue();
		m_timer = m_startTimer;
		m_radius = g_params.getRange("plantdashedradius").getRandomValue();
		m_fade = g_params.getRange("plantdashedfade").getRandomValue();

		m_color = getColor(false);//Color3(Math::randFloat(), Math::randFloat(), Math::randFloat());

		if (m_type == TYPE_DASHED)
		{
			m_position = position;
                        //wtf
			//m_position2 = position + (Vector3(0, 1, 0) * Matrix::rotation(0, 0, (rand()%4)*0.50f * 3.141592f)) * m_radius;
                        m_position2 = m_position + (Vector3(0, 1, 0) * Matrix::rotation(0, 0, (rand()%4)*0.50f * 3.141592f)) * m_radius;
		}
		else
		{
			//up
			m_position = position;
                        //wtf
			//m_position2 = position + Vector3(0, 0, 1) * m_radius;
                        m_position2 = m_position + Vector3(0, 0, 1) * m_radius;
		}
	}
	else if (m_type == TYPE_EQU)
	{
		m_startTimer = g_params.getRange("plantequtime").getRandomValue();
		m_timer = m_startTimer;
		m_radius = g_params.getRange("plantequradius").getRandomValue();
		m_fade = g_params.getRange("plantequfade").getRandomValue();
		m_petals = (int)(g_params.getRange("plantequpetals").getRandomValue());
		m_petallength = g_params.getRange("plantequpetallength").getRandomValue();

		m_color = getColor(false);//Color3(Math::randFloat(), Math::randFloat(), Math::randFloat());
		m_position = position;

		m_curve = new CatmullRomCurve();
		m_curve->begin();

		Vector3 p = m_position;
		m_curve->addPoint(p);
		m_curve->addPoint(p);
		m_curve->addPoint(p);

		const int pcount = (int)(g_params.getRange("plantequpointcount").getRandomValue());
		for (int i = 0; i < pcount; i++)
		{
			p += Vector3(0, 0, 1.0f) * g_params.getRange("plantequpointspread").getRandomValue();
			p += (Vector3(Math::randFloat()-0.5f, Math::randFloat()-0.5f, 0.0f).normalize()) * g_params.getRange("plantequpointwidth").getRandomValue();
			m_curve->addPoint(p);
		}
		m_curve->end();
	}

	m_life = 1.0f;
}
void Plant::update()
{
	if (g_system.event("buildingstopdie").hasPassed() && m_type == TYPE_TOWER)
	{
		//stop collapsing buildings
		if (m_timer > m_startTimer * 0.5f)
		{
			m_timer -= 0.01f;
		}
	}
	else
	{
		m_timer -= 0.01f;
	}
	m_beat -= 0.01f;
	m_beat2 -= 0.003f;
	m_life = m_timer / m_startTimer;


	if (m_type == TYPE_FLOWER)
	{
		m_rotation += m_rotateSpeed;
	}
	if (m_type == TYPE_TOWER)
	{
		m_currentHeight += m_growSpeed;
		m_currentHeight = min(m_currentHeight, m_height);
		m_currentHeight *= min(m_life * 10.0f, 1.0f);

	}

	if (m_type == TYPE_DASHED || m_type == TYPE_DASHED_UP)
	{
		m_v1 = m_position;
		m_v2 = m_position + (m_position2 - m_position) * (1.0f - m_life);
//		g_particleTree->addFireParticle(m_v2);
	}

	if (m_type == TYPE_SYMBOL)
	{
		int size = m_symbolRotation.size();
		for (int i = 0; i < size; i++)
		{
			m_symbolRotation[i] += m_rotationDelta[i];
		}
	}

	if (m_type == TYPE_EQU)
	{

		float grow = min((1.0f - m_life) * 3, 1.0f);
		const float step = 0.1f;
		const float s = fmodf(grow, step);

		if (grow < 0.9999f && s < step / 2.0f)
		{
			Vector3 r = Math::randVectSphere() * grow * 0.6f;
			Vector3 p = m_curve->getValue(grow);
			g_particleTree->addFireParticle(p + r);
		}
	}
}
void Plant::drawMask()
{
	if (m_type == TYPE_TOWER)
	{
		int drawcount = (int)(m_floors * m_currentHeight / m_height);

		float frac = (m_floors * m_currentHeight / m_height) - drawcount;
		if (m_currentHeight / m_height > 0.999f)
		{
			drawcount = m_floors;
			frac = 1.0f;
		}

		if (drawcount < 1)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(0, 0, 0, frac);

		}
		else
		{
			glDisable(GL_BLEND);
			glColor4f(0, 0, 0, 1);
		}
		glEnable(GL_DEPTH_TEST);
		glBegin(GL_QUADS);
		const float less = 0.97f;
		Vector3 r1 = m_position + Vector3(-m_radius * less, -m_radius * less, m_currentHeight);
		Vector3 r2 = m_position + Vector3( m_radius * less, -m_radius * less, m_currentHeight);
		Vector3 r3 = m_position + Vector3( m_radius * less,  m_radius * less, m_currentHeight);
		Vector3 r4 = m_position + Vector3(-m_radius * less,  m_radius * less, m_currentHeight);
		Vector3 g1 = m_position + Vector3(-m_radius * less, -m_radius * less, 0);
		Vector3 g2 = m_position + Vector3( m_radius * less, -m_radius * less, 0);
		Vector3 g3 = m_position + Vector3( m_radius * less,  m_radius * less, 0);
		Vector3 g4 = m_position + Vector3(-m_radius * less,  m_radius * less, 0);
		glVertex3fv((float *)&g1);
		glVertex3fv((float *)&r1);
		glVertex3fv((float *)&r2);
		glVertex3fv((float *)&g2);
		glVertex3fv((float *)&g2);
		glVertex3fv((float *)&r2);
		glVertex3fv((float *)&r3);
		glVertex3fv((float *)&g3);
		glVertex3fv((float *)&g3);
		glVertex3fv((float *)&r3);
		glVertex3fv((float *)&r4);
		glVertex3fv((float *)&g4);
		glVertex3fv((float *)&g4);
		glVertex3fv((float *)&r4);
		glVertex3fv((float *)&r1);
		glVertex3fv((float *)&g1);
		Vector3 v1 = m_position + Vector3(-m_radius, -m_radius, m_currentHeight);
		Vector3 v2 = m_position + Vector3( m_radius, -m_radius, m_currentHeight);
		Vector3 v3 = m_position + Vector3( m_radius,  m_radius, m_currentHeight);
		Vector3 v4 = m_position + Vector3(-m_radius,  m_radius, m_currentHeight);
		glVertex3fv((float *)&v1);
		glVertex3fv((float *)&v2);
		glVertex3fv((float *)&v3);
		glVertex3fv((float *)&v4);
		glEnd();
		glDisable(GL_BLEND);
	}

}

void Plant::draw(float alpha)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_LINE_SMOOTH);
	glDisable(GL_TEXTURE_2D);

	if (m_type == TYPE_FLOWER)
	{
		float a = sinf(m_life*3.141592f);
		m_color.useWithAlpha(a * m_fade * alpha);

		glLineWidth(2.0f);
		const int steps = 50;
		for (int j = 0; j < 2; j++)
		{
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < steps; i++)
			{
				float t = i / (float)steps;
				float a = t * 2 * 3.141592f;

				float r = m_radius - j * 0.1f;
				Vector3 v1 = m_position + Vector3(sinf(a), cosf(a), 0) * r;
				glVertex3fv((float *)&v1);
			}
			glEnd();
		}

		for (int i = 0; i < m_petals; i++)
		{
			float angle = m_rotation + (i / (float)m_petals)*2*3.141592f;
			float l = m_petallength * (1.0f - m_life);

			Vector3 v1 = m_position;
			Vector3 v2 = m_position + Vector3(sinf(angle), cosf(angle), 0) * l;
			Primitives::dashedLine(v1, v2, 0.2f);
		}
	}
	else if (m_type == TYPE_TOWER)
	{
		float f = m_currentHeight / m_height;
		int drawcount = (int)(m_floors * f);

		float frac = (m_floors * f) - drawcount;
		if (f > 0.999f)
		{
			drawcount = m_floors;
			frac = 1.0f;
		}

		glLineWidth(3.0f);


		glDepthMask(1);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_BLEND);

		glEnable(GL_DEPTH_TEST);
		glBegin(GL_LINES);
		for (int i = 0; i < drawcount; i++)
		{
			float t = i / (float)m_floors;
			float h = t * m_height;
			Vector3 v1 = m_position + Vector3(-m_radius, -m_radius, h);
			Vector3 v2 = m_position + Vector3( m_radius, -m_radius, h);
			Vector3 v3 = m_position + Vector3( m_radius,  m_radius, h);
			Vector3 v4 = m_position + Vector3(-m_radius,  m_radius, h);

			m_color.useWithAlpha(m_fade * alpha * ((i == drawcount - 1) ? frac : 1.0f));

			glVertex3fv((float *)&v1);
			glVertex3fv((float *)&v2);
			glVertex3fv((float *)&v2);
			glVertex3fv((float *)&v3);
			glVertex3fv((float *)&v3);
			glVertex3fv((float *)&v4);
			glVertex3fv((float *)&v4);
			glVertex3fv((float *)&v1);
		}
		glEnd();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(1);
		glDisable(GL_BLEND);
		glBegin(GL_QUADS);
		glColor4f(0, 0, 0, 1);
		const float less = 0.97f;
		Vector3 r1 = m_position + Vector3(-m_radius * less, -m_radius * less, m_currentHeight);
		Vector3 r2 = m_position + Vector3( m_radius * less, -m_radius * less, m_currentHeight);
		Vector3 r3 = m_position + Vector3( m_radius * less,  m_radius * less, m_currentHeight);
		Vector3 r4 = m_position + Vector3(-m_radius * less,  m_radius * less, m_currentHeight);
		Vector3 g1 = m_position + Vector3(-m_radius * less, -m_radius * less, 0);
		Vector3 g2 = m_position + Vector3( m_radius * less, -m_radius * less, 0);
		Vector3 g3 = m_position + Vector3( m_radius * less,  m_radius * less, 0);
		Vector3 g4 = m_position + Vector3(-m_radius * less,  m_radius * less, 0);
		glVertex3fv((float *)&g1);
		glVertex3fv((float *)&r1);
		glVertex3fv((float *)&r2);
		glVertex3fv((float *)&g2);
		glVertex3fv((float *)&g2);
		glVertex3fv((float *)&r2);
		glVertex3fv((float *)&r3);
		glVertex3fv((float *)&g3);
		glVertex3fv((float *)&g3);
		glVertex3fv((float *)&r3);
		glVertex3fv((float *)&r4);
		glVertex3fv((float *)&g4);
		glVertex3fv((float *)&g4);
		glVertex3fv((float *)&r4);
		glVertex3fv((float *)&r1);
		glVertex3fv((float *)&g1);
		glEnd();

                g_textures.bindTexture(m_textureName, GL_TEXTURE0_ARB);

		glBegin(GL_QUADS);
		m_color2.useModulated(m_fade * f);
		Vector3 v1 = m_position + Vector3(-m_radius, -m_radius, m_currentHeight);
		Vector3 v2 = m_position + Vector3( m_radius, -m_radius, m_currentHeight);
		Vector3 v3 = m_position + Vector3( m_radius,  m_radius, m_currentHeight);
		Vector3 v4 = m_position + Vector3(-m_radius,  m_radius, m_currentHeight);
		glTexCoord2f(0, 0);
		glVertex3fv((float *)&v1);
		glTexCoord2f(1, 0);
		glVertex3fv((float *)&v2);
		glTexCoord2f(1, 1);
		glVertex3fv((float *)&v3);
		glTexCoord2f(0, 1);
		glVertex3fv((float *)&v4);
		glEnd();
	}
	else if (m_type == TYPE_SYMBOL)
	{
		Matrix rot = Matrix::rotation(0, 0, m_rotation);

		glEnable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glDepthMask(0);

		float fadein = min(1.0f, (1.0f - m_life) * 3.0f);
		float alphafadeout = min(m_life * 5.0f, 1.0f);

		int drawcount = (int)(m_symbolCount * fadein);
		float frac = (m_symbolCount * fadein) - drawcount;

		for (int i = 0; i < drawcount; i++)
		{
			if (!m_symbolEnabled[i])
				continue;

			float alphamod = m_symbolFade[i] * (m_blink ? (0.7f * m_beat2) : 1.0f);

			float alphavalue = m_fade * alpha * alphafadeout * alphamod;
			if (i == drawcount - 1)
			{
				alphavalue *= frac;
			}
			const float offsetsize = m_radius * 1.8f;

			Vector3 offset;
			Vector3 v1, v2, v3, v4;

			if (m_upwards)
			{
				offset = Vector3(0, 0, i * offsetsize);// * Matrix::rotation(0, 0, m_rowDirection);
				rot = Matrix::rotation(0, 0, m_symbolRotation[i]);
				v1 = m_position + offset + Vector3(-m_radius, 0.0f, -m_radius) * rot;
				v2 = m_position + offset + Vector3( m_radius, 0.0f, -m_radius) * rot;
				v3 = m_position + offset + Vector3( m_radius, 0.0f,  m_radius) * rot;
				v4 = m_position + offset + Vector3(-m_radius, 0.0f,  m_radius) * rot;

				glDisable(GL_TEXTURE_2D);
				glColor4f(0, 0, 0, alphavalue);

				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glBegin(GL_QUADS);
				glVertex3fv((float *)&v1);
				glVertex3fv((float *)&v2);
				glVertex3fv((float *)&v3);
				glVertex3fv((float *)&v4);
				glEnd();
				glDisable(GL_DEPTH_TEST);
				glDepthMask(0);
                                g_textures.bindTexture(m_symbolNames[i], GL_TEXTURE0_ARB);
				m_color.useWithAlpha(alphavalue);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				glBegin(GL_QUADS);
				glTexCoord2f(1, 0);
				glVertex3fv((float *)&v1);
				glTexCoord2f(0, 0);
				glVertex3fv((float *)&v2);
				glTexCoord2f(0, 1);
				glVertex3fv((float *)&v3);
				glTexCoord2f(1, 1);
				glVertex3fv((float *)&v4);
				glEnd();

				glDisable(GL_TEXTURE_2D);
				m_color.useWithAlpha(alphavalue * 0.3f);
				glBegin(GL_LINE_LOOP);
				glVertex3fv((float *)&v1);
				glVertex3fv((float *)&v2);
				glVertex3fv((float *)&v3);
				glVertex3fv((float *)&v4);
				glEnd();
				glEnable(GL_DEPTH_TEST);
				glDepthMask(1);
			}
			else
			{
				offset = Vector3(i * offsetsize, 0, 0) * Matrix::rotation(0, 0, m_rowDirection);
				v1 = m_position + offset + Vector3(-m_radius, -m_radius, 0.0f) * rot;
				v2 = m_position + offset + Vector3( m_radius, -m_radius, 0.0f) * rot;
				v3 = m_position + offset + Vector3( m_radius,  m_radius, 0.0f) * rot;
				v4 = m_position + offset + Vector3(-m_radius,  m_radius, 0.0f) * rot;
                                g_textures.bindTexture(m_symbolNames[i], GL_TEXTURE0_ARB);

				m_color.useWithAlpha(alphavalue);
				glBegin(GL_QUADS);
				glTexCoord2f(1, 0);
				glVertex3fv((float *)&v1);
				glTexCoord2f(0, 0);
				glVertex3fv((float *)&v2);
				glTexCoord2f(0, 1);
				glVertex3fv((float *)&v3);
				glTexCoord2f(1, 1);
				glVertex3fv((float *)&v4);
				glEnd();
			}
		}

		glDepthMask(1);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
	}
	else if (m_type == TYPE_DASHED || m_type == TYPE_DASHED_UP)
	{
		float a = sinf(m_life*3.141592f);
		m_color.useWithAlpha(a * m_fade * alpha);

		Primitives::dashedLine(m_v1, m_v2, 0.2f);
	}
	else if (m_type == TYPE_EQU)
	{
		float a = sinf(m_life*3.141592f);

//		Color3 tqwer = Color3(1,1,1);
//		tqwer.useWithAlpha(a * m_fade * alpha);
		m_color.useWithAlpha(a * m_fade * alpha);

		const int steps = 50;
		for (int j = 0; j < 2; j++)
		{
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < steps; i++)
			{
				float t = i / (float)steps;
				float a = t * 2 * 3.141592f;

				float r = m_radius - j * 0.1f;
				Vector3 v1 = m_position + Vector3(sinf(a), cosf(a), 0) * r;
				glVertex3fv((float *)&v1);
			}
			glEnd();
		}
/*
		Color3 tqwer = Color3(1,1,1);
		tqwer.useWithAlpha(a * m_fade * alpha);
		glBegin(GL_LINE_STRIP);
		float grow = min(m_life * 3, 1.0f);
		for (float t = 0.0f; t < grow; t += 0.02f)
		{
			int index = (int)(t * 511);
			float s = m_spectrum[index];
			Vector3 d = Math::randVectSphere()*m_spectrum[index];
			d.z = 0.0f;
			Vector3 p = m_curve->getValue(t) + d;
			glVertex3fv((float *)&p);
		}
		glEnd();
*/
		if (m_beat > 0.0f)
		{
			m_color.useWithAlpha(a * m_fade * alpha * m_beat);

			float pos = 1.0f - m_beat;
			const int steps = 50;
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < steps; i++)
			{
				float t = i / (float)steps;
				float a = t * 2 * 3.141592f;

				float r = m_radius + pos * 1.0f;
				Vector3 v1 = m_position + Vector3(sinf(a), cosf(a), 0) * r;
				glVertex3fv((float *)&v1);
			}
			glEnd();
		}
	}
}

void Plant::onBeat()
{
	m_beat = 1.0f;
	if (m_broken)
	{
		for (vector<bool>::iterator it = m_symbolEnabled.begin(); it != m_symbolEnabled.end(); it++)
		{
			(*it) = Math::randFloat() < 0.78f;
		}
	}
}
void Plant::onBeat2()
{
	m_beat2 = 1.0f;
}
void Plant::release()
{
}
bool Plant::isDead()
{
	return m_timer < 0.0f;
}





vector<RainParticle *> Cloud::sm_rain;

static int rainparticlecount = 0;
void Cloud::updateRain()
{
	for (vector<RainParticle*>::iterator it = sm_rain.begin(); it != sm_rain.end(); )
	{
		RainParticle *rp = *it;
		if (!updateRainParticle(rp))
		{
			rainparticlecount++;
			if (rainparticlecount %= 10)
			{
				g_particleTree->addSmokeParticle(rp->m_position, rp->m_direction * -1);
			}

			it = sm_rain.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void Cloud::drawRain()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBegin(GL_LINES);
	for (vector<RainParticle*>::iterator it = sm_rain.begin(); it != sm_rain.end(); it++)
	{
		RainParticle *rp = *it;

		rp->m_color.useModulated(rp->m_life);
		glVertex3fv((float *)&rp->m_v1);
		glVertex3fv((float *)&rp->m_v2);
	}
	glEnd();
}
RainParticle* Cloud::initRainParticle(Cloud *source)
{
	RainParticle *rp = new RainParticle();

	rp->m_timer = g_params.getRange("rainparticletime").getRandomValue();
	rp->m_timerStart = rp->m_timer;
	rp->m_life = g_params.getRange("rainparticlelife").getRandomValue();
	rp->m_color = source->m_glowColor;
	rp->m_length = g_params.getRange("rainparticlelength").getRandomValue();

	float a = Math::randFloat()*2*3.141592f;

	int quad = rand() % source->m_quads.size();
	Quad* q = source->m_quads[quad];

	Vector3 c = (q->v1 + q->v2 + q->v3 + q->v4)*0.25f;

	rp->m_position = c + Vector3(sinf(a), cosf(a), 0) * source->m_size;
	rp->m_position.z = c.z;
	rp->m_direction = Vector3(0, 0, -g_params.getRange("rainfallspeed").getRandomValue());

	rp->m_rotation = Matrix::rotation(g_params.getRange("rainrotation_x").getRandomValue(),
									  g_params.getRange("rainrotation_y").getRandomValue(),
									  g_params.getRange("rainrotation_z").getRandomValue());


	Matrix randrot = Matrix::rotation(Math::randVectSphere() * 2 * 3.141592f);
	rp->m_p1 = Vector3(rp->m_length, 0, 0) * randrot;
	rp->m_p2 = Vector3(-rp->m_length, 0, 0) * randrot;

	rp->m_state = RainParticle::STATE_RAINING;

	return rp;
}
bool Cloud::updateRainParticle(RainParticle *rp)
{
	if (rp->m_state == RainParticle::STATE_RAINING)
	{
		//hit ground
		if (rp->m_position.z < 0.0f)
		{
			rp->m_state = RainParticle::STATE_GROUND;
			rp->m_position.z = 0.0f;
			rp->m_v1.z = 0.0f;
			rp->m_v2.z = 0.0f;

		}
		else
		{
			//falling
			rp->m_life = 1.0f;
			rp->m_position += rp->m_direction;
			rp->m_v1 = rp->m_position + rp->m_p1;
			rp->m_v2 = rp->m_position + rp->m_p2;

			rp->m_p1 *= rp->m_rotation;
			rp->m_p2 *= rp->m_rotation;
		}
	}
	else if (rp->m_state == RainParticle::STATE_GROUND)
	{
		rp->m_timer -= 0.01f;
		if (rp->m_timer <= 0.0f)
			return false;
		rp->m_life = rp->m_timer / rp->m_timerStart;
	}
	else return false; //wtf

	return true;
}


Cloud::Cloud()
{
}

Cloud::~Cloud()
{
	for (vector<Quad*>::iterator it = m_quads.begin(); it != m_quads.end(); it++)
	{
		Quad *q = *it;
		delete q;
	}
}

Cloud::CloudType Cloud::getCloudType()
{
	CloudType type = TYPE_DARK;

	if (Math::randFloat() < 0.1f)
	{
		type = TYPE_SYMBOL;
	}

	if (g_system.event("cloudsturnround").hasPassed())
	{
		type = TYPE_ROUND;
	}
	return type;
}

void Cloud::init(const Vector3& position)
{
	m_quads.clear();
	m_timer = g_params.getRange("cloudlife").getRandomValue();
	m_startTimer = m_timer;
	m_rainTimer = 0.0f;

	m_type = getCloudType();
	m_textureNames.push_back(getTextureName(TYPE_KANJI));

	Vector3 startpos = position;
	m_height = g_params.getRange("cloudheight").getRandomValue();
	m_speed = Vector3(-0.01f, 0, 0) * g_params.getRange("cloudspeed").getRandomValue();

	int quads = (int)g_params.getRange("cloudquads").getRandomValue();
	for (int i = 0; i < quads; i++)
	{
		float a = Math::randFloat() * 2 * 3.141592f;
		m_size = g_params.getRange("cloudsize").getRandomValue();

		Vector3 pos = startpos + Vector3(sinf(a), cosf(a), 0) * g_params.getRange("cloudquadspread").getRandomValue();
		Quad *q = new Quad();
		q->v1 = pos + Vector3(-m_size, -m_size, m_height);
		q->v2 = pos + Vector3( m_size, -m_size, m_height);
		q->v3 = pos + Vector3( m_size,  m_size, m_height);
		q->v4 = pos + Vector3(-m_size,  m_size, m_height);

		float a2 = Math::randFloat() * 2 * 3.141592f;
		q->direction = Vector3(sinf(a), cosf(a), 0) * g_params.getRange("cloudquadspeed").getRandomValue();

		m_quads.push_back(q);
	}

	m_glowColor = getColor(false);//Color3(Math::randFloat(), Math::randFloat(), Math::randFloat());
	m_symbolColor = getColor(false);//Color3(Math::randFloat(), Math::randFloat(), Math::randFloat());

	if (m_type == TYPE_ROUND)
	{
		m_transparency = g_params.getRange("cloudtransparencyround").getRandomValue();
	}
	else
	{
		m_transparency = g_params.getRange("cloudtransparency").getRandomValue();
	}
	m_life = m_timer / m_startTimer;
}

void Cloud::update()
{
	m_timer -= 0.01f;
	m_life = m_timer / m_startTimer;

	for (vector<Quad*>::iterator it = m_quads.begin(); it != m_quads.end(); it++)
	{
		Quad *q = *it;

		Vector3 d = m_speed + q->direction;
		q->v1 += d;
		q->v2 += d;
		q->v3 += d;
		q->v4 += d;
	}

	if (g_system.event("rainstart").hasPassed())
	{
		float value = g_system.event("rainstart").getValue();
		m_rainTimer += 0.01f;
		float RAIN_THRESHOLD = g_params.getFloat("rainthreshold");
		int add = g_params.getInt("rainaddcount");
		while (m_rainTimer > RAIN_THRESHOLD)
		{
			if (Math::randFloat() < value)
			{
				for (int i = 0; i < add; i++)
				{
					sm_rain.push_back(initRainParticle(this));
				}
			}
			m_rainTimer -= RAIN_THRESHOLD;
		}
	}
}

float Cloud::getFade()
{
	const float FADETIME = 0.1f;

	float f = 0.0f;

	if (m_life > 1.0f - FADETIME)
	{
		f = 1.0f - (m_life - (1.0f - FADETIME)) / FADETIME;
	}
	else if (m_life < FADETIME)
	{
		f = m_life / FADETIME;
	}
	else
	{
		f = 1.0f;
	}
	return powf(sinf(f*3.141592f*0.5f), 1.5f);
	//return min(1.0f, sinf(m_life*3.141592f) * g_params.getFloat("cloudfade"));
}

static float roundcloudadd = 0.1f;
static bool cloudinit = false;


void Cloud::draw(float alpha, bool useTransparency)
{
	if (g_system.getRandomized())
	{
		if (!cloudinit)
		{
			cloudinit = true;
			float r = Math::randFloat();
			if (r < 0.4f)
			{
				int divisor = 3 + rand() % 4;
				roundcloudadd = (2*3.141592f)/divisor;
			}
			else
			{
				roundcloudadd = 0.1f;
			}
		}
	}
	else
	{
		roundcloudadd = 0.1f;
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	float fade = getFade();

	if (useTransparency)
	{
		const float dim = 0.02f;
		Color3 tempColor = m_glowColor;
		tempColor.r = dim;
		tempColor.g *= dim;
		tempColor.b *= dim;
		glColor4f(tempColor.r, tempColor.g, tempColor.b, alpha * fade * m_transparency);
	}
	else
	{
		glColor4f(0, 0, 0, alpha * fade);
	}

//	glBegin(GL_QUADS);


	switch(m_type)
	{
		case TYPE_DARK:
		{
			glBegin(GL_QUADS);
			for (vector<Quad*>::iterator it = m_quads.begin(); it != m_quads.end(); it++)
			{
				Quad *q = *it;
				glVertex3fv((float *)&q->v1);
				glVertex3fv((float *)&q->v2);
				glVertex3fv((float *)&q->v3);
				glVertex3fv((float *)&q->v4);
			}
			glEnd();

		} break;

		case TYPE_SYMBOL:
		{
			if (useTransparency)
			{
				glBegin(GL_QUADS);
				for (vector<Quad*>::iterator it = m_quads.begin(); it != m_quads.end(); it++)
				{
					Quad *q = *it;
					glVertex3fv((float *)&q->v1);
					glVertex3fv((float *)&q->v2);
					glVertex3fv((float *)&q->v3);
					glVertex3fv((float *)&q->v4);
				}
				glEnd();
			}
			else
			{
				glEnable(GL_TEXTURE_2D);
                                g_textures.bindTexture(m_textureNames[0], GL_TEXTURE0_ARB);
				m_glowColor.useWithAlpha(alpha * fade);

				glBegin(GL_QUADS);
				for (vector<Quad*>::iterator it = m_quads.begin(); it != m_quads.end(); it++)
				{
					Quad *q = *it;
					glTexCoord2f(0, 0);
					glVertex3fv((float *)&q->v1);
					glTexCoord2f(1, 0);
					glVertex3fv((float *)&q->v2);
					glTexCoord2f(1, 1);
					glVertex3fv((float *)&q->v3);
					glTexCoord2f(0, 1);
					glVertex3fv((float *)&q->v4);
				}
				glEnd();
			}

		} break;

		case TYPE_ROUND:
		{
			for (vector<Quad*>::iterator it = m_quads.begin(); it != m_quads.end(); it++)
			{
				Quad *q = *it;
				Vector3 v = q->v1 + q->v2 +	q->v3 + q->v4;

				v *= 1 / 4.0f;

				glBegin(GL_TRIANGLE_FAN);
				float r = m_size * fade;
				for (float y = 0.0f; y < 2*3.141592f; y += roundcloudadd)
				{
					glVertex3f(v.x + sinf(y) * r, v.y + cosf(y) * r, v.z);
				}
				glEnd();
			}

		} break;

	}
}
void Cloud::drawShadow(float alpha)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	float fade = getFade();
	glColor4f(0, 0, 0, alpha * fade * g_params.getFloat("cloudshadow"));

	switch(m_type)
	{
		case TYPE_DARK:
		case TYPE_SYMBOL:
		{
			glBegin(GL_QUADS);
			for (vector<Quad*>::iterator it = m_quads.begin(); it != m_quads.end(); it++)
			{
				Quad *q = *it;
				Vector3 v1 = q->v1;
				Vector3 v2 = q->v2;
				Vector3 v3 = q->v3;
				Vector3 v4 = q->v4;

				v1.z = 0.0f;
				v2.z = 0.0f;
				v3.z = 0.0f;
				v4.z = 0.0f;

				glVertex3fv((float *)&v1);
				glVertex3fv((float *)&v2);
				glVertex3fv((float *)&v3);
				glVertex3fv((float *)&v4);
			}
			glEnd();

		} break;

		case TYPE_ROUND:
		{
			for (vector<Quad*>::iterator it = m_quads.begin(); it != m_quads.end(); it++)
			{
				Quad *q = *it;
				Vector3 v = q->v1 + q->v2 +	q->v3 + q->v4;

				v *= 1 / 4.0f;

				glBegin(GL_TRIANGLE_FAN);
				float r = m_size * fade;
				for (float y = 0.0f; y < 2*3.141592f; y += roundcloudadd)
				{
					glVertex3f(v.x + sinf(y) * r, v.y + cosf(y) * r, 0.0f);
				}
				glEnd();
			}
		} break;
	}
}
void Cloud::drawGlow(float alpha)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(4.0f);

	float fade = getFade();
	m_glowColor.useModulated(alpha * fade);

	switch(m_type)
	{
		case TYPE_DARK:
		{
			for (vector<Quad*>::iterator it = m_quads.begin(); it != m_quads.end(); it++)
			{
				Quad *q = *it;
				glBegin(GL_LINE_LOOP);
				glVertex3fv((float *)&q->v1);
				glVertex3fv((float *)&q->v2);
				glVertex3fv((float *)&q->v3);
				glVertex3fv((float *)&q->v4);
				glEnd();
			}
		} break;

		case TYPE_SYMBOL:
		{
			glDisable(GL_TEXTURE_2D);
			for (vector<Quad*>::iterator it = m_quads.begin(); it != m_quads.end(); it++)
			{
				Quad *q = *it;
				glBegin(GL_LINE_LOOP);
				glVertex3fv((float *)&q->v1);
				glVertex3fv((float *)&q->v2);
				glVertex3fv((float *)&q->v3);
				glVertex3fv((float *)&q->v4);
				glEnd();
			}

		} break;

		case TYPE_ROUND:
		{
			for (vector<Quad*>::iterator it = m_quads.begin(); it != m_quads.end(); it++)
			{
				Quad *q = *it;
				Vector3 v = q->v1 + q->v2 +	q->v3 + q->v4;
				v *= 1 / 4.0f;
				glBegin(GL_LINE_LOOP);
				float r = m_size * fade;
				for (float y = 0.0f; y < 2*3.141592f; y += roundcloudadd)
				{
					glVertex3f(v.x + sinf(y) * r, v.y + cosf(y) * r, v.z);
				}
				glEnd();
			}
		} break;

	}

	glDisable(GL_LINE_SMOOTH);
}
void Cloud::release()
{
}
bool Cloud::isDead()
{
	return m_timer < 0.0f;
}


 void TreeScene::init()
{
	g_params.useNamespace("tree");
	m_debug = g_params.getBool("debug");
	m_timer = 0.0f;

	m_trees.clear();
	int count = g_params.getInt("treecount");
	if (g_system.getRandomized())
	{
		count = (int)Math::randBetween(2, 5);
	}
	for (int i = 0; i < count; i++)
	{
		Tree *tree = new Tree();
		tree->init();
		m_trees.push_back(tree);
	}

	m_cursor = Vector3(0, 0, 0);
	m_angle = 0.0f;
	m_cameraRotation = 0.0f;
	m_cameraZoom = 0.0f;

	m_cameraTarget = Vector3(0, 0, 0);
	m_cameraTarget2 = Vector3(0, 0, 0);
	m_cursorDirection = Vector3(1.0f, 0, 0);

	m_plantTimer = 0.0f;
	m_plants.clear();
	m_buildings.clear();

	Plant::initStatic();

	m_clouds.clear();
	m_cloudTimer = 0;
	Cloud::sm_rain.clear();

	m_wind.clear();
	m_windTimer = 0.0f;

	if (g_system.getRandomized())
	{
#ifdef _WIN32
		srand(timeGetTime());
#else
                srand(time(NULL));
#endif
		m_angleTurnVelocity = 0.002f * Math::randBetween(-3.0f, 3.0f);
		m_angleTurnSpeed = 200.0f * Math::randBetween(-3.0f, 3.0f);
		m_travelSpeedMod = Math::randBetween(0.4f, 1.2f);
		m_cameraRotationSpeed = Math::randBetween(-0.5f, 0.5f);
		m_angleSpeed = Math::randBetween(-0.006f, 0.006f);
		m_animationSpeed = Math::randBetween(0.003f, 0.03f);
		m_movementSpeed = Math::randBetween(0.005f, 0.05f);
		m_travelSpeed = g_params.getRange("travelspeed").getMinimum() * Math::randBetween(0.5f, 1.0f);
		m_globalZoomAmount = g_params.getFloat("cameraglobalzoomamount") * Math::randBetween(0.5f, 2.0f);
		m_cameraDistance = g_params.getFloat("cameradistance") * Math::randBetween(0.5f, 1.5f);
	}
	else
	{
		srand(g_params.getInt("randomseed"));
		m_angleTurnVelocity = 0.002f;
		m_angleTurnSpeed = 200.0f;
		m_travelSpeedMod = 1.0f;
		m_cameraRotationSpeed = 0.2f;
		m_angleSpeed = g_params.getFloat("anglespeed");
		m_animationSpeed = g_params.getFloat("animationspeed");
		m_movementSpeed = g_params.getFloat("movementspeed");
		m_travelSpeed = g_params.getRange("travelspeed").getMinimum();
		m_globalZoomAmount = g_params.getFloat("cameraglobalzoomamount");
		m_cameraDistance = g_params.getFloat("cameradistance");//
	}
}
void TreeScene::release()
{
}

struct cloudComparison
{
	bool operator() (Cloud *c1, Cloud *c2) { return c1->m_height < c2->m_height; }
};
struct plantComparison
{
	bool operator() (Plant *p1, Plant *p2)
	{
		return p2->m_type == Plant::TYPE_TOWER;
	}
};

void TreeScene::updatePlants(vector<Plant*> &plants)
{
	for (vector<Plant*>::iterator it = plants.begin(); it != plants.end(); )
	{
		Plant* p = *it;
		p->update();

		if (p->isDead())
		{
			if (p->m_type == Plant::TYPE_DASHED)
			{
				g_particleTree->addFireParticle(p->m_position);
				g_particleTree->addFireParticle(p->m_position2);
				g_particleTree->addFireParticle(p->m_position);
				g_particleTree->addFireParticle(p->m_position2);

			}
			delete p;
			it = plants.erase(it);
		}
		else
		{
			it++;
		}
	}}

void TreeScene::update()
{
        g_params.useNamespace("tree");

	Vector3 tgt = Vector3(0, 0, 0);
	for (vector<Tree*>::iterator it = m_trees.begin(); it != m_trees.end(); it++)
	{
		Tree* t = *it;
		t->update();
		tgt += t->getCameraPoint();
	}

	int plantcount = 0;
	g_particleTree = m_trees[0]; //...
	updatePlants(m_plants);
	updatePlants(m_buildings);

	Plant::sm_plants = &m_plants; //C++ OOP ftw! :D

	for (vector<Cloud*>::iterator it = m_clouds.begin(); it != m_clouds.end(); )
	{
		Cloud* c = *it;
		c->update();

		if (c->isDead())
		{
			if (c->m_type == Cloud::TYPE_DARK || c->m_type == Cloud::TYPE_SYMBOL)
			{
				vector<Quad*>::iterator it2;
				for (it2 = c->m_quads.begin(); it2 != c->m_quads.end(); it2++)
				{
					Quad* q = *it2;
					g_particleTree->addSmokeParticle(q->v1, c->m_speed);
					g_particleTree->addSmokeParticle(q->v2, c->m_speed);
					g_particleTree->addSmokeParticle(q->v3, c->m_speed);
					g_particleTree->addSmokeParticle(q->v4, c->m_speed);
				}
			}
			delete c;
			it = m_clouds.erase(it);
		}
		else
		{
			it++;
		}
	}

	sort(m_clouds.begin(), m_clouds.end(), cloudComparison());
/*
	g_debug << "---";
	int count = 0;
	for (vector<Cloud*>::iterator it = m_clouds.begin(); it != m_clouds.end(); it++)
	{
		g_debug << "cloud " << count << " height = " << (*it)->m_height << "\n";
		count++;
	}
*/
	Cloud::updateRain();

	if (g_system.event("beatcircles").hasPassed())
	{
		static int lastBeat = 0;
		static int lastBeatCount = 0;
		int count = g_system.beat("basicbeat1").getBeatcount();

		if (count > lastBeatCount)
		{
			lastBeatCount = count;
			int time = g_system.getTime();
			lastBeat = time;
			for (vector<Plant*>::iterator it = m_plants.begin(); it != m_plants.end(); it++)
			{
				Plant* p = *it;
				p->onBeat();
			}
			if (!g_system.event("circuitprocreationstop").hasPassed())
			{
				for (vector<Tree*>::iterator it = m_trees.begin(); it != m_trees.end(); it++)
				{
					Tree *t = *it;
					t->onBeat();
				}
			}
		}




		static int lastBeat2 = 0;
		static int lastBeatCount2 = 0;
		int count2 = g_system.beat("basicbeat0").getBeatcount();

		if (count2 > lastBeatCount2)
		{
			lastBeatCount2 = count2;
			int time = g_system.getTime();
			lastBeat2 = time;
			for (vector<Plant*>::iterator it = m_plants.begin(); it != m_plants.end(); it++)
			{
				Plant* p = *it;
				p->onBeat2();
			}
		}
	}

	tgt *= 1.0f / m_trees.size();

	m_timer += m_animationSpeed;//g_params.getFloat("animationspeed");
	m_speed = m_movementSpeed;//g_params.getFloat("movementspeed");
	m_angle += m_angleSpeed + m_angleTurnVelocity * sinf(m_pos * m_angleTurnSpeed);

	float travelspeed = g_params.getRange("travelspeed").getMinimum();
	m_cursor.x += sinf(m_angle) * m_speed;
	m_cursor.y += cosf(m_angle) * m_speed;
	m_cursor += m_cursorDirection * m_travelSpeed * m_travelSpeedMod;//g_params.getFloat("travelspeed");

	Tree::sm_cursor = m_cursor;

	m_cameraPosition = m_cursor;
	float d1 = m_cameraDistance;//g_params.getFloat("cameradistance");
	float d2 = g_params.getFloat("cameradistancebeforezoom");
	float dist =  d1 + (d2 - d1) * (1.0f - powf(sinf(g_system.event("camerazoominstart").getValue()*3.141592f*0.5f), 2.0f));
	m_cameraPosition.z = dist + g_params.getFloat("camerazoomamount")*sinf(m_cameraZoom);
	m_cameraPosition.z += m_globalZoomAmount * sinf(g_params.getFloat("cameraglobalzoomspeed") * g_system.getTime());

	m_cameraRotation = m_angle * m_cameraRotationSpeed;
	m_cameraZoom += g_params.getFloat("camerazoomspeed");
	m_cameraTarget = m_cameraTarget * 0.99f + tgt * 0.01f;//m_cursor;

	if (g_system.event("plantfadein").hasPassed())
	{
		g_system.getWaveData(Plant::m_spectrum, 512);

		m_plantTimer += 0.01f;
		const float PLANT_THRESHOLD = g_params.getFloat("plantthreshold");

		while (m_plantTimer > PLANT_THRESHOLD)
		{
			if (!g_system.event("plantfadeout").hasPassed())
			{
				Plant *plant = createNewPlant(Plant::TYPE_NONE);
				if (plant->m_type == Plant::TYPE_TOWER)
				{
					m_buildings.push_back(plant);
					if (Math::randFloat() < 0.35f)
					{
						m_buildings.push_back(createNewPlant(Plant::TYPE_TOWER));
					}
				}
				else
					m_plants.push_back(plant);

				//dashed lines are nice
				for (int j = 0; j < 2; j++)
				{
					m_plants.push_back(createNewPlant(Plant::TYPE_DASHED));
				}

				//add extra dashed lines
				if (g_system.event("dashedplantfadein").hasPassed())
				{
					for (int j = 0; j < 5; j++)
					{
						m_plants.push_back(createNewPlant(Plant::TYPE_DASHED));
					}
				}
			}

			if (g_system.event("plantstartagain").hasPassed())
			{
				if (Math::randFloat() < 0.5f && !g_system.event("towerfadeout").hasPassed())
				{
					m_buildings.push_back(createNewPlant(Plant::TYPE_TOWER));
				}
				else
				{
					m_plants.push_back(createNewPlant(Plant::TYPE_SYMBOL));
				}
			}
			m_plantTimer -= PLANT_THRESHOLD;
		}


		if (g_system.event("cloudscomein").hasPassed())
		{
			m_cloudTimer += 0.01f;

			const float CLOUD_THRESHOLD = g_system.event("cloudsturnround").hasPassed() ? g_params.getFloat("cloudthreshold2") :
				g_params.getFloat("cloudthreshold");

			bool inCloudStop = g_system.event("cloudstopstart").hasPassed() && !g_system.event("cloudstopend").hasPassed();
			bool initCloud = (!g_system.event("cloudsgoout").hasPassed()) && !inCloudStop;

			while (m_cloudTimer > CLOUD_THRESHOLD)
			{
				if (initCloud)
				{
					Cloud *c = new Cloud();
					float r = g_params.getRange("plantrange").getRandomValue();
					float a = Math::randFloat()*2*3.141592f;
					Vector3 d = Vector3(sinf(a), cosf(a), 0) * r;

					c->init(m_cursor + d);
					m_clouds.push_back(c);
				}
				m_cloudTimer -= CLOUD_THRESHOLD;

			}
		}
	}

	sort(m_plants.begin(), m_plants.end(), plantComparison());

	if (g_system.event("windstart").hasPassed())
	{
		for (vector<WindParticle*>::iterator it = m_wind.begin(); it != m_wind.end(); )
		{
			WindParticle *wp = *it;
			if (updateWindParticle(wp))
			{
				it++;
			}
			else
			{
				it = m_wind.erase(it);
			}
		}

		m_windTimer += 0.01f;
		const float WIND_THRESHOLD = g_params.getFloat("windthreshold");

		while (m_windTimer > WIND_THRESHOLD)
		{
			if (!g_system.event("windfadeout").hasPassed())
			{
				int addcount = g_params.getInt("windaddcount");
				int maxcount = g_params.getInt("maxwindparticles");

				float r = g_system.event("windstart").getValue();

				if (Math::randFloat() < r)
				{
					if ((int)m_wind.size() < maxcount)
					{
						for (int i = 0; i < addcount; i++)
						{
							m_wind.push_back(initWindParticle());
						}
					}
				}
			}
			m_windTimer -= WIND_THRESHOLD;
		}
	}


	m_lightPosition = m_cameraTarget;

}
Plant *TreeScene::createNewPlant(Plant::PlantType type)
{
	Plant *p = new Plant();
	float r = g_params.getRange("plantrange").getRandomValue();
	float a = Math::randFloat()*2*3.141592f;
	Vector3 d = Vector3(sinf(a), cosf(a), 0) * r;
	p->init(m_cursor + d, type);
	return p;
}

WindParticle *TreeScene::initWindParticle()
{
	WindParticle *wp = new WindParticle();

	wp->m_timer = g_params.getRange("windparticletime").getRandomValue();
	wp->m_timerStart = wp->m_timer;
	wp->m_life = 1.0f;

	wp->m_color = getWindColor();
	wp->m_length = g_params.getRange("windparticlelength").getRandomValue();

	float r = g_params.getRange("windrange").getRandomValue();
	float a = Math::randFloat()*2*3.141592f;
	wp->m_position = m_cursor + Vector3(sinf(a), cosf(a), 0) * r;
	wp->m_position.z = g_params.getRange("windheight").getRandomValue();
	wp->m_direction = g_params.getVector3("winddirection") * g_params.getRange("windspeed").getRandomValue();
	wp->m_direction *= Matrix::rotation(g_params.getRange("winddeviation_x").getRandomValue(),
										g_params.getRange("winddeviation_y").getRandomValue(),
										g_params.getRange("winddeviation_z").getRandomValue());

	wp->m_rotation = Matrix::rotation(g_params.getRange("windrotation_x").getRandomValue(),
									  g_params.getRange("windrotation_y").getRandomValue(),
									  g_params.getRange("windrotation_z").getRandomValue());


	Matrix randrot = Matrix::rotation(Math::randVectSphere() * 2 * 3.141592f);
	wp->m_p1 = Vector3(wp->m_length, 0, 0) * randrot;
	wp->m_p2 = Vector3(-wp->m_length, 0, 0) * randrot;

	return wp;
}

bool TreeScene::updateWindParticle(WindParticle *wp)
{
	wp->m_timer -= 0.01f;
	if (wp->m_timer <= 0.0f)
		return false;

	wp->m_life = wp->m_timer / wp->m_timerStart;
	wp->m_position += wp->m_direction;

	wp->m_v1 = wp->m_position + wp->m_p1;
	wp->m_v2 = wp->m_position + wp->m_p2;

	wp->m_p1 *= wp->m_rotation;
	wp->m_p2 *= wp->m_rotation;

	return true;
}

void TreeScene::drawDebug()
{
	glColor4f(1, 0, 0, 1);

	//cursor
	glBegin(GL_LINES);
	const float CURSOR_SIZE = 0.2f;
	Vector3 cursor_left = m_cursor + Vector3(-CURSOR_SIZE, 0, 0);
	Vector3 cursor_right = m_cursor + Vector3( CURSOR_SIZE, 0, 0);
	Vector3 cursor_up = m_cursor + Vector3(0, -CURSOR_SIZE, 0);
	Vector3 cursor_down = m_cursor + Vector3(0, CURSOR_SIZE, 0);
	glVertex3fv((float *)&cursor_left);
	glVertex3fv((float *)&cursor_right);
	glVertex3fv((float *)&cursor_up);
	glVertex3fv((float *)&cursor_down);
	glEnd();
}

static bool backgroundinit = false;
static bool hascheckers = false;
static bool hasbothcheckers = false;
static bool haswireframe = false;
static bool haskanjicheckers = false;
static bool hasbothkanjicheckers = false;
static int andshift1 = 0;
static int andshift2 = 0;
static string kanjicheckertexture1 = "";
static string kanjicheckertexture2 = "";
static Color3 wireframecolor = Color3(0,0,0);
static Color3 checkercolor1 = Color3(0,0,0);
static Color3 checkercolor2 = Color3(0,0,0);
static Color3 checkercolor3 = Color3(0,0,0);
static Color3 checkercolor4 = Color3(0,0,0);
static float wireframemod = 0.0f;

void TreeScene::drawBackground()
{
	if (!backgroundinit)
	{
		backgroundinit = true;
		if (g_system.getRandomized())
		{
			hascheckers = Math::randFloat() < 0.5f;
			haswireframe = Math::randFloat() < 0.5f;

			if (hascheckers)
			{
				haskanjicheckers = Math::randFloat() < 0.25f;
				kanjicheckertexture1 = getTextureName(TYPE_KANJI);
				kanjicheckertexture2 = getTextureName(TYPE_KANJI);
				hasbothcheckers = Math::randFloat() < 0.5f;
			}
			wireframecolor = Color3(Math::randFloat(), Math::randFloat(), Math::randFloat()).normalize();
			checkercolor1 = Color3(Math::randFloat(), Math::randFloat(), Math::randFloat()).normalize();
			if (Math::randFloat() < 0.5f)
			{
				checkercolor2 = Color3(0, 0, 0).normalize();
			}
			else
			{
				checkercolor2 = Color3(1, 1, 1);//Math::randFloat(), Math::randFloat(), Math::randFloat()).normalize();
			}
			checkercolor3 = Color3(Math::randFloat(), Math::randFloat(), Math::randFloat()).normalize();
			wireframemod = Math::randBetween(0.05f, 0.3f);
			andshift1 = (Math::randFloat() < 0.5f) ? 1 : 0;
			andshift2 = (Math::randFloat() < 0.5f) ? 1 : 0;
		}
		else
		{
			hascheckers = false;
			haswireframe = false;
		}
	}
	float checkeralpha = g_system.event("backgroundfadeout").getValue();
	if (checkeralpha > 0.0001f && hascheckers)
	{
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_LINE_SMOOTH);

		const float size = g_params.getFloat("backgroundgridsize");
		const int STEPS = g_params.getInt("backgroundgridsteps");
		glLineWidth(2.0f);

		float stepsize = size / STEPS;

		float start_y = m_cursor.y - size;
		float dy = fmod(start_y, stepsize);
		start_y -= dy;

		float start_x = m_cursor.x - size;
		float dx = fmod(start_x, stepsize);
		start_x -= dx;

		float end_x = start_x + size * 2;
		float end_y = start_y + size * 2;

		int ysteps = (int)((end_y - start_y) / stepsize) + 1;
		int xsteps = (int)((end_x - start_x) / stepsize) + 1;

		int andvalue = ((int)(start_x) + (int)(start_y))& 1;

		glBegin(GL_QUADS);
		for (int y = 0; y < ysteps; y++)
		{
			float ypos1 = start_y + y * stepsize;
			float ypos2 = start_y + (y + 1) * stepsize;
			for (int x = 0; x < xsteps; x++)
			{
				float xpos1 = start_x + x * stepsize;
				float xpos2 = start_x + (x + 1) * stepsize;

				bool checker = ((x+y) & 1) == andvalue;

				if (checker)
				{
					glColor4f(checkercolor1.r, checkercolor1.g, checkercolor1.b, 0.2f*checkeralpha);
					glVertex3f(xpos1, ypos1, 0);
					glVertex3f(xpos2, ypos1, 0);
					glVertex3f(xpos2, ypos2, 0);
					glVertex3f(xpos1, ypos2, 0);
				}
				else if (hasbothcheckers)
				{
					glColor4f(checkercolor2.r, checkercolor2.g, checkercolor2.b, 0.2f*checkeralpha);
					glVertex3f(xpos1, ypos1, 0);
					glVertex3f(xpos2, ypos1, 0);
					glVertex3f(xpos2, ypos2, 0);
					glVertex3f(xpos1, ypos2, 0);
				}
			}
		}
		glEnd();

		if (haskanjicheckers)
		{
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                        g_textures.bindTexture(kanjicheckertexture1, GL_TEXTURE0_ARB);

			glBegin(GL_QUADS);
			for (int y = 0; y < ysteps; y++)
			{
				float ypos1 = start_y + y * stepsize;
				float ypos2 = start_y + (y + 1) * stepsize;
				for (int x = 0; x < xsteps; x++)
				{
					float xpos1 = start_x + x * stepsize;
					float xpos2 = start_x + (x + 1) * stepsize;

					bool checker = ((x+y) & 1) == ((andvalue + andshift1)&1);
					glColor4f(checkercolor3.r, checkercolor3.g, checkercolor3.b, 0.7f*checkeralpha);

					if (checker)
					{
						glTexCoord2f(0, 1);
						glVertex3f(xpos1, ypos1, 0);
						glTexCoord2f(1, 1);
						glVertex3f(xpos2, ypos1, 0);
						glTexCoord2f(1, 0);
						glVertex3f(xpos2, ypos2, 0);
						glTexCoord2f(0, 0);
						glVertex3f(xpos1, ypos2, 0);
					}
				}
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}

/*

		glBegin(GL_LINES);
		for (float y = start_y; y < end_y; y += stepsize)
		{
			glVertex3f(start_x, y, 0.0f);
			glVertex3f(end_x, y, 0.0f);
		}

		for (float x = start_x; x < end_x; x += stepsize)
		{
			glVertex3f(x, start_y, 0.0f);
			glVertex3f(x, end_y, 0.0f);
		}
		glEnd();
*/
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_BLEND);
	}


	//wireframe part
	float wireframealpha = g_system.event("backgroundfadeinstart").getValue() * (1.0f - g_system.event("backgroundfadeout").getValue());
	if (wireframealpha > 0.0001f && haswireframe)
	{
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_LINE_SMOOTH);

		const float size = g_params.getFloat("backgroundgridsize");
		const int STEPS = g_params.getInt("backgroundgridsteps");
		glLineWidth(2.0f);

		float stepsize = size / STEPS;

		float start_y = m_cursor.y - size;
		start_y -= fmod(start_y, stepsize);

		float start_x = m_cursor.x - size;
		start_x -= fmod(start_x, stepsize);

		float end_x = start_x + size * 2;
		float end_y = start_y + size * 2;

		glBegin(GL_LINES);
		glColor4f(wireframecolor.r, wireframecolor.g, wireframecolor.b, wireframemod*wireframealpha);

		for (float y = start_y; y < end_y; y += stepsize)
		{
			glVertex3f(start_x, y, 0.0f);
			glVertex3f(end_x, y, 0.0f);
		}

		for (float x = start_x; x < end_x; x += stepsize)
		{
			glVertex3f(x, start_y, 0.0f);
			glVertex3f(x, end_y, 0.0f);
		}
		glEnd();
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_BLEND);
	}
	drawShadows();

}

void TreeScene::drawShadows()
{
	for (vector<Cloud*>::iterator it = m_clouds.begin(); it != m_clouds.end(); it++)
	{
		Cloud* c = *it;
		c->drawShadow(1.0f);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(0);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(3.0f);
	glBegin(GL_LINES);
	glColor4f(0, 0, 0, 0.2f);
	for (vector<WindParticle*>::iterator it = m_wind.begin(); it != m_wind.end(); it++)
	{
		WindParticle *wp = *it;

		Vector3 v1 = wp->m_v1;
		Vector3 v2 = wp->m_v2;
		v1.z = 0.0f;
		v2.z = 0.0f;
		glVertex3fv((float *)&v1);
		glVertex3fv((float *)&v2);
	}
	glEnd();
	glDepthMask(1);
	glDisable(GL_LINE_SMOOTH);
}




void TreeScene::drawWind()
{
//	g_debug << "wind particles " << m_wind.size() << "\n";
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBegin(GL_LINES);
	for (vector<WindParticle*>::iterator it = m_wind.begin(); it != m_wind.end(); it++)
	{
		WindParticle *wp = *it;

		wp->m_color.useModulated(wp->m_life);
		glVertex3fv((float *)&wp->m_v1);
		glVertex3fv((float *)&wp->m_v2);
	}
	glEnd();
}



void TreeScene::drawPlants(vector<Plant*> &plants)
{
	float alpha = g_system.event("plantfadein").getValue() * (1.0f - g_system.event("plantgooutforgood").getValue());
	for (vector<Plant*>::iterator it = plants.begin(); it != plants.end(); it++)
	{
		Plant* p = *it;
		p->draw(alpha);
	}
}
void TreeScene::drawPlantsMask(vector<Plant*> &plants)
{
	for (vector<Plant*>::iterator it = plants.begin(); it != plants.end(); it++)
	{
		Plant* p = *it;
		p->drawMask();
	}
}

void TreeScene::drawClouds()
{
	float alpha = 1.0f;
	for (vector<Cloud*>::iterator it = m_clouds.begin(); it != m_clouds.end(); it++)
	{
		Cloud* c = *it;
		c->drawGlow(alpha);
	}
	Cloud::drawRain();
}
void TreeScene::drawCloudsMask(bool useTransparency)
{
	float alpha = 1.0f;
	for (vector<Cloud*>::iterator it = m_clouds.begin(); it != m_clouds.end(); it++)
	{
		Cloud* c = *it;
		c->draw(alpha, useTransparency);
	}
}

void TreeScene::drawTree(bool glow)
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_LINE_SMOOTH);

	float treealpha = g_system.event("treefadein").getValue() * (1.0f - g_system.event("treefadeout").getValue());
	float circuitalpha = g_system.event("circuitfadein").getValue() * (1.0f - g_system.event("circuitfadeout").getValue());
	for (vector<Tree*>::iterator it = m_trees.begin(); it != m_trees.end(); it++)
	{
		Tree* t = *it;
		t->draw(treealpha, circuitalpha, 1.0f, glow);
	}
	glDisable(GL_BLEND);
}

static int caminit = false;
static float camerafocus = 1.0f;
static int cameramode = 0;
static Vector3 baseUp = Vector3(0, 1, 0);

void TreeScene::setCam()
{
	if (!caminit)
	{
		caminit = true;
		if (g_system.getRandomized())
		{
			camerafocus = Math::randBetween(0.5f, 1.0f);
			if (Math::randFloat() < 0.86f)
			{
				cameramode = 0;
			}
			else
			{
				cameramode = 1;
			}
			if (Math::randFloat() < 0.8f)
			{
				baseUp = Vector3(0, 1, 0);
			}
			else
			{
				baseUp = Vector3(Math::randBetween(-0.001f, 0.001f), 1.0f, Math::randBetween(-0.001f, 0.001f)).normalize();
			}
		}
		else
		{
			cameramode = 0;
			baseUp = Vector3(0, 1, 0);
		}
	}

	Vector3 cameraUp = baseUp * Matrix::rotation(0, 0, m_cameraRotation);

	switch(cameramode)
	{
		case 0: //default mode
		{
			gluLookAt(m_cameraPosition.x, m_cameraPosition.y, m_cameraPosition.z,
					  m_cameraTarget.x, m_cameraTarget.y, m_cameraTarget.z,
					  cameraUp.x, cameraUp.y, cameraUp.z);

		} break;
		case 1:
		{
			gluLookAt(m_cameraPosition.x,
					  m_cameraPosition.y,
					  m_cameraPosition.z,
					  m_cameraPosition.x + (m_cameraTarget.x - m_cameraPosition.x) * camerafocus,
					  m_cameraPosition.y + (m_cameraTarget.y - m_cameraPosition.y) * camerafocus,
					  m_cameraPosition.z + (m_cameraTarget.z - m_cameraPosition.z) * camerafocus,
					  cameraUp.x, cameraUp.y, cameraUp.z);

		} break;
	}
}
void TreeScene::draw()
{
        g_params.useNamespace("tree");

	glLoadIdentity();
	setCam();

	drawBackground();
	if (g_system.getRandomized())
	{
		drawPlantsMask(m_buildings);
	}

	g_postprocess.init(RENDERTARGET_0);
	drawTree(true);
	drawPlantsMask(m_buildings);
	g_postprocess.glow(RENDERTARGET_0, g_params.getInt("treeglowcount"), g_params.getFloat("treeglow_x"), g_params.getFloat("treeglow_y"),
						g_params.getFloat("treeglow_alpha"), g_params.getFloat("treeglow_exp"), true);
	setCam();
	g_postprocess.init(RENDERTARGET_0);
	drawWind();
	drawPlants(m_plants);
	drawPlants(m_buildings);
	float s = sinf(g_system.triggers("sync").getValue()*3.141592f) * 0.04f;
	g_postprocess.glow(RENDERTARGET_0, g_params.getInt("glowcount"), g_params.getFloat("glow_x") + s, g_params.getFloat("glow_y") + s,
		g_params.getFloat("glow_alpha") + s,
		g_params.getFloat("glow_exp"), true);

	//savu ja maskit
	setCam();
	drawTree(false);
	drawCloudsMask(true); //musta pilven pohja
	g_postprocess.init(RENDERTARGET_0);

	//pilvet
	drawClouds();		   //hohtavat reunat
	drawCloudsMask(false); //ylimääräiset reunat pois

	if (g_system.event("cloudstopend").hasPassed())
	{
		g_postprocess.glow(RENDERTARGET_0, g_params.getInt("cloudglowcount2"),
										   g_params.getFloat("cloudglow_x2"),
										   g_params.getFloat("cloudglow_y2"),
										   g_params.getFloat("cloudglow_alpha2"),
										   g_params.getFloat("cloudglow_exp2"), true);
	}
	else
	{
		g_postprocess.glow(RENDERTARGET_0, g_params.getInt("cloudglowcount"),
										   g_params.getFloat("cloudglow_x"),
										   g_params.getFloat("cloudglow_y"),
										   g_params.getFloat("cloudglow_alpha"),
										   g_params.getFloat("cloudglow_exp"), true);
	}
	float brsalpha = g_system.event("brsfadein").getValue() * (1.0f - g_system.event("brsfadeout").getValue());
	if (brsalpha> 0.001f)
	{
		glColor4f(1,1,1,brsalpha);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		Primitives::picture2D("brslogo.png", 0.5f, 0.5f, 0.40f - (1.0f - brsalpha)*0.015f);
	}
	float titlealpha1 = g_system.event("titlefadein1").getValue() * (1.0f - g_system.event("titlefadeout1").getValue());
	float titlealpha2 = g_system.event("titlefadein2").getValue() * (1.0f - g_system.event("titlefadeout2").getValue());
	float titlealpha3 = g_system.event("titlefadein3").getValue() * (1.0f - g_system.event("titlefadeout3").getValue());
	float titlealpha4 = g_system.event("titlefadein4").getValue() * (1.0f - g_system.event("titlefadeout4").getValue());
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	if (titlealpha1 > 0.001f)
	{
		glColor4f(1,1,1,titlealpha1);
		const float SIZE = 0.12f + 0.03f * powf(sinf(titlealpha1*3.141592f*0.5f), 2.0f);
		Primitives::picture2D("2.png", 0.2f, 0.5f, SIZE);
	}
	if (titlealpha2 > 0.001f)
	{
		glColor4f(1,1,1,titlealpha2);
		const float SIZE = 0.12f + 0.03f * powf(sinf(titlealpha2*3.141592f*0.5f), 2.0f);
		Primitives::picture2D("0.png", 0.4f, 0.5f, SIZE);
	}
	if (titlealpha3 > 0.001f)
	{
		glColor4f(1,1,1,titlealpha3);
		const float SIZE = 0.12f + 0.03f * powf(sinf(titlealpha3*3.141592f*0.5f), 2.0f);
		Primitives::picture2D("1.png", 0.6f, 0.5f, SIZE);
	}
	if (titlealpha4 > 0.001f)
	{
		glColor4f(1,1,1,titlealpha4);
		const float SIZE = 0.12f + 0.03f * powf(sinf(titlealpha4*3.141592f*0.5f), 2.0f);
		Primitives::picture2D("9.png", 0.8f, 0.5f, SIZE);
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Primitives::fullscreenFade(Color4(0, 0, 0, g_system.event("globalfadeout").getValue()));

	float creditsalpha = g_system.event("creditsfadein").getValue() * (1.0f - g_system.event("creditsfadeout").getValue());

	if (creditsalpha> 0.001f)
	{
		glColor4f(1,1,1,creditsalpha);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		Primitives::picture2D("credits.png", 0.5f, 0.5f, 0.30f + g_system.event("creditszoom").getValue()*0.19f);
	}
}

