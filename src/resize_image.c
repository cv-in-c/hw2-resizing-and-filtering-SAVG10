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
    // TODO
    return 0;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    return make_image(1,1,1);
}

