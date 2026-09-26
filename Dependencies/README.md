# Bundled dependencies

These dependencies are local to Cineris. Engine and Sandbox builds need only
Visual Studio 2026, C++ desktop tools, and a Windows SDK; there is no dependency
on another checkout or an absolute developer-machine path.

| Dependency | Source / version | Integration |
|---|---|---|
| Assimp | 6.0.5 development snapshot, upstream commit `f3e5f003b36b11d33b2abff40797fcac73b07892` | x64 DLL and import library per configuration, MSVC v145 |
| GLFW | Official tag `3.4` | x64 static library per configuration, MSVC v145 |
| GLAD | Existing generated OpenGL loader | Compiled into Cineris |
| GLM | Headers preserved from the original local project | Header-only |
| Dear ImGui | Sources preserved from the original local project | Compiled into Cineris with GLFW/OpenGL backends |
| stb_image | Sources preserved from the original local project | Compiled into Cineris |

Debug uses the debug DLL C runtime (`/MDd`); Release uses `/MD`.
Assimp is built with its default importers and without exporters, tools or tests.
Its headers and generated configuration match the bundled binaries. Its source
snapshot came from the existing local Assimp checkout; that checkout had a
pre-existing modification to `contrib/zlib/zconf.h.included`.

For rebuilding third-party binaries only, Assimp's CMake options were:
`ASSIMP_BUILD_TESTS=OFF`, `ASSIMP_BUILD_ASSIMP_TOOLS=OFF`, `ASSIMP_INSTALL=OFF`,
`ASSIMP_WARNINGS_AS_ERRORS=OFF`, `ASSIMP_BUILD_ZLIB=ON`, `ASSIMP_NO_EXPORT=ON`,
`ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT=ON`. GLFW used
`GLFW_BUILD_EXAMPLES=OFF`, `GLFW_BUILD_TESTS=OFF`, `GLFW_BUILD_DOCS=OFF`,
`GLFW_INSTALL=OFF`, `USE_MSVC_RUNTIME_LIBRARY_DLL=ON`.
Both were built with the Visual Studio 18 2026 generator for x64.

Libraries live in `lib/Debug` and `lib/Release`; Assimp DLLs live in
`bin/Debug` and `bin/Release`. The application's build copies its matching DLL.

Upstream sources: https://github.com/assimp/assimp and https://github.com/glfw/glfw.
Third-party copyright notices in the supplied sources remain in force;
additional licenses are in `licenses`.
