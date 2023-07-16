#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    int index; 
    float pixVal;
    if(x<0){
        x=0;
    }
    if(x>im.w-1){
        x=im.w-1;
    }
    if(y<0){
        y=0;
    }
    if(y>im.h-1){
        y=im.h-1;
    }
    if(c<0){
        c=0;
    }
    if(c>im.c-1){
        c=im.c-1;
    }
    index =  x+y*im.w+c*im.h*im.w;
    pixVal = im.data[index];

    return pixVal;
}

void set_pixel(image im, int x, int y, int c, float v)
{
    if(x<0||x>im.w-1||y<0||y>im.h-1||c<0||c>im.c-1){
        return;
    }
    else {
        int index;
        index=c*im.h*im.w+y*im.w+x;
        im.data[index]=v;
    }
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    for(int c=0;c<im.c;c++){
        for(int y=0;y<im.h;y++){
            for(int x=0;x<im.w;x++){
                int index = c*im.h*im.w + y*im.w + x;
                copy.data[index]=im.data[index];
            }
        }
    }
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    for(int y=0;y<im.h;y++){
        for(int x=0;x<im.w;x++){
            int gray_index = y*im.w+x;
            int red_ind = y*im.w+x;
            int green_ind = 1*im.h*im.w+y*im.w+x;
            int blue_ind = 2*im.h*im.w+y*im.w+x;
            gray.data[gray_index]=0.299*im.data[red_ind]+0.587*im.data[green_ind]+0.114*im.data[blue_ind];
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    for(int y=0;y<im.h;y++){
        for(int x=0;x<im.w;x++){
            int index=c*im.h*im.w+y*im.w+x;
            im.data[index]+=v;
        }
    }
}

void clamp_image(image im)
{
    for(int t=0;t<im.h*im.w*im.c;t++){
        if(im.data[t]<0){
            im.data[t]=0;
        }
        else if(im.data[t]>1){
            im.data[t]=1;
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    for(int y=0;y<im.h;y++){
        for(int x=0;x<im.w;x++){
            float r =  im.data[y*im.w+x];
            float g = im.data[1*im.h*im.w+y*im.w+x];
            float b = im.data[2*im.h*im.w+y*im.w+x];
            float V = three_way_max(r,g,b);
            float m = three_way_min(r,g,b);
            float C =  V-m;
            float S;
            if(V==0){
                S=0;
            }
            else{
                S = C/V;
            }
            float H,Hp;
            if(C==0){
                Hp=0;
            }
            else if(V==r){
                Hp=(g-b)/C;
            }
            else if(V==g){
                Hp = (b-r)/C+2;
            }
            else if(V==b){
                Hp = (r-g)/C+4;
            }
            if(Hp<0){
                H=Hp/6+1;
            }
            else{
                H=Hp/6;
            }
            im.data[y*im.w+x]=H;
            im.data[1*im.h*im.w+y*im.w+x]=S;
            im.data[2*im.h*im.w+y*im.w+x]=V;
        }
    }
}

void hsv_to_rgb(image im)
{
    for(int y=0;y<im.h;y++){
        for(int x=0;x<im.w;x++){
            float H = im.data[y*im.w+x];
            float S = im.data[1*im.h*im.w+y*im.w+x];
            float V = im.data[2*im.h*im.w+y*im.w+x];
            float Hp = H*6;
            float f = floor(Hp);
            float d = Hp-f;
            float C = V*S;
            float X_1 = V*(1-S*d);
            float X_2 = V*(1-S*(1-d));
            float r,g,b;
            float m = V-C;
            if(Hp>=0 && Hp<1){
                r=V;
                g=X_2;
                b=m;
            }
            else if(Hp>=1 && Hp<2){
                r=X_1;
                g=V;
                b=m;
            }
            else if(Hp>=2 && Hp<3){
                r=m;
                g=V;
                b=X_2;
            }
            else if(Hp>=3 && Hp<4){
                r=m;
                g=X_1;
                b=V;
            }
            else if(Hp>=4 && Hp<5){
                r=X_2;
                g=m;
                b=V;
            }
            else{
                r=V;
                g=m;
                b=X_1;
            }
            im.data[y*im.w+x]=r;
            im.data[1*im.h*im.w+y*im.w+x]=g;
            im.data[2*im.h*im.w+y*im.w+x]=b;
        }
    }
}

void scale_image(image im, int c, float v){
    for(int y=0;y<im.h;y++){
        for(int x=0;x<im.w;x++){
            int index=c*im.h*im.w+y*im.w+x;
            im.data[index]*=v;
        }
    }
}
