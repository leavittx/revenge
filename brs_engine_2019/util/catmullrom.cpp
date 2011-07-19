#include "catmullrom.h"

CatmullRomCurve::CatmullRomCurve()
{
    this->points = 0;
    this->pcount = 0;
}

CatmullRomCurve::~CatmullRomCurve()
{
    this->destroy();
}

void CatmullRomCurve::destroy()
{
    this->pcount = 0;
    if (this->points != 0)
    {
        delete [] this->points;
        this->points = 0;
    }
}

CatmullRomCurve *CatmullRomCurve::clone()
{
    CatmullRomCurve *temp = new CatmullRomCurve();
    temp->set(this);
    return temp;
}

void CatmullRomCurve::set(CatmullRomCurve *src)
{
    this->destroy();
    this->pcount = src->pcount;
    this->points = new Vector3[this->pcount];

    for (int i = 0; i < this->pcount; i++)
    {
        this->points[i] = src->points[i];
    }

}

void CatmullRomCurve::begin()
{
    this->destroy();
}

void CatmullRomCurve::addPoint(const Vector3 &point)
{
    this->insertedPoints.push_back(point);
}

void CatmullRomCurve::reverse()
{
    if (this->pcount <= 0 || !this->points)
    {
        return;
    }

    //ugly
    Vector3* temp = new Vector3[this->pcount];
    for (int i = 0; i < this->pcount; i++)
    {
        temp[i] = this->points[this->pcount - i - 1];
    }
    delete [] this->points;
    this->points = temp;


}
void CatmullRomCurve::end()
{
    //destroy() kutsuttu jo aikaisemmin
    this->pcount = this->insertedPoints.size();

    this->points = new Vector3[this->pcount];

    std::vector<Vector3>::iterator i;

    int pointoffset = 0;
    for (i = this->insertedPoints.begin(); i < this->insertedPoints.end(); i++)
    {
        Vector3 &p = *i;
        this->points[pointoffset++] = p;
    }

    this->insertedPoints.clear();
}

void CatmullRomCurve::arcLengthParametrize(int newSplinePoints, int samples)
{
    //lasketaan splinin pituus

    float l = 0.0f;
    float t = 0.0f;

    Vector3 prev = this->getValue(0.0f);

    for (t = 0.0f; t < 1.0f; t += 1.0f / samples)
    {
        Vector3 cur = this->getValue(t);
        Vector3 diff = cur - prev;

        l += diff.length();
        prev = cur;
    }

    if (newSplinePoints == 0)
    {
        newSplinePoints = this->pcount;
    }


    float segmentLength = l / newSplinePoints;
    float cumlength = 0.0f;
    prev = this->getValue(0.0f);

    t = 0.0f;

    CatmullRomCurve *uusi = new CatmullRomCurve();
    uusi->begin();

    for (int i = 0; i < newSplinePoints; i++)
    {
        while (cumlength < segmentLength)
        {
            //mennään spliniä eteenpäin
            Vector3 cur = this->getValue(t);
            float d = (cur - prev).length();
            cumlength += d;

            prev = cur;
            t += 1.0f / samples;
        }
        //laitetaan uusi piste
        Vector3 p = this->getValue(t);
        uusi->addPoint(p);

        cumlength -= segmentLength;
    }

    uusi->end();
    this->destroy();
    this->set(uusi);




}


Vector3 CatmullRomCurve::getValue(float pos)
{

    if (this->points == 0)
    {
        return Vector3(0, 0, 0);
    }
    if (this->pcount < 4)
    {
        return Vector3(0, 0, 0);
    }

    //t = 0 -> index = 1
    //t = 1.0f -> index = count - 2

    if (pos > 1.000f)
    {
        return Vector3(0, 0, 0);
    }
    if (pos < 0.000f)
    {
        return Vector3(0, 0, 0);
    }

    float val = (pos * (pcount-3)) + 1;
    const int index = ((int)floorf(pos * (this->pcount-3))) + 1;

    float t = val - index;

    float t2 = t*t;
    float t3 = t2*t;

    Vector3& p0 = this->points[index - 1];
    Vector3& p1 = this->points[index];
    Vector3& p2 = this->points[index + 1];
    Vector3& p3 = this->points[index + 2];

    Vector3 result = ((p1*2) + (p2 - p0) * t + (p0*2 - p1 * 5 + p2*4 - p3)*t2 + (p1*3 - p0- p2*3 + p3) * t3)*0.5f;
    return result;

}

Vector3 CatmullRomCurve::getTangent(float pos)
{
    return Vector3(0, 0, 0);
}
