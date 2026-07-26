#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;
void main()
{
vec4 color = vec4(1.0);
color.x = lightColor.x;
color.y = lightColor.y;
color.z = lightColor.z;

FragColor = color; // set all 4 vector values to 1.0
}