#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    int nearX = (int)(x+0.5f);
    int nearY = (int)(y+0.5f);
    return get_pixel(im, nearX, nearY, c);
}

image nn_resize(image im, int w, int h)
{
    image new = make_image(w, h, im.c);

    float xScale = ((float)(im.w))/w;
    float yScale = ((float)(im.h))/h;
    for(int z=0;z<new.c;z++){
        for(int x=0;x<new.w;x++){
            for(int y=0;y<new.h;y++){
                float nearestX = (float)(x + 0.5f) * xScale - 0.5f;
                float nearestY = (float)(y + 0.5f) * yScale - 0.5f;
                set_pixel(new, x, y, z, nn_interpolate(im, nearestX, nearestY, z));
            }
        }
    }
    return new;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    float p1X = floorf(x);
    float p1Y = floorf(y);

    float p2X = ceilf(x);
    float p2Y = floorf(y);

    float p3X = floorf(x);
    float p3Y = ceilf(y);
    
    float p4X = ceilf(x);
    float p4Y = ceilf(y);

    // this gives us the four neighbouring points of the point (x,y) obtained by scaling the required image according to the original image
    
    float d1x = x-p1X;
    float d1y = y-p1Y;

    float d3x= x-p3X;
    float d3y = p3Y-y;
    
    float p1 = get_pixel(im, floor(p1X), floor(p1Y),c);
    float p2 = get_pixel(im, ceil(p2X), floor(p2Y),c);
    float p3 = get_pixel(im, floor(p3X), ceil(p3Y),c);
    float p4 = get_pixel(im, ceil(p4X), ceil(p4Y),c);

    float l1 = p1*(1-d1x)+p2*(d1x);
    float l2 = p3*(1-d1x)+p4*(d1x);
    float l = l1*d3y + l2*d1y;

    
    return l;
}

image bilinear_resize(image im, int w, int h)
{
image new2 = make_image(w, h, im.c);

    float width_ratio = (float)im.w / (float)w;
    float height_ratio = (float)im.h / (float)h;
    float value = 0.0;

    float lx, ly;

    // resize the image
    for (int y = 0; y < new2.h; y++)
    {
        for (int x = 0; x < new2.w; x++)
        {
            for (int z = 0; z < new2.c; z++)
            {
                lx = (x + 0.5f) * width_ratio-0.5f;
                ly = (y + 0.5f) * height_ratio-0.5f;
                value = bilinear_interpolate(im, lx, ly, z);

                set_pixel(new2, x, y, z, value);
            }
        }
    }

    return new2;
}

