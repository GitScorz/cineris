#pragma once
#include <include.h>
#include <string>
#include <iostream>

class Window {
public:
  GLFWwindow* m_Handle;
  int m_Width, m_Height;
  std::string m_Title;

  Window(int w, int h, const std::string& t);
  ~Window();

  void update();
  bool shouldClose() const;
};