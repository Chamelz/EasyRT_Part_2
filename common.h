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

std::uniform_real_distribution<> dis(0.0, 1.0);
random_device rd;
mt19937 gen(rd());

double randf(){
    return dis(gen);
}

typedef struct Triangle{
    vec3 p1, p2, p3;
    vec3 center;
    Triangle(vec3 a, vec3 b, vec3 c){
        p1 = a;
        p2 = b;
        p3 = c;
        center = (p1+p2+p3) / vec3(3, 3, 3);
    }
}Triangle;

struct BVHNode{
    BVHNode* left = nullptr;    // 左子树索引
    BVHNode* right = nullptr;   // 右子树索引
    int n, index;               // 叶子节点信息
    vec3 AA, BB;                // 碰撞盒: AA 是包围盒的数值小的坐标，BB 是数值大的坐标
};

tm getTime(){
    time_t rawtime;
    struct tm *ptminfo;
    time(&rawtime);
    ptminfo = localtime(&rawtime);
    return *ptminfo;
}

void readOBJ(string filepath,
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
        GLfloat x, y, z;
        int v0, vt0, vn0;              // 面片第 1 个顶点的【位置，纹理坐标，法线】索引
        int v1, vt1, vn1;              //       2
        int v2, vt2, vn2;              //       3
        char slash;

        // 读取obj文件
        sin >> type;
        if(type == "v"){
            sin >> x >> y >> z;
            vertexPosition.push_back(vec3(x, y, z));
        }
        else if(type == "vt"){
            sin >> x >> y;
            vertexTexCoord.push_back(vec2(x, y));
        }
        else if(type == "vn"){
            sin >> x >> y >> z;
            vertexNormal.push_back(vec3(x, y, z));
        }
        else if(type == "f"){
            sin >> v0 >> slash >> vt0 >> slash >> vn0;
            sin >> v1 >> slash >> vt1 >> slash >> vn1;
            sin >> v2 >> slash >> vt2 >> slash >> vn2;
            positionIndex.push_back(ivec3(v0-1, v1-1, v2-1));
            texCoordIndex.push_back(ivec3(vt0-1, vt1-1, vt2-1));
            normalIndex.push_back(ivec3(vn0-1, vn1-1, vn2-1));
        }
    }
    // 根据面片信息生成最终传入顶点着色器的顶点数据
    for(int i=0;i<positionIndex.size();i++){
        // 顶点位置
        points.push_back(vertexPosition[positionIndex[i].x]);
        points.push_back(vertexPosition[positionIndex[i].y]);
        points.push_back(vertexPosition[positionIndex[i].z]);

        // 顶点纹理坐标
        texCoords.push_back(vertexTexCoord[texCoordIndex[i].x]);
        texCoords.push_back(vertexTexCoord[texCoordIndex[i].y]);
        texCoords.push_back(vertexTexCoord[texCoordIndex[i].z]);

        // 顶点法线
        normals.push_back(vertexNormal[normalIndex[i].x]);
        normals.push_back(vertexNormal[normalIndex[i].y]);
        normals.push_back(vertexNormal[normalIndex[i].z]);
    }
}

bool cmpx(const Triangle& t1, const Triangle& t2){
    return t1.center.x < t2.center.x;
}
bool cmpy(const Triangle& t1, const Triangle& t2){
    return t1.center.y < t2.center.y;
}
bool cmpz(const Triangle& t1, const Triangle& t2){
    return t1.center.z < t2.center.z;
}

#endif //EASYRT_COMMON_H
