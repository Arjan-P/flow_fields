#include "shader.h"
#include <string>

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
  try {
    // open file
    vShaderFile.open(vertexShaderPath);
    fShaderFile.open(fragmentShaderPath);

    std::stringstream vShaderStream, fShaderStream;
    // read buffers into string streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
     
    // close fd
    vShaderFile.close();
    fShaderFile.close();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  }
  catch(std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl; 
  }

  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();

  // compile shaders

  unsigned int vertexShader, fragmentShader;
  int success;
  char infolog[512];
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vShaderCode, NULL);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success)
  {
      glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
  };

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
  glCompileShader(fragmentShader);
  if(!success)
  {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << std::endl;
  };

  ID = glCreateProgram();
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glLinkProgram(ID);
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if(!success)
  {
      glGetProgramInfoLog(ID, 512, NULL, infolog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infolog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::use() {
  glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setFloat(const std::string &name, float value) {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), (float)value);
}
