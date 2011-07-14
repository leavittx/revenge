#pragma once

class Entity
{
public:
	Entity();
	~Entity();

	//virtual functions to be overloaded
	virtual void draw() = 0;
	virtual void init() = 0;
	virtual void update() = 0;
};