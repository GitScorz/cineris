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

  void update();
  bool shouldClose() const;
  void updateFPS();
private:
  double m_dLastFrameTime = 0.0;
  int m_iFrames = 0;
};