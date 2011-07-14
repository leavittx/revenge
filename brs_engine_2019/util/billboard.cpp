#include "billboard.h"

Billboard::Billboard()
{
}
Billboard::~Billboard()
{
}



BillboardSystem::BillboardSystem()
{
	clear();

}

BillboardSystem::~BillboardSystem()
{
}


void BillboardSystem::clear()
{
	m_billboards.clear();
}


void BillboardSystem::draw()
{
	//TODO:

	//get modelview matrix
	//multiply position with modelview matrix
	//sort by z
	//draw 



}

void BillboardSystem::update()
{
	//?
}