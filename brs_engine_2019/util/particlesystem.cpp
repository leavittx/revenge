#include "particlesystem.h"

/*
BasicParticleSystem::BasicParticleSystem()
{
	m_gravity = Vector3(0, 0, 0);
	m_particles.clear();
	m_texture = "";
}
BasicParticleSystem::~BasicParticleSystem()
{
}

void BasicParticleSystem::init()
{
	m_particles.clear();
}

int BasicParticleSystem::getParticleCount()
{
	return m_particles.size();
}
vector<BasicParticle>* BasicParticleSystem::getParticles()
{
	return &m_particles;
}

void BasicParticleSystem::addParticle(Vector3& pos, 
									  Vector3& speed, 
									  Color3& color, 
									  float size, 
									  float energy, 
									  float fade, 
									  float energydecrease)
{
	BasicParticle p;
	p.alive = true;
	p.energy = energy;
	p.maxenergy = energy;
	p.energydecrease = energydecrease;
	p.pos = pos;
	p.speed = speed;
	p.color = color;
	p.size = size;
	p.fade = fade;

	m_particles.push_back(p);
}

void BasicParticleSystem::draw()
{
	Vector3 v1, v2, v3, v4;
	g_textures.bindTexture(m_texture, GL_TEXTURE0_ARB);
	glEnable(GL_BLEND);
	glBlendFunc(m_sourceBlend, m_destBlend);
	glBegin(GL_QUADS);

	for (vector<BasicParticle>::iterator it = m_particles.begin(); it < m_particles.end(); it++)
	{
		BasicParticle &p = *it;
		if (p.alive)
		{
			ParticleUtils::project(p.pos, p.size, v1, v2, v3, v4);
			float f = p.fade * (p.energy / p.maxenergy);
			p.color.useWithAlpha(f);
			glTexCoord2f(0, 0);
			glVertex3fv((float *)&v1);
			glTexCoord2f(1, 0);
			glVertex3fv((float *)&v2);
			glTexCoord2f(1, 1);
			glVertex3fv((float *)&v3);
			glTexCoord2f(0, 1);
			glVertex3fv((float *)&v4);
		}
		
	}
	glEnd();
}

void BasicParticleSystem::update()
{
	for (vector<BasicParticle>::iterator it = m_particles.begin(); it < m_particles.end(); )
	{
		BasicParticle& p = *it;
		if (p.alive)
		{
			p.pos += p.speed;
			p.speed += m_gravity;
			p.energy -= p.energydecrease;
			if (p.energy < 0)
			{
				p.alive = false;
			}
			it++;
		}
		else
		{
			it = m_particles.erase(it);
		}
	}
}

void BasicParticleSystem::setBlendMode(GLuint source, GLuint dest)
{
	m_sourceBlend = source;
	m_destBlend = dest;
}
void BasicParticleSystem::setGravity(float gravity)
{
	m_gravity = Vector3(0, gravity, 0);
}
void BasicParticleSystem::setGravity(Vector3& gravity)
{
	m_gravity = gravity;
}
void BasicParticleSystem::setTexture(string texture)
{
	m_texture = texture;
}




ParticleSmoke::ParticleSmoke()
{
}
ParticleSmoke::~ParticleSmoke()
{
}

void ParticleSmoke::update()
{
	m_alive = false;

	for (int i = 0; i < m_count; i++)
	{
		SmokeParticle& s = m_particles[i];
		if (s.alive)
		{
			m_alive = true; //whole particle system still alive
			s.energy -= 0.01f;
			s.pos += s.speed;
			if (s.pos.y < 0)
				s.pos.y = 0;

			if (s.energy < 0.0f)
				s.alive = false;
		}
		else
		{
			//do nothing
		}
	}
}

void ParticleSmoke::draw()
{
	g_textures.bindTexture(m_textureName, GL_TEXTURE0_ARB);

	if (m_type == PARTICLE_SMOKE_TYPE_NORMAL)
	{
		glBegin(GL_QUADS);
		for (int i = 0; i < m_count; i++)
		{
			SmokeParticle &p = m_particles[i];
			if (p.alive)
			{
				Vector3 v1, v2, v3, v4;
				ParticleUtils::project(p.pos, p.size, v1, v2, v3, v4);
				float f = p.fade * sinf((p.energy / p.maxenergy)*3.141592f*0.5f);
				p.color.useWithAlpha(f);
				glTexCoord2f(0, 0);
				glVertex3fv((float *)&v1);
				glTexCoord2f(1, 0);
				glVertex3fv((float *)&v2);
				glTexCoord2f(1, 1);
				glVertex3fv((float *)&v3);
				glTexCoord2f(0, 1);
				glVertex3fv((float *)&v4);
			}
		}
		glEnd();
	}
	else if (m_type == PARTICLE_SMOKE_TYPE_FADE)
	{
		glBegin(GL_QUADS);
		for (int i = 0; i < m_count; i++)
		{
			SmokeParticle &p = m_particles[i];
			if (p.alive)
			{
				Vector3 v1, v2, v3, v4;
				ParticleUtils::project(p.pos, p.size, v1, v2, v3, v4);
				float f = p.fade * sinf((p.energy / p.maxenergy)*3.141592f*1.0f);
				p.color.useWithAlpha(f);
				glTexCoord2f(0, 0);
				glVertex3fv((float *)&v1);
				glTexCoord2f(1, 0);
				glVertex3fv((float *)&v2);
				glTexCoord2f(1, 1);
				glVertex3fv((float *)&v3);
				glTexCoord2f(0, 1);
				glVertex3fv((float *)&v4);
			}
		}
		glEnd();
	}

}
bool ParticleSmoke::isAlive()
{
	return m_alive;
}
void ParticleSmoke::init(Vector3& position, 
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
						 ParticleSmokeSystemType type)
{

	m_type = type;
	m_alive = true;
	m_textureName = texture;
	m_count = mincount + rand()%(maxcount - mincount);
	m_particles = new SmokeParticle[m_count];
	
	for (int i = 0; i < m_count; i++)
	{
		SmokeParticle s;
		float e = Math::randBetween(minenergy, maxenergy);
		float fade = Math::randBetween(minfade, maxfade);
		float size = Math::randBetween(minsize, maxsize);
		s.energy = e;
		s.maxenergy = e;
		s.size = size;
		s.fade = fade;
		s.color = mincolor + (maxcolor - mincolor) * Math::randFloat(1.0f);

		Vector3 direction = Math::randVectSphere(Math::randBetween(0, spread));

		s.pos = position + direction;
		s.speed = direction.normalize() * Math::randBetween(minspeed, maxspeed);
		s.alive = true;
		m_particles[i] = s;
	}
}


ParticleSmokeSystem::ParticleSmokeSystem()
{
	m_smokes.clear();
}
ParticleSmokeSystem::~ParticleSmokeSystem()
{
}

int ParticleSmokeSystem::getCount()
{
	return m_smokes.size();
}

void ParticleSmokeSystem::draw()
{
	vector<ParticleSmoke*>::iterator it;
	for (it = m_smokes.begin(); it < m_smokes.end(); it++)
	{
		ParticleSmoke *s = *it;
		s->draw();
	}
}
void ParticleSmokeSystem::update()
{
	vector<ParticleSmoke*>::iterator it;
	for (it = m_smokes.begin(); it < m_smokes.end(); )
	{
		ParticleSmoke *s = *it;
		if (s->isAlive())
		{
			s->update();
			it++;
		}
		else
		{
			delete s;
			it = m_smokes.erase(it);
		}
	}
}
void ParticleSmokeSystem::init(Vector3& position, 
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
							   ParticleSmokeSystemType type)
{
	ParticleSmoke *s = new ParticleSmoke();
	//uglyyy.y..y.y .y. .. 
	s->init(position, 
			texture, 
			mincolor, 
			maxcolor, 
			mincount, 
			maxcount, 
			minsize, 
			maxsize, 
			minenergy, 
			maxenergy, 
			minfade, 
			maxfade, 
			spread, 
			minspeed, 
			maxspeed,
			type);
	m_smokes.push_back(s);
}



ParticleWind::ParticleWind()
{
	m_count = 0;
	m_addCount = 0;
	m_index = 0;

	m_minSize = 0.0f;
	m_maxSize = 0.0f;
	m_minEnergy = 0.0f;
	m_maxEnergy = 0.0f;

	m_particles = 0;
}
ParticleWind::~ParticleWind()
{
}

void ParticleWind::draw(float alphamod)
{
	ParticleUtils::update();
	glBegin(GL_QUADS);
	for (int i = 0; i < m_count; i++)
	{
		WindParticle& p = m_particles[i];
		if (p.active)
		{
			Vector3 v1, v2, v3, v4;
			ParticleUtils::project(p.position, p.size * sinf((p.energy / p.maxenergy) * 3.141592f) , v1, v2, v3, v4);
			p.color.useWithAlpha(p.fade * alphamod);
			glTexCoord2f(0, 0);
			glVertex3fv((float *)&v1);
			glTexCoord2f(1, 0);
			glVertex3fv((float *)&v2);
			glTexCoord2f(1, 1);
			glVertex3fv((float *)&v3);
			glTexCoord2f(0, 1);
			glVertex3fv((float *)&v4);
		}
	}
	glEnd();
}
void ParticleWind::update()
{
	for (int i = 0; i < m_count; i++)
	{
		WindParticle& p = m_particles[i];
		if (p.active)
		{
			p.position += p.direction * 0.01f;
			p.energy -= 0.001f;
			if (p.energy <= 0.000f)
			{
				p.active = false; //dead
			}
		}
	}
	for (int i = 0; i < m_addCount; i++)
	{
		if (!m_particles[m_index].active)
		{
			//init new particle

			WindParticle& p = m_particles[m_index];
			p.size = Math::randBetween(m_minSize, m_maxSize);
			p.energy = Math::randBetween(m_minEnergy, m_maxEnergy);
			p.maxenergy = p.energy;

			float dt = Math::randFloat();
			float pt = Math::randFloat();
			p.position.x = Math::randBetween(m_minPos.x, m_maxPos.x);
			p.position.y = Math::randBetween(m_minPos.y, m_maxPos.y);
			p.position.z = Math::randBetween(m_minPos.z, m_maxPos.z);
			p.direction = Math::randVectSphere(m_deviationStrength);
			p.direction += m_globalDirection;
			p.color = Color3(1,1,1);
			p.fade = Math::randBetween(m_minFade, m_maxFade);


			p.active = true;

		}
		m_index++;
		m_index %= m_addCount;
	}
}
void ParticleWind::init(int maxcount, int addcount, float minsize, float maxsize, float minenergy, float maxenergy,
						float minfade, float maxfade, float deviationstrength, Vector3& globaldirection,
						Vector3& minpos, Vector3& maxpos, bool preinit)
{
	m_count = maxcount;
	m_addCount = addcount;
	m_particles = new WindParticle[m_count];
	m_minSize = minsize;
	m_maxSize = maxsize;
	m_minFade = minfade;
	m_maxFade = maxfade;
	m_minEnergy = minenergy;
	m_maxEnergy = maxenergy;
	m_deviationStrength = deviationstrength;
	m_globalDirection = globaldirection;
	m_minPos = minpos;
	m_maxPos = maxpos;

	for (int i = 0; i < m_count; i++)
	{
		m_particles[i].active = false;
	}

	if (preinit)
	{
		//run a few iterations to initialize it visually (this is bad, yes)
		for (int i = 0; i < 50; i++)
		{
			update();
		}
	}


}




















Vector3 ParticleUtils::xr;
Vector3 ParticleUtils::yr;
Vector3 ParticleUtils::zr;

void ParticleUtils::update()
{
	glUtil::antiRotate(xr, yr, zr);
}

inline void ParticleUtils::project(Vector3& position, float size, Vector3 &v1, Vector3 &v2, Vector3& v3, Vector3 &v4)
{
	v1 = position - xr * size - yr * size;
	v2 = position + xr * size - yr * size;
	v3 = position + xr * size + yr * size;
	v4 = position - xr * size + yr * size;
}





ParticleSystem::ParticleSystem()
{
}
ParticleSystem::~ParticleSystem()
{
}

/*
class ParticleUtils
{
public:

	static void update();
	inline static void project(Vector3& position, float size, Vector3 &v1, Vector3 &v2, Vector3& v3, Vector3 &v4);
	static Vector3 xr, yr, zr;

};


class ParticleSystem
{
public:
	//TODO
	ParticleSystem();
	ParticleSystem~();

};
*/