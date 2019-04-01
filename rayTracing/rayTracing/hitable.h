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
#include "aabb.h"
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
    virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;
};

aabb surrounding_box(aabb box0, aabb box1){
    vec3 small(fmin(box0.min().x(), box1.min().x()),
               fmin(box0.min().y(), box1.min().y()),
               fmin(box0.min().z(), box1.min().z()));
    vec3 big(fmin(box0.max().x(), box1.max().x()),
                   fmin(box0.max().y(), box1.max().y()),
                   fmin(box0.max().z(), box1.max().z()));
    return aabb(small, big);
}

#endif /* hitable_h */
