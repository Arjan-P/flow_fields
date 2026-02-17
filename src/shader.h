#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
  public:
    unsigned int ID;

    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

    void use();

    // uniform helpers
    void setBool(const std::string &name, bool value);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);

};
