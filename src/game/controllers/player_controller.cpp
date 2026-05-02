#include "player_controller.h"

PlayerController::PlayerController(Camera *pCamera, InputManager *pInputManager)
    : m_pCamera(pCamera), 
    m_pInputManager(pInputManager), 
    m_vPosition(0.0f, 0.0f, 0.0f), 
    m_fSpeed(5.0f)
{
  // Mesh *playerMesh = Mesh::createCube();
  // Shader *playerShader = new Shader("cube");
  // GameObject* playerObject = new GameObject(playerMesh, playerShader, glm::vec3(0.0f, 0.5f, 0.0f));

}

void PlayerController::update(float fDeltaTime) {
  glm::vec3 vFront = m_pCamera->getFront();
  glm::vec3 vRight = glm::normalize(glm::cross(vFront, glm::vec3(0.0f, 1.0f, 0.0f)));

  vFront.y = 0.0f;
  vRight.y = 0.0f;

  if (m_pInputManager->IsControlPressed(GLFW_KEY_W))
    m_vPosition += vFront * m_fSpeed * fDeltaTime;
  if (m_pInputManager->IsControlPressed(GLFW_KEY_S))
    m_vPosition -= vFront * m_fSpeed * fDeltaTime;
  if (m_pInputManager->IsControlPressed(GLFW_KEY_A))
    m_vPosition -= vRight * m_fSpeed * fDeltaTime;
  if (m_pInputManager->IsControlPressed(GLFW_KEY_D))
    m_vPosition += vRight * m_fSpeed * fDeltaTime;

  m_vPosition += m_vVelocity * fDeltaTime;

  m_pCamera->updateCameraPosition(m_vPosition);
}

PlayerController::~PlayerController() {

}
