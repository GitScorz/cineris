# Cineris
Cineris is the name of the game engine I’m developing for Ashmoor Case. It’s a custom-built engine designed to give me full control over the rendering pipeline, game architecture, and overall development process. Cineris means “ashes” in Latin, for me, the name represents the idea of building something new from previous experiences


> **Recent rewrite:** I've been rewriting Cineris recently to give the engine a cleaner foundation, separate it from the game, and focus on graphics development. The rewrite currently targets Windows and Visual Studio.

# Ashmoor Case

Ashmoor Case is my first game development project. A personal journey into the world of game creation.
I’ve always wanted to learn how games are made, and this project marks the beginning of that dream.

Inspired by one of my favorite films, Shutter Island, Ashmoor Case will explore mystery, psychological tension, and atmosphere.


# About the Project

This project is both a learning experience and the foundation for a full game.
I’m building everything from scratch using C++, OpenGL, and GLFW, to understand how rendering, shaders, and game architecture truly work.

The goal of The Ashmoor Case is to create a dark, grounded world that combines a low-poly visual style with a sense of realism. The focus is on mood, lighting, and subtle environmental storytelling.

## References
If the description above doesn’t fully capture the mood, here are some visual references I generated to explore the atmosphere in more detail.

> Visual inspiration only — these are not in-game screenshots or final previews.

| Ambient 1 | Ambient 2 | Main Character |
|---|---|---|
| ![ASHMOOR REFERENCE 1](Resources/references/image.png) | ![ASHMOOR REFERENCE 2](Resources/references/image2.png) | ![ASHMOOR REFERENCE 3](Resources/references/image3.png) |

> Generated with ChatGPT and used only as visual inspiration.

I plan to create some of the textures, models and also music for this game.

## Setup

The rewrite currently supports **Windows x64**. To build and run it, install:

- **Visual Studio 2026** with the **Desktop development with C++** workload, the **MSVC v145** toolset and a **Windows SDK**.
- A graphics driver with **OpenGL 4.6** support.
- Git, if you want to clone the repository.

The required third-party libraries are included in `Dependencies/`. **CMake and vcpkg are not required** to build the engine or Sandbox.

### Build and run

1. Clone or download the repository.
2. Open `Cineris.slnx` in Visual Studio.
3. Select **Debug | x64** or **Release | x64**.
4. In Solution Explorer, right-click **Sandbox** and choose **Set as Startup Project**.
5. Press **F5** to build and run.

`Cineris` builds as a static library (`Cineris.lib`). `Sandbox` is the executable that uses it to test the engine's graphics.

The build automatically copies the required DLLs, shaders and demo assets beside the executable:

```text
bin/x64/Debug/Sandbox.exe
bin/x64/Release/Sandbox.exe
```

## License

This project is available for personal and educational use under the Cineris Non-Commercial License.

You may study, modify, and share the project for non-commercial purposes.

You may not sell this project, sell modified versions, include it in paid products, or otherwise use it commercially without explicit permission from the author.

<br>

---

#### June 15 2026

I’m not completely happy with how this scene looks yet. The basic layout, textures, water, and lighting are in place, but the screenshot still feels darker and less atmospheric than what I have in mind. 
It’s a start, but I’ll need to keep working on the lighting, composition, and overall mood until it feels closer to the vision for Ashmoor Case.
![First screenshot lol](Journey/june_15_2026.png)
