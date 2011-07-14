#pragma once

/*
    An abstract base class for a single scene/effect in the demo. The timeline contains references to these.
*/

class Scene
{
public:

    Scene();
    //pure virtual destructor so the correct one will be called once the effect is deleted. 
    virtual ~Scene() = 0;

    //does all the initialization in the effect. Allocation etc. should be put here
    virtual void init() = 0;
    //frees all memory etc.
    virtual void release() = 0;
    //updates the effect visuals. Called at a constant framerate so fixed values can and should be used. 
    virtual void update() = 0;
    //draws the effect
    virtual void draw() = 0;

    //sets the normalized position of the effect (pos == 0 is the beginning of the effect, pos == 1 is the end)
    void setPosition(const float t);

protected:
    float m_pos; //normalized position in the effect

private:
    


};

