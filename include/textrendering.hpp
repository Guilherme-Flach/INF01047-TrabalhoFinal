#ifndef TEXTRENDERING_HEADER
#define TEXTRENDERING_HEADER

#include <GLFW/glfw3.h>
#include <string>
#include <glm/vec4.hpp>

void TextRendering_Init();

void TextRendering_ShowFramesPerSecond(GLFWwindow *window);

void TextRendering_PrintString(GLFWwindow *window, const std::string &str,
                               float x, float y, float scale = 1.0f);

void TextRendering_PrintVector(GLFWwindow *window, glm::vec4 v, float x,
                               float y, float scale = 1.0f);

#endif