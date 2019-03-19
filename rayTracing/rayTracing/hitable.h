//
//  hitable.h
//  rayTracing
//
//  Created by coderooookie on 2019/3/6.
//  Copyright © 2019 coderooookie. All rights reserved.
//

#ifndef hitable_h
#define hitable_h

#include "ray.hpp"

class material;

struct hit_record {
    float t;
    vec3 p;
    vec3 normal;
    material* mat_prt;
};

class hitable {
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};


#endif /* hitable_h */
