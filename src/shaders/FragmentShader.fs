#version 440 core
out vec4 FragColor;
uniform float time;
void main()
{
    //vec3 color = (1 + sin(time)) * vec3(1.0, 1.0, 0.0);
    FragColor = vec4(1.0,1.0,1.0, 1.0);
}
