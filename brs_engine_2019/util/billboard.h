#pragma once

#include "../globals.h"

class Billboard
{
public:
	Billboard();
	~Billboard();

	Vector3 position;
	float size;
};


class BillboardSystem
{
public:
	BillboardSystem();
	~BillboardSystem();

	void draw();
	void clear();
	void update();

	vector<Billboard>& getBillboards() { return m_billboards; };

private:
	vector<Billboard> m_billboards;

};
