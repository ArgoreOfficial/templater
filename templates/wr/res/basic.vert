#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aVel;
layout (location = 2) in double aMass;
layout (location = 3) in vec3 aColor;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec3 color;

void main()
{
   gl_Position = proj * view * model * vec4(aPos, 1.0);
   color = aColor;
}