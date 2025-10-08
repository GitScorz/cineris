Vamos montar isto bem:
vou-te dar o conceito de jogo, a mecânica central, o estilo visual (PS2-like) e depois uma estrutura técnica que podes começar a implementar com C++ + OpenGL.

🎭 Conceito de jogo — “The Ashmoor Case” (inspirado em Shutter Island)

Resumo:
Jogas como um detetive que chega a uma ilha remota onde existe um hospital psiquiátrico isolado após um paciente desaparecer.
Mas à medida que exploras, começas a questionar o que é real — e quem realmente és.

Tema central:
A tua mente está a criar o caso.
Cada descoberta muda o ambiente e altera a tua perceção do mundo (paredes movem-se, sombras piscam, sons contradizem o que vês).

🧠 Mecânica principal
1. Sanidade Dinâmica

A “barra de sanidade” controla o estado do mundo.

Quanto mais baixa → o ambiente distorce-se, NPCs mudam de rosto, sons tornam-se ruído.

Isto pode ser feito com post-processing shaders (distorção, blur, chromatic aberration) + eventos de script que trocam assets.

2. Interrogatório e Exploração

Falas com pacientes ou funcionários (estilo dialog tree simples).

As tuas escolhas alteram o mundo (podes usar uma estrutura de “flags” ou variáveis no engine).

3. Flashbacks interativos

Entras em memórias que têm regras próprias (gravidade alterada, sons invertidos, luz fria).

Podes simular isto carregando “mini-scenes” com diferentes shaders e iluminação.

🎨 Estilo visual “PS2-like”

Resolução interna baixa (480p upscale) → dá aquele look PS2 autêntico.

Texturas 256x256/512x512 no máximo.

Vertex lighting (nada de PBR).

Fog baseado na distância para esconder clipping e dar atmosfera.

Paleta fria (tons cinzentos, verdes, azuis escuros).

Post effects: dithering, film grain, e vinheta suave.

Som ambiente com low-pass filter quando estás em salas fechadas.

🧱 Estrutura técnica da tua engine (OpenGL + C++)

1. Core (Base Engine)

Window manager (GLFW)

Renderer (OpenGL)

Input handler

Timer / Delta time

Logger (para debug)

2. Renderer

Shader loader

Camera system (FPS ou terceira pessoa)

Fog, Lighting, Skybox

Framebuffers (para pós-processamento)

3. Scene system

Scene → lista de entidades + scripts

Entity → posição, modelo, comportamento

Component system (ECS simplificado)

4. Audio

OpenAL ou FMOD wrapper

3D positional audio (para sons no espaço)

5. Gameplay layer

Script manager (JSON/Lua)

Dialogue manager

Sanity system

Event triggers (colisões, luzes, sons, etc.)

🎬 Estrutura de Jogo

Capítulo 1 – Chegada à ilha
Aprendes a mexer na lanterna, a explorar, a ouvir vozes ao longe.
Luz e som definem o ritmo.

Capítulo 2 – O paciente desaparecido
A ilha começa a mudar, e percebes que o mapa se “reconfigura” (podes randomizar certos layouts).

Capítulo 3 – Verdade fragmentada
Entras em memórias — com distorções visuais e sons de ECG, chuva, gritos.

Capítulo 4 – A revelação
Descobres que eras o paciente o tempo todo. O loop reinicia.

🔧 O que podes praticar tecnicamente:

Fog + post-processing shader (para o ambiente).

Sistema de diálogos em JSON.

Gestão de múltiplas cenas (ilha, hospital, memórias).

Iluminação dinâmica simples (Phong).

Audio 3D + sanidade como filtro no som.

Gestão de recursos (textures, models, shaders).

Se quiseres, posso montar-te um documento de design técnico (GDD + engine roadmap) para este jogo The Ashmoor Case — com a lista de sistemas que deves programar e a ordem recomendada para aprender tudo (ex: primeiro renderer, depois camera, depois lighting…).

❓ Queres que te monte esse roadmap completo com base nesta ideia “Shutter Island style”?