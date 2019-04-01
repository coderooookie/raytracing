//
//  ray.hpp
//  rayTracing
//
//  Created by coderooookie on 2019/3/3.
//  Copyright © 2019 coderooookie. All rights reserved.
//

#ifndef ray_hpp
#define ray_hpp

#include <stdio.h>
#include "vec3.hpp"

class ray
{
public:
    ray(){}
    ray(const vec3& a, const vec3& b, float ti = 0.0){A = a; B = b; _time = ti;}
    vec3 origin() const {return A;}
    vec3 direction() const {return B;}
    vec3 point_at_parameter(float t) const {return A + B*t;}
    float time() const {return _time;}
    
    vec3 A;
    vec3 B;
    float _time;
};

#endif /* ray_hpp */
