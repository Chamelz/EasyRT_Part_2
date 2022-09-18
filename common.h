//
// Created by Chamelz on 2022/9/18.
//

#ifndef EASYRT_COMMON_H
#define EASYRT_COMMON_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>

#include <svpng.inc>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace glm;
using namespace std;

#define INF 114514.0

// 输出图像分辨率
const int WIDTH = 256;
const int HEIGHT = 256;

// 采样次数
const int SAMPLE = 4096;
// 每次采样的亮度
const double BRIGHTNESS = (2.0f * 3.1415926f) * (1.0f / double(SAMPLE));

void readOBJ(
        string filepath,
        vector<vec3>& points,
        vector<vec2>& texCoords,
        vector<vec3>& normals
        ){
    // 顶点属性
    vector<vec3> vertexPosition;
    vector<vec2> vertexTexCoord;
    vector<vec3> vertexNormal;

    // 面片索引信息
    vector<ivec3> positionIndex;
    vector<ivec3> texCoordIndex;
    vector<ivec3> normalIndex;

    // 打开文件流
    ifstream fin(filepath);
    string line;
    if(!fin.is_open()){
        cout << "FILE: " << filepath << " FAILED TO OPEN" << endl;
        exit(-1);
    }
    // 按行读取
    while(getline(fin, line)){
        istringstream sin(line);    // 以一行的数据作为 string stream 解析并且读取
        string type;
        float x, y, z;


    }

}


typedef struct Ray{
    vec3 startPoint = vec3(0, 0, 0);
    vec3 direction  = vec3(0, 0, 0);
}Ray;

typedef struct Material{
    bool isEmissive     = false;                    // 是否发光
    vec3 normal         = vec3(0, 0, 0);    // 法向量
    vec3 color          = vec3(0, 0, 0);    // 颜色
    double specularRate = 0.0f;                     // 反射率, 反射光占比
    double roughness    = 1.0f;                     // 粗糙程度
    double refractRate  = 0.0f;                     // 折射光占比
    double refractAngle = 1.0f;                     // 折射率
    double refractRoughness = 0.0f;                 // 折射粗糙度

}Material;

typedef struct HitResult{
    bool isHit      = false;                    // 是否命中
    double distance = 0.0f;                     // 与交点的距离
    vec3 hitPoint   = vec3(0, 0, 0);    // 光线命中点
    Material        material;                   // 命中点表面的材质
};

class Shape{
public:
    Shape(){}
    virtual HitResult intersect(Ray ray){   return HitResult(); }
};

class Triangle : public Shape{
public:
    Triangle(){}
    Triangle(vec3 P1, vec3 P2, vec3 P3, vec3 C){
        p1 = P1;
        p2 = P2;
        p3 = P3;
        material.normal = normalize(cross(p2-p1, p3-p1));
        material.color = C;
    }
    HitResult intersect(Ray ray){
        HitResult res;

        vec3 S = ray.startPoint;
        vec3 d = ray.direction;
        vec3 N = material.normal;
        if(dot(N, d) > 0)   N = -N;     // 获取正确的法向量

        // 如果视线和三角形平行
        if(fabs(dot(N, d)) < 0.00001f)  return res;

        // 距离
        float t = (dot(N, p1) - dot(S, N)) / dot(d, N);
        if(t < 0.00005f)    return res;     // 如果三角形在相机背面

        // 交点计算
        vec3 P = S + d * t;

        // 判断交点是否在三角形内
        vec3 c1 = cross(p2-p1, P-p1);
        vec3 c2 = cross(p3-p2, P-p2);
        vec3 c3 = cross(p1-p3, P-p3);
        vec3 n  = material.normal;  // 需要 "原生法向量" 来判断
        if(dot(c1, n)<0 || dot(c2, n)<0 || dot(c3,n)<0)     return res;

        // 装填返回结果
        res.isHit           = true;
        res.distance        = t;
        res.material        = material;
        res.hitPoint        = P;
        res.material.normal = N;

        return res;
    }
public:
    vec3 p1, p2, p3;    // 顶点
    Material material;  // 材质
};

class Sphere : public Shape{
public:
    Sphere(){};
    Sphere(vec3 o, double r, vec3 c, double spec, double rough){
        O = o;
        R = r;
        material.color = c;
        material.specularRate = spec;
        material.roughness = rough;
    }

    HitResult intersect(Ray ray){
        HitResult res;
        vec3 S = ray.startPoint;
        vec3 Dir = ray.direction;
        float OS = length(O-S);
        float SH = dot(O-S, Dir);
        float OH = sqrt(pow(OS, 2) - pow(SH, 2));
        float PH = sqrt(pow(R, 2) - pow(OH, 2));

        if(OH > R)      return res;     // OH大于半径则不相交
        float t1 = length(SH) - PH;
        float t2 = length(SH) + PH;
        float t = (t1<0) ? t2 : t1;     // 最近距离
        vec3 P = S + t*Dir;             // 交点

        // 防止自己交自己
        if(fabs(t1)<0.0005f || fabs(t2)<0.0005f)    return res;

        // 装填返回结果
        res.isHit = true;
        res.material = material;
        res.hitPoint = P;
        res.distance = t;
        res.material.normal = normalize(P - O); // 要返回正确的法向
        return res;
    }

public:
    vec3 O;
    double R;
    Material material;
};

vector<Shape*> shapes;      // 几何物体的集合

std::uniform_real_distribution<> dis(0.0, 1.0);
random_device rd;
mt19937 gen(rd());

double randf(){
    return dis(gen);
}

void imshow(double* SRC){
    unsigned char* image = new unsigned char[WIDTH * HEIGHT * 3];   // 图像buffer
    unsigned char* p = image;
    double* S = SRC;    // 源数据
    FILE* fp = fopen("../image_8_Final.png", "wb");
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            *p++ = (unsigned char)clamp(pow(*S++, 1.0f / 2.2f) * 255, 0.0, 255.0);  // R 通道
            *p++ = (unsigned char)clamp(pow(*S++, 1.0f / 2.2f) * 255, 0.0, 255.0);  // G 通道
            *p++ = (unsigned char)clamp(pow(*S++, 1.0f / 2.2f) * 255, 0.0, 255.0);  // B 通道
        }
    }
    svpng(fp, WIDTH, HEIGHT, image, 0);
}

HitResult shoot(vector<Shape*> shapes, Ray ray){
    HitResult res, r;
    res.distance = INFINITY;
    for(auto& shape : shapes){
        r = shape->intersect(ray);
        if(r.isHit && r.distance < res.distance)    res = r;    // 记录距离最近的求交结果
    }
    return res;
}

vec3 randomVec3(){
    vec3 d;
    do{
        d = 2.0f * vec3(randf(), randf(), randf()) - vec3(1.0, 1.0, 1.0);
    }while(dot(d, d) > 1.0f);
    return normalize(d);
}

vec3 randomDirection(vec3 n){
    return normalize(randomVec3() + n);
}

vec3 pathTracing(vector<Shape*> shapes, Ray ray, int depth){
    // 大于8层直接返回
    if(depth > 8)   return vec3(0, 0, 0);

    HitResult res = shoot(shapes, ray);

    // 未命中
    if(!res.isHit)                  return vec3(0, 0, 0);

    // 如果发光则返回颜色
    if(res.material.isEmissive)     return res.material.color;

    // 有 P 的概率终止
    double r = randf();
    float P = 0.8;
    if(r > P)   return vec3(0, 0, 0);

    // 否则继续
    Ray randomRay;
    randomRay.startPoint = res.hitPoint;
    randomRay.direction  = randomDirection(res.material.normal);

    vec3 color = vec3(0, 0, 0);
    float cosine = dot(-ray.direction, res.material.normal);

    // 根据反射率决定光线最终的方向
    r = randf();
    // 当随机数小于 reflectRate 的时候发生反射，
    // 随机数在 reflectRate 和 refractRate 之间发生折射，
    // 随机数大于 refractRate 的时候才是漫反射
    if(r < res.material.specularRate){      // 镜面反射
        // 我们反射的时候不再按照反射光线的方向，而是根据粗糙度，在随机向量和反射光线的方向做一个*线性插值*以决定最终反射的方向
        vec3 reflect = normalize(glm::reflect(ray.direction ,res.material.normal));
        randomRay.direction = mix(reflect, randomRay.direction, res.material.roughness);
        color = pathTracing(shapes, randomRay, depth+1) * cosine;
    }
    else if(r<res.material.refractRate && r>res.material.specularRate){
        vec3 refract = normalize(glm::refract(ray.direction, res.material.normal, float(res.material.refractAngle)));
        randomRay.direction = mix(refract, -randomRay.direction, res.material.refractRoughness);
        color = pathTracing(shapes, randomRay, depth+1) * cosine;
    }
    else{
        vec3 srcColor = res.material.color;
        vec3 ptColor = pathTracing(shapes, randomRay, depth+1) * cosine;
        color = ptColor * srcColor;    // 和原颜色混合
    }

    // 否则直接返回
    return color/P;
}

tm getTime(){
    time_t rawtime;
    struct tm *ptminfo;
    time(&rawtime);
    ptminfo = localtime(&rawtime);
    return *ptminfo;
}


#endif //EASYRT_COMMON_H
