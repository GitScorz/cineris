#include "player_controller.h"
#include "game/world/world.h"

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

auto PlayerController::getAABBAt(glm::vec3 vPosition) -> AABB {
  return {
    vPosition + glm::vec3(-0.3f, -0.3f, 0.0f),
    vPosition + glm::vec3( 0.3f,  0.3f, 1.8f)
  };
}

auto PlayerController::update(float fDeltaTime, World* pWorld) -> void {
  glm::vec3 vFront = m_pCamera->getFront();
  glm::vec3 vRight = glm::normalize(glm::cross(vFront, glm::vec3(0.0f, 0.0f, 1.0f)));

  vFront.z = 0.0f;
  vRight.z = 0.0f;

  glm::vec3 vMovement(0.0f);

  if (m_pInputManager->IsControlPressed(GLFW_KEY_W))
    vMovement += vFront * m_fSpeed * fDeltaTime;
  if (m_pInputManager->IsControlPressed(GLFW_KEY_S))
    vMovement -= vFront * m_fSpeed * fDeltaTime;
  if (m_pInputManager->IsControlPressed(GLFW_KEY_A))
    vMovement -= vRight * m_fSpeed * fDeltaTime;
  if (m_pInputManager->IsControlPressed(GLFW_KEY_D))
    vMovement += vRight * m_fSpeed * fDeltaTime;
  if (m_pInputManager->IsControlPressed(GLFW_KEY_E))
    vMovement += glm::vec3(0.0f, 0.0f, 1.0f) * m_fSpeed * fDeltaTime;
  if (m_pInputManager->IsControlPressed(GLFW_KEY_Q))
    vMovement -= glm::vec3(0.0f, 0.0f, 1.0f) * m_fSpeed * fDeltaTime;

  if (glm::length(vMovement) > 0.0f)
    vMovement = glm::normalize(vMovement) * m_fSpeed * fDeltaTime;

  glm::vec3 vNewPosition = m_vPosition + vMovement;

  AABB playerBox = getAABBAt(vNewPosition);

  if (!pWorld->collides(playerBox)) {
    m_vPosition = vNewPosition;
  }

  m_vPosition += m_vVelocity * fDeltaTime;

  m_pCamera->updateCameraPosition(m_vPosition);
}

PlayerController::~PlayerController() {

}
