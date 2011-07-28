#include <string.h>

#include "imagefactory.h"

#ifdef _WIN32
#include "DevIL/il.h"
#include "DevIL/ilu.h"
#include "DevIL/ilut.h"
#else
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#endif

Image* ImageFactory::createEmpty(int width, int height)
{
    Image *image = new Image();
    //create image and
    image->m_width = width;
    image->m_height = height;
    image->m_data = new unsigned int [image->m_width * image->m_height];
    memset(image->m_data, 0, image->m_width*image->m_height*sizeof(unsigned int));
    return image;
}
Image* ImageFactory::loadJPG(string filename)
{
    unsigned int imageID;
    unsigned char *ptr = NULL, *dest = NULL;

    ilInit();
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    // Load image from a file
    if(!ilLoadImage((char *)filename.c_str()))
    {
        g_debug << "loading image " << filename << " failed!" << endl;
        return 0;
    }

    Image *image = new Image();
    image->m_width = ilGetInteger(IL_IMAGE_WIDTH);
    image->m_height = ilGetInteger(IL_IMAGE_HEIGHT);
    image->m_data = new unsigned int[image->m_width*image->m_height];

    ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
    ptr = ilGetData();
    dest = (unsigned char *)image->m_data;
    // Fix x64 texture bug
    for(int i = 0; i < image->m_width*image->m_height; i++, dest++)
    {
      /*
        *dest++ = *ptr++;
        *dest++ = *ptr++;
        *dest++ = *ptr++;
        *dest++ = 255; //TODO: adjustable alpha channel?
       */
      for(int j = 0, bytes = 16; j < 4; j++, bytes -= 8)
        *dest |= (*ptr++ << bytes); // Correctly pack bytes into uint!
      *dest |= 0xFF; //TODO: adjustable alpha channel?
    }
    ilDeleteImages(1, &imageID);

    return image;

}

Image* ImageFactory::loadPNG(string filename)
{
    unsigned int imageID;

    ilInit();
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    // Load image from a file
    if(!ilLoadImage((char *)filename.c_str()))
    {
        g_debug << "loading image " << filename << " failed!" << endl;
        return 0;
    }

    Image *image = new Image();
    image->m_width = ilGetInteger(IL_IMAGE_WIDTH);
    image->m_height = ilGetInteger(IL_IMAGE_HEIGHT);
    image->m_data = new unsigned int[image->m_width*image->m_height];
    // Clear data buffer
    memset(image->m_data, 0,
           sizeof(unsigned int) * image->m_width * image->m_height);
    // Here is our "texture" bag: source == NULL(on x64):)
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    unsigned char *source = (unsigned char *)ilGetData();
    unsigned int *dest = image->m_data;
    for(int i = 0; i < image->m_width*image->m_height; i++, dest++)
    {
       // Here our bug:
        //*dest++ = *source++;
      /* Little "black magic" trick(^__^)
        *dest |= (*source++ << 24); // R
        *dest |= (*source++ << 16); // G
        *dest |= (*source++ << 8); // B
        *dest |= (*source++); // A
      */
      for(int j = 0, bytes = 24; j < 4; j++, bytes -= 8)
        *dest |= (*source++ << bytes); // Correctly pack bytes into uint!
    }

    /*
 ptr = ilGetData();
 dest = (unsigned int *)m_data;

 for(int i = 0; i < m_width*m_height; i++)
 {
  *dest++ = *ptr++;
  *dest++ = *ptr++;
  *dest++ = *ptr++;
  *dest++ = 255; //TODO: adjustable alpha channel?
 }
*/
    ilDeleteImages(1, &imageID);

    return image;
}
