#include "engine.h"
#include "debug.h"
#include "resource_manager.h"
#include "game/world/level_loader.h"

Cineris::Cineris(const std::string& title) {
  Debug::setupDebugConsole();

  m_pWindow = new Window(1920, 1080, title.c_str());

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD." << std::endl;
    glfwTerminate();
    exit(-1);
  }

  // Enable depth testing so terrain/objects render with correct occlusion
  glEnable(GL_DEPTH_TEST);

  m_pCamera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f));
  m_pInputManager = new InputManager(m_pWindow->m_Handle);
  m_pWorld = new World();
  m_pPlayer = new PlayerController(m_pCamera, m_pInputManager);
}

Cineris::~Cineris() {
  delete m_pWindow;
  delete m_pCamera;
  delete m_pInputManager;
  delete m_pWorld;
  delete m_pPlayer;
}

auto Cineris::run() -> void {
  float windowWidth = m_pWindow->m_iWidth;
  float windowHeight = m_pWindow->m_iHeight;

  Mesh *playerMesh = ResourceManager::get().getCubeMesh();
  Shader *playerShader = ResourceManager::get().getShader("lightning");
  Texture *playerTexture = ResourceManager::get().getTexture("black.png");
  WorldObject* playerObject = new WorldObject(playerMesh, playerShader, playerTexture, glm::vec3(0.0f, 0.0f, 0.0f));
  playerObject->setObjectColor(glm::vec3(1.0f, 0.5f, 0.31f));

  Debug::registerDebugBindings(m_pInputManager);

  // std::vector<std::string> level = {
  //   "#########################",
  //   "#...........#...........#",
  //   "#...........#...........#",
  //   "#.....L.....#...........#",
  //   "#...........#...........#",
  //   "#..P........#######D#####",
  //   "#.......................#",
  //   "#.................L.....#",
  //   "#########################"
  // };
  
  auto level = LevelLoader::load("ashmoor_entrance");
  m_pWorld->loadLevel(m_pPlayer, level);
  
  while (!m_pWindow->shouldClose()) {
    m_dDeltaTime = calculateDeltaTime();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = m_pCamera->getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(70.0f), windowWidth / windowHeight, 0.1f, 100.0f);

    RenderContext context = { 
      view, 
      projection, 
      m_pCamera->getPosition(),
      m_pWorld->getLightPositions(),
      glm::vec3(1.0f, 1.0f, 1.0f),
    };

    // oceanShader->use();
    // oceanShader->setMat4("view", view);
    // oceanShader->setMat4("projection", projection);
    // oceanShader->setVec3("lightDir", glm::normalize(glm::vec3(-0.5f, -1.0f, -0.3f)));
    // oceanShader->setFloat("time", (float)glfwGetTime());
    // oceanShader->setFloat("waveHeight", fWaveHeight);
    // oceanShader->setFloat("waveFrequency", fWaveFrequency);    

    playerObject->setPosition(m_pPlayer->getPosition());
    m_pPlayer->update(m_dDeltaTime, m_pWorld);
    playerObject->draw(context);

    m_pCamera->rotate(
      m_pInputManager->getMouseDeltaX() * m_pInputManager->m_fMouseSensitivity,
      m_pInputManager->getMouseDeltaY() * m_pInputManager->m_fMouseSensitivity
    );

    m_pInputManager->processInput();
    m_pWorld->draw(context);
    m_pWindow->update();
    m_pWindow->updateFPS();
  }

  delete playerObject;
  m_pWorld->clearObjects();
}
