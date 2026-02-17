#version 440 core
layout (location = 0) in vec2 aPos;
uniform float time;

void main()
{
   gl_Position = vec4(aPos,0.0, 1.0);
   gl_PointSize = 4.0 + sin(time);
};
