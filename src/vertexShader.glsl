#version 330 core
layout (location = 0) in vec3 aPos; // position has attribute position 0
layout (location = 1) in vec3 aColor; // color has attribute position 1
out vec3 ourColor; // output a color to the fragment shader

vec3 bPos;
uniform float Xoffset;

void main()
{
bPos = aPos.xyz;
//bPos = -bPos;
//bPos.x =bPos.x + Xoffset;
gl_Position = vec4(bPos, 1.0);
ourColor = aColor; // set ourColor to input color from the vertex data
//ourColor = bPos;
}