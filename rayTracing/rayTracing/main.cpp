//
//  main.cpp
//  rayTracing
//
//  Created by coderooookie on 2019/3/1.
//  Copyright © 2019 coderooookie. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "vec3.hpp"
//#include "vec3.cpp"
#include "ray.hpp"
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"
#include "moving_sphere.h"
#include "bvh_node.h"
#include "texture.h"
#include "perlin.h"
//float hit_sphere(const vec3& center, float radius, const ray& r){
//    vec3 oc = r.origin() - center;
//    float a = dot(r.direction(), r.direction());
//    float b = 2.0 * dot(oc, r.direction());
//    float c = dot(oc, oc) - radius * radius;
//    float discriminat = b * b - 4 * a * c;
//    if (discriminat < 0){
//        return -1.0;
//    }
//    else{
//        return (-b - sqrt(discriminat))/(2.0*a);
//    }
//}

//vec3 color(const ray& r){
//    float t = hit_sphere(vec3(0,0,-1), 0.5, r);
//    if (t > 0.0){
//        vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0,0,-1));
//        return 0.5 * vec3(N.x() + 1, N.y() + 1, N.z() + 1);
//    }
//    vec3 unit_direction = unit_vector(r.direction());
//    t = 0.5 * (unit_direction.y() + 1.0);
//    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
//}
//vec3 random_in_unit_sphere(){
//    vec3 p;
//    do {
//        p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1,1,1);
//    }while (p.squared_length() >= 1.0);
//    return p;
//}
//vec3 color(const ray& r, hitable *world){
//    hit_record rec;
//    if (world->hit(r, 0.001, MAXFLOAT, rec)){
//        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
//        return 0.5 * color(ray(rec.p, target - rec.p), world);
////        return 0.5 * vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
//    }
//    else{
//        vec3 unit_direction = unit_vector(r.direction());
//        float t = 0.5 * (unit_direction.y() + 1);
//        return (1-t)*vec3(1,1,1) + t * vec3(0.5, 0.7, 1.0);
//    }
//}

vec3 color(const ray& r, hitable *world, int depth){
    hit_record rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec)){
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_prt->scatter(r, rec, attenuation, scattered)){
            return attenuation*color(scattered, world, depth+1);
        }
        else{
            return vec3(0,0,0);
        }
    }
    else{
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1);
        return (1-t)*vec3(1,1,1) + t * vec3(0.5, 0.7, 1.0);
    }
}

hitable *random_scene(){
    int n = 500;
    hitable **list = new hitable*[n+1];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(new constant_texture(vec3(0.5, 0.5,0.5))));
    int i = 1;
    for (int a = -11; a < 11; a++){
        for (int b = -11; b < 11; b++){
            float choose_mat = drand48();
            vec3 center(a + 0.9*drand48(), 0.2, b + drand48());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9){
                if (choose_mat < 0.8){
//                    list[i++] = new sphere(center, 0.2, new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
                    list[i++] = new moving_sphere(center, center + vec3(0, 0.5*drand48(),0), 0, 1.0, 0.2, new lambertian(new constant_texture(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48()))));
                }
                else if (choose_mat < 0.95){
                    list[i++] = new sphere(center, 0.2, new metal(vec3(0.5 * (1 + drand48()), 0.5 * (1 + drand48()), 0.5 * (1 + drand48())), 0.5 * drand48()));
                }
                else {
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }
    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
//    return new hitable_list(list, i);
    return new bvh_node(list, i, 0, 1.0);
}

hitable* two_spheres(){
    texture* checker = new checker_texture(new constant_texture(vec3(0.3,0.2,0.1)), new constant_texture(vec3(0.9,0.9,0.9)));
    int n = 50;
    hitable **list = new hitable*[n+1];
    list[0] = new sphere(vec3(0, -10, 0), 10, new lambertian(checker));
    list[1] = new sphere(vec3(0, 10, 0), 10, new lambertian(checker));
    return new bvh_node(list, 2, 0, 1);
}

hitable* two_perlin_spheres(){
    texture* pertext = new noise_texture(15);
    hitable **list = new hitable*[2];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
    list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
    return new bvh_node(list, 2, 0, 1);
}

pthread_mutex_t mutex1;
const int sizeX = 1200;
const int sizeY = 800;
vec3 colorResult[sizeX*sizeY];
struct thread_data {
    int nx;
    int ny;
    int ns;
    int i;
    int j;
    camera* cam;
    hitable* world;
};

void* generateColor(void* threadArgs){
    thread_data* threadData;
    threadData = (thread_data*) threadArgs;
    vec3 col(0,0,0);
    for (int s=0; s <threadData->ns; s++){
        float u = float(threadData->i + drand48())/threadData->nx;
        float v = float(threadData->j + drand48())/threadData->ny;
        ray r = threadData->cam->get_ray(u, v);
        col += color(r, threadData->world, 0);
    }
    col /= float(threadData->ns);
    col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
    pthread_mutex_lock(&mutex1);
//    std::cout << col << std::endl;
    colorResult[(threadData->j) * (threadData->nx) + (threadData->i)] = col;
    pthread_mutex_unlock(&mutex1);
    pthread_exit(NULL);
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    std::ofstream outfile("raytracing.ppm");
    int nx = 1200;
    int ny = 800;
    int ns = 10;
    outfile << "P3\n" << nx << " " << ny << "\n255\n";
    hitable *list[5];
    list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(new constant_texture(vec3(0.1, 0.2, 0.5))));
    list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(new constant_texture(vec3(0.8, 0.8, 0.0))));
    list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.1));
    list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
    list[4] = new sphere(vec3(-1, 0, -1), -0.49, new dielectric(1.5));
//    hitable* world = new bvh_node(list, 5, 0, 1);
//    hitable* world = random_scene();
    hitable* world = two_perlin_spheres();
    vec3 lookfrom = vec3(13,2,3);
    vec3 lookat = vec3(0,0,0);
    float dist_to_focus = 10;
    float aperture = 0.1;
//    colorResult[10] = vec3(0,0,0);
    camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx)/float(ny), aperture, dist_to_focus, 0, 1.0);
    for (int j = ny-1; j >= 0; j--){
        for (int i = 0; i < nx; i++){
            vec3 col(0, 0, 0);
            for (int s=0; s <ns; s++){
                float u = float(i + drand48())/nx;
                float v = float(j + drand48())/ny;
                ray r = cam.get_ray(u, v);
                col += color(r, world, 0);
            }
            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            //            vec3 p = r.point_at_parameter(2.0);
            int ir = int(255.99 * col.r());
            int ig = int(255.99 * col.g());
            int ib = int(255.99 * col.b());
            outfile << ir << " " << ig << " " << ib << "\n";
        }
    }
    //多线程方法
//    pthread_t tids[nx*ny];
//    for (int j = ny-1; j >= 0; j--){
//        for (int i = 0; i < nx; i++){
//            thread_data threadData = { nx,ny,ns,i,j,&cam,world};
//            //            pthread_create(pthread_t  _Nullable * _Nonnull, const pthread_attr_t * _Nullable, void * _Nullable (* _Nonnull)(void * _Nullable), void * _Nullable)
//            int ret = pthread_create(&tids[j*nx + i], NULL, generateColor, &threadData);
//            if (ret != 0)
//            {
//                std::cout << "pthread_create error: error_code=" << ret << std::endl;
//            }
//        }
//    }
//    for (int j = ny-1; j >= 0; j--){
//        for (int i = 0; i < nx; i++){
//            //            vec3 p = r.point_at_parameter(2.0);
//            int ir = int(255.99 * (colorResult[j*nx + i]).r());
//            int ig = int(255.99 * (colorResult[j*nx + i]).g());
//            int ib = int(255.99 * (colorResult[j*nx + i]).b());
//            outfile << ir << " " << ig << " " << ib << "\n";
//        }
//    }
    //
    outfile.close();
    return 0;
}
