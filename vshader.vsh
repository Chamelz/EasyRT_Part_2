#version 330 core
//layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 0) in vec3 vPosition;  // cpu传入的顶点坐标

out vec2 TexCoords;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(vPosition, 1.0);
    Normal = aNormal;
//    gl_Position = model * vec4(vPosition, 1.0);
}