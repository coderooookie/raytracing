//
//  texture.h
//  rayTracing
//
//  Created by 洪树斌 on 2019/4/3.
//  Copyright © 2019 洪树斌. All rights reserved.
//

#ifndef texture_h
#define texture_h
#include "vec3.hpp"
#include "perlin.h"

class texture {
public:
    virtual vec3 value(float u, float v, const vec3&p) const = 0;
};

class constant_texture : public texture {
public:
    constant_texture() {}
    constant_texture(vec3 c) : color(c) {}
    virtual vec3 value(float u, float v, const vec3& p) const {return color;}
    vec3 color;
};

class checker_texture : public texture {
public:
    checker_texture() {}
    checker_texture(texture* t0, texture* t1):even(t0), odd(t1){}
    virtual vec3 value(float u, float v, const vec3& p) const {
        float sines = sin(10*p.x()) * sin(10*p.x()) * sin(10*p.x());
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }
    
    texture* odd;
    texture* even;
};


class noise_texture : public texture{
public:
    noise_texture(){}
    virtual vec3 value(float u, float v, const vec3& p) const{
        return vec3(1,1,1) * noise.noise(p);
    }
    perlin noise;
};
#endif /* texture_h */
