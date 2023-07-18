#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    for(int z = 0; z < im.c; z ++) {
        float sumT = 0;
        for(int y = 0; y < im.h; y ++) {
            for(int x = 0; x < im.w; x ++) {
                sumT = sumT+get_pixel(im, x, y, z);
            }
        }

        if (sumT == 0) {
        return;
        }   

        for(int y = 0; y < im.h; y ++) {
            for(int x = 0; x < im.w; x ++) {
                set_pixel(im, x, y, z, get_pixel(im, x, y, z) / sumT);
            }
        }
    }
    
}

image make_box_filter(int w)
{
    image boxFilter = make_image(w, w, 1);
    float num_of_pixels = ((float)w) * ((float)w);
    for(int x = 0; x < w; x ++) {
        for(int y = 0; y < w; y ++) {
            set_pixel(boxFilter, y, x, 0, 1.0 / num_of_pixels);
        }
    }

    return boxFilter;

}

//New to function to help out in convolve_image

static float filter_value(image im, image f, int x, int y, int c, int cf)
 {
    float sum = 0;
    int fy = (f.h-1)/2;
    int fx = (f.w-1)/2; 

    for (int i = 0; i < f.w; i++) {
        for (int j = 0; j < f.h; j++) {
            sum = sum + (get_pixel(im, x - fx +i, y - fy + j, c) * get_pixel(f, i, j, cf));
        }
    }

    return sum;

}

image convolve_image(image im, image filter, int preserve)
{
    image convolve = copy_image(im);
    
    for (int z = 0; z < im.c; z++) {
        int filter_c = z;
        
        //Can reuse the filter if the image has only one channel

        if(filter.c == 1) {
            filter_c = 0;
        }
        
        for (int x = 0; x < im.w; x++) {
            for (int y = 0; y < im.h; y++) {
                set_pixel(convolve, x, y, z, filter_value(im, filter, x, y, z, filter_c));
            }
        }

    }
    
    // To get a 1 channel image

    if ((!preserve) || (im.c == 1)) {
        image sum = make_image(convolve.w, convolve.h, 1); 
        for (int x = 0; x < im.w; x++) {
            for (int y = 0; y < im.h; y++) {
                float sumD = 0;
                for(int z = 0; z < im.c; z++) {
                    sumD = sumD +  get_pixel(convolve, x, y ,z);
                }
                set_pixel(sum, x, y, 0, sumD);
            }
        }
        return sum;
    }
    return convolve;
}

image make_highpass_filter()
{
     image filter = make_image(3, 3, 1);
    set_pixel(filter, 0, 0, 0, 0);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 1, 1, 0, 4);
    set_pixel(filter, 1, 2, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);
    set_pixel(filter, 2, 1, 0, -1);
    set_pixel(filter, 2, 2, 0, 0);

    return filter;
}

image make_sharpen_filter()
{
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 0, 0, 0, 0);
    set_pixel(filter, 2, 1, 0, -1);
    set_pixel(filter, 2, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 1, 1, 0, 5);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 0, 2, 0, 0);
    
    return filter;
}

image make_emboss_filter()
{
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 0, 0, 0, -2);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 1, 1, 0, 1);
    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 2, 0, 0, 0);
    set_pixel(filter, 2, 1, 0, 1);
    set_pixel(filter, 2, 2, 0, 2);
    set_pixel(filter, 1, 2, 0, 1);
   
    return filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: sharpen, emboss and highpass.(Not sure why ??)

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: Yes, we need to normalize them so that the pixel value remains less than 1 and image doesn't go outside the boundary.

image make_gaussian_filter(float sigma)
{
    int boxL = 6 * sigma;
    if (boxL % 2 == 0) boxL += 1;

    image filter = make_image(boxL, boxL, 1);
    int center = (boxL - 1) / 2;
    for(int i = 0; i < boxL; i ++) {
        for(int j = 0; j < boxL; j ++) {
            float x = j - center;
            float y = i - center;
            float a = TWOPI * powf(sigma, 2);
            float b = powf(x, 2) + powf(y, 2);
            float c = 2 * powf(sigma, 2);
            float d = (1.0 /a) * expf((-1) * (b) / (c));
            set_pixel(filter, j, i, 0, d);
        }
    }
    return filter;
}

image add_image(image a, image b)
{
    // TODO
    return make_image(1,1,1);
}

image sub_image(image a, image b)
{
    // TODO
    return make_image(1,1,1);
}

image make_gx_filter()
{
    // TODO
    return make_image(1,1,1);
}

image make_gy_filter()
{
    // TODO
    return make_image(1,1,1);
}

void feature_normalize(image im)
{
    // TODO
}

image *sobel_image(image im)
{
    // TODO
    return calloc(2, sizeof(image));
}

image colorize_sobel(image im)
{
    // TODO
    return make_image(1,1,1);
}
