#pragma once

#include "../globals.h"


class Image
{
    friend class ImageFactory;
public:
    Image();
    ~Image();

    void clear();
    void releaseData();

    int getWidth() { return m_width; }
    int getHeight() { return m_height; }
    unsigned int *getData() { return m_data; }

private:
    int m_width;
    int m_height;
    unsigned int *m_data;

};

