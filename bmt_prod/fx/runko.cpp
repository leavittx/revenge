#include "runko.h"

void Runko::init()
{
	g_params.useNamespace("runko");
	m_debug = g_params.getBool("debug");
}

void Runko::release()
{
}

void Runko::update()
{
	g_params.useNamespace("runko");
}

void Runko::draw()
{
	g_params.useNamespace("runko");
}
