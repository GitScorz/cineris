#pragma once
#include <include.h>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

class Window {
public:
  GLFWwindow* m_Handle;
  int m_iWidth, m_iHeight;
  std::string m_Title;

  Window(int w, int h, const std::string& t);
  ~Window();

  auto update() -> void;
  auto shouldClose() const -> bool;
  auto updateFPS() -> void;
private:
  double m_dLastFrameTime = 0.0;
  int m_iFrames = 0;
};
