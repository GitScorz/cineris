Engine/
 ├── Core/              → Ciclo principal, inicialização, gestão de tempo, logs
 ├── Renderer/          → OpenGL abstractions (shaders, meshes, textures, cameras, lighting)
 ├── Scene/             → Objetos, entidades, transformações, sistema de hierarquia
 ├── ECS/               → Entity-Component-System (opcional, mas recomendado)
 ├── Input/             → Gestão de teclado, rato, gamepad
 ├── Audio/             → Sons 2D e 3D (OpenAL, FMOD, ou SDL_mixer)
 ├── Physics/           → Colisões simples, raycasts, triggers
 ├── Resources/         → Carregamento de assets (texturas, modelos, cenas)
 ├── Scripting/         → Lua/JSON para eventos e lógica de jogo
 ├── UI/                → Interface simples (texto, ícones, menus)
 ├── Game/              → Código específico do teu jogo (ex: lógica do Motel)
 └── main.cpp           → Ponto de entrada


Entity player = scene.CreateEntity("Player");
player.AddComponent<Transform>({0, 0, 0});
player.AddComponent<MeshRenderer>(mesh, material);
player.AddComponent<AudioSource>("breathing.wav");


Renderer/
 ├── Shader.h / .cpp       → Compilar e ligar shaders
 ├── Mesh.h / .cpp         → VBO, VAO, EBO
 ├── Texture.h / .cpp      → Carregamento e binding de texturas
 ├── Camera.h / .cpp       → View + Projection matrices
 ├── Material.h / .cpp     → Uniforms, shaders associados
 └── Renderer.h / .cpp     → Render pipeline (Forward Renderer simples)



Game/
├── MotelScene.cpp
├── MonsterAI.cpp
├── PlayerController.cpp
├── GameManager.cpp