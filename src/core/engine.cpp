#include "engine.h"
#include "debug.h"

Engine::Engine() {
  m_pWindow = new Window(1200, 800, "The Ashmoor Case");

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD." << std::endl;
    glfwTerminate();
    exit(-1);
  }

  m_pCamera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
  m_pInputManager = new InputManager(m_pWindow->m_Handle);
  m_pWorld = new World();
  m_pPlayer = new Player(m_pCamera, m_pInputManager);
}

Engine::~Engine() {
  delete m_pWindow;
  delete m_pCamera;
  delete m_pInputManager;
  delete m_pWorld;
}

void Engine::run() {
  // just a test for now
  Shader *basicShader = new Shader("../shaders/vertex.glsl", "../shaders/fragment.glsl");
  Mesh *cubeMesh = Mesh::createCube();

  m_pWorld->addObject(new GameObject(cubeMesh, basicShader, glm::vec3(0.0f, 0.0f, -2.0f)));

  Debug::registerDebugBindings(m_pInputManager);
  
  while (!m_pWindow->shouldClose()) {
    m_dDeltaTime = calculateDeltaTime();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = m_pCamera->getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(70.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

    basicShader->use();
    basicShader->setMat4("view", view);
    basicShader->setMat4("projection", projection);

    m_pCamera->rotate(
      m_pInputManager->getMouseDeltaX() * m_pInputManager->m_fMouseSensitivity, 
      m_pInputManager->getMouseDeltaY() * m_pInputManager->m_fMouseSensitivity
    );

    m_pInputManager->processInput();
    m_pWorld->draw();
    m_pPlayer->update(m_dDeltaTime);
    m_pWindow->update();
  }

  delete basicShader;
  delete cubeMesh;
  delete m_pWorld;
}