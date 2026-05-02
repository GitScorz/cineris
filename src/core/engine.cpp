#include "engine.h"
#include "debug.h"

Engine::Engine() {
  m_pWindow = new Window(1400, 800, "The Ashmoor Case");

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD." << std::endl;
    glfwTerminate();
    exit(-1);
  }

  // Enable depth testing so terrain/objects render with correct occlusion
  glEnable(GL_DEPTH_TEST);

  m_pCamera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
  m_pInputManager = new InputManager(m_pWindow->m_Handle);
  m_pWorld = new World();
  // m_pPlayer = new PlayerController(m_pCamera, m_pInputManager);
}

Engine::~Engine() {
  delete m_pWindow;
  delete m_pCamera;
  delete m_pInputManager;
  delete m_pWorld;
}

void Engine::run() {
  float fWaveHeight = 0.2f;
  float fWaveFrequency = 2.0f;

  // Mesh *oceanMesh = Mesh::createQuad(100.0f, 100.0f);
  // Shader *oceanShader = new Shader("ocean");
  // m_pWorld->addObject(new GameObject(oceanMesh, oceanShader, glm::vec3(0.0f, 0.0f, 0.0f)));

  // Mesh *playerMesh = Mesh::createCube();
  // Shader *playerShader = new Shader("cube");
  // GameObject* playerObject = new GameObject(playerMesh, playerShader, glm::vec3(0.0f, 0.5f, 0.0f));

  Debug::registerDebugBindings(m_pInputManager);
  
  while (!m_pWindow->shouldClose()) {
    m_dDeltaTime = calculateDeltaTime();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = m_pCamera->getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(70.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

    // oceanShader->use();
    // oceanShader->setMat4("view", view);
    // oceanShader->setMat4("projection", projection);
    // oceanShader->setVec3("lightDir", glm::normalize(glm::vec3(-0.5f, -1.0f, -0.3f)));
    // oceanShader->setFloat("time", (float)glfwGetTime());
    // oceanShader->setFloat("waveHeight", fWaveHeight);
    // oceanShader->setFloat("waveFrequency", fWaveFrequency);

    // playerShader->use();
    // playerShader->setMat4("view", view);
    // playerShader->setMat4("projection", projection);
    // playerObject->setPosition(m_pPlayer->getPosition());
    // playerObject->draw();

    m_pCamera->rotate(
      m_pInputManager->getMouseDeltaX() * m_pInputManager->m_fMouseSensitivity, 
      m_pInputManager->getMouseDeltaY() * m_pInputManager->m_fMouseSensitivity
    );

    m_pInputManager->processInput();
    m_pWorld->draw();
    // m_pPlayer->update(m_dDeltaTime);
    m_pWindow->update();
    m_pWindow->updateFPS();
  }

  m_pWorld->clearObjects();
}