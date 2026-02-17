#include "compute.h"
#include <string>

Compute::Compute(const char* computeShaderPath) {
  std::string computeCode;
  std::ifstream cShaderFile;

  cShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

  try {
    // open file
    cShaderFile.open(computeShaderPath);

    std::stringstream vShaderStream, fShaderStream, cShaderStream;
    // read buffers into string streams
    cShaderStream << cShaderFile.rdbuf();
     
    // close fd
    cShaderFile.close();

    computeCode = cShaderStream.str();
  }
  catch(std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }

  const char* cShaderCode = computeCode.c_str();
  std::cout << cShaderCode << std::endl;

  // compile shaders

  unsigned int computeShader;
  int success;
  char infolog[512];

  computeShader = glCreateShader(GL_COMPUTE_SHADER);
  glShaderSource(computeShader, 1, &cShaderCode, NULL);
  glCompileShader(computeShader);
  glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(computeShader, 512, NULL, infolog);
    std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infolog << std::endl;
  }

  ID = glCreateProgram();
  glAttachShader(ID, computeShader);
  glLinkProgram(ID);
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if(!success)
  {
      glGetProgramInfoLog(ID, 512, NULL, infolog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infolog << std::endl;
  }

  glDeleteShader(computeShader);
}

void Compute::use() {
  glUseProgram(ID);
}

void Compute::setBool(const std::string &name, bool value) {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Compute::setUInt(const std::string &name, unsigned int value) {
  glUniform1ui(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Compute::setFloat(const std::string &name, float value) {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), (float)value);
}
