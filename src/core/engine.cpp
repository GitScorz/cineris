#include "engine.h"
#include "../game/world.h"

Engine::Engine() {
  m_pWindow = new Window(800, 600, "The Ashmoor Case");

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD." << std::endl;
    glfwTerminate();
    exit(-1);
  }

  m_pCamera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
}

Engine::~Engine() {
  delete m_pWindow;
  delete m_pCamera;
}

void Engine::run() {
  Shader* basicShader = new Shader("../shaders/vertex.glsl", "../shaders/fragment.glsl");

  // just a test for now
  Mesh* cubeMesh = Mesh::createCube();

  World* pWorld = new World();
  pWorld->addObject(new GameObject(cubeMesh, basicShader, glm::vec3(0.0f, 0.0f, -2.0f)));
  
  while (!m_pWindow->shouldClose()) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = m_pCamera->getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(70.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

    basicShader->use();
    basicShader->setMat4("view", view);
    basicShader->setMat4("projection", projection);

    pWorld->draw();
    m_pWindow->update();
  }

  delete basicShader;
  delete cubeMesh;
  delete pWorld;
}