#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

class Compute
{
  public:
    unsigned int ID;

    Compute(const char* computeShaderPath);

    void use();

    // uniform helpers
    void setBool(const std::string &name, bool value);
    void setUInt(const std::string &name, unsigned int value);
    void setFloat(const std::string &name, float value);

};
