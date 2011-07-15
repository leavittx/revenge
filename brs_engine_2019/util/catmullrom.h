#pragma once

#include <vector>
#include "../globals.h"

/*
    Catmull-Rom spline class.
*/

class CatmullRomCurve : public Curve
{
public:
    CatmullRomCurve();
    ~CatmullRomCurve();

    //palauttaa kopion
    CatmullRomCurve *clone();

    void begin();
    void addPoint(const Vector3 &point);
    void end();

    //parametrisoi kaaren pituuden mukaan (laittaa pisteet tasavälein)
    //tuhoaa vanhan splinen, ts. osoittimet taulukkoon hajoavat
    //newSplinePoints on uuden splinin pisteiden määrä (nollalla käyttää alkuperäistä pistemäärää)
    //samples on splinistä otettavien näytteiden määrä 
    void arcLengthParametrize(int newSplinePoints = 0, int samples = 1000);

    Vector3 getValue(float t);
    Vector3 getTangent(float t);

	void reverse();

    Vector3 *getPoints() { return this->points; };
    int getPointCount() { return this->pcount; };

private:
    int pcount;
    Vector3 *points;

    void destroy();
    void set(CatmullRomCurve *src);

    std::vector<Vector3> insertedPoints;


};



