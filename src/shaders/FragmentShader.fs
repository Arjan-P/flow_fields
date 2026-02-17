#version 440 core

out vec4 FragColor;
uniform float time;

void main()
{
    vec2 uv = gl_PointCoord * 2.0 - 1.0;  // map to [-1,1]
    float r = length(uv);
    if (r > 1.0)
        discard;
    FragColor = vec4(0, 0, 0, 1.0);
}
