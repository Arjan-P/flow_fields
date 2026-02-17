#include <cstdlib>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"
#include "compute.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  // update window viewport on resize
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
}

int main()
{
  if(!glfwInit()) {
    std::cerr << "Failed to initialize glfw\n";
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "Flow Fields", NULL, NULL);
  if (window == NULL)
  {
      std::cerr << "Failed to create GLFW window\n";
      glfwTerminate();
      return -1;
  }
  glfwMakeContextCurrent(window);

  // load the driver opengl function implementation pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
  }
  glEnable(GL_PROGRAM_POINT_SIZE);


  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  Shader shader("./shaders/VertexShader.vs", "./shaders/FragmentShader.fs");
  Shader quadShader("./shaders/quadShaders/VertexShader.vs", "./shaders/quadShaders/FragmentShader.fs");
  Compute computeShader("./shaders/ComputeShader.comp");

  const unsigned int PARTICLE_COUNT = 300 *  300;
  
  float particles[PARTICLE_COUNT * 2];
  for(int i = 0; i < PARTICLE_COUNT; i++) {
      particles[i * 2 + 0] = 2.0f * ((float)rand() / RAND_MAX) - 1.0f;
      particles[i * 2 + 1] = 2.0f * ((float)rand() / RAND_MAX) - 1.0f;
      // particles[i * 2 + 0] = 0.0f;
      // particles[i * 2 + 1] = 0.0f;
  }

  unsigned int VAO;
  unsigned int SSBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &SSBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(particles), particles, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);

  glBindBuffer(GL_ARRAY_BUFFER, SSBO);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
  glEnableVertexAttribArray(0);

  // unbind VAO
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  unsigned int fullscreenQuadVAO, fullscreenQuadVBO;
  float quad[] = {
    1.0f, -1.0f,
    1.0f, 1.0f,
    -1.0f, 1.0f,
    -1.0f, -1.0f
  };

  glGenVertexArrays(1, &fullscreenQuadVAO);
  glGenBuffers(1, &fullscreenQuadVBO);

  glBindVertexArray(fullscreenQuadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, fullscreenQuadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while(!glfwWindowShouldClose(window)) {
    processInput(window);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // fade previous frame
    quadShader.use();
    quadShader.setFloat("alpha", 0.09f);
    glBindVertexArray(fullscreenQuadVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT);


    // run compute shader
    static float lastTime = glfwGetTime();
    float currentTime = glfwGetTime();
    float dt = currentTime - lastTime;
    lastTime = currentTime;

    computeShader.use();
    computeShader.setFloat("time", currentTime);
    computeShader.setFloat("dt", dt);
    computeShader.setUInt("count", PARTICLE_COUNT);

    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);
    glDispatchCompute((PARTICLE_COUNT + 255) / 256, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

    // render pipeline shaders
    shader.use();
    shader.setFloat("time", currentTime);

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
