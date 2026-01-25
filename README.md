# LT Engine v2.0

**LT-Engine** is a custom game engine created by 3 university students as part of the Game Engines course at UPC, **CITM (Centre de la Imatge i la Tecnologia Multimèdia, Barcelona, 2025-2026)**.

The main objective we had with this project was tunderstanding and utilizing the core systems and architecture of real game engines, including resource management, scene handling, rendering, editor tools, and runtime execution.

The engine is implemented mainly in C++, using different implemented libraries such as SDL3, OpenGL, ImGui, and Glad for window management, rendering, input, and editor UI.

<img width="800" height="800" alt="LT Engine" src="https://github.com/user-attachments/assets/40012c27-ed16-4e60-bd78-4efad1c9036e" />



## Developers

| **Member**                                             | **Role**         | **Main Contributions**                                 |
| ------------------------------------------------------ | ---------------- | ------------------------------------------------------ |
| [Mario Martos](https://github.com/MarioPrincesita)     | Programmer       | Editor development, system integration, scene workflow |
| [Didac Sillué](https://github.com/digui048)            | Programmer       | Rendering pipeline, camera system, resource handling   |
| [Ton Casals](https://github.com/Tontito05)             | Programmer       | Input system, engine logic, core components            |

---

## Main and core sub-systems

**Rendering System**

- Built on OpenGL for real-time rendering.
- Supports mesh rendering and basic materials.
- Includes Octree-based Frustum Culling to optimize draw calls and improve performance.

**Scene & Component System**

- Object-oriented architecture based on GameObjects and Components.
- Core components include Transform, Mesh, and Texture.
- Enables modular and scalable scene creation.

**Resource Management**

- Handles loading and management of assets such as FBX models and textures.
- Centralized resource system avoids duplication and improves memory usage.
- Assets imported through the editor can be reused across scenes.

**Editor & Camera System**

- Integrated Editor mode using ImGui.
- Unity-style camera navigation for scene inspection.
- Object selection, hierarchy visualization, and scene editing tools.

---

## Last Assignment Sub-System

The last assignment focused on extending the engine with a foundational UI and rendering system, enabling the creation and visualization of user interface elements both in the editor and at runtime.

A Rect Transform component was implemented to define UI elements using position, size, and anchoring in screen space, providing a flexible layout system independent of the 3D world. Based on this system, new UI components were created, including Image, Button, Text, and Canvas, allowing structured and hierarchical user interface construction.

To support text rendering, a font importer using FreeType was developed. This system loads font files, generates glyph data, and prepares them for efficient rendering within the engine, enabling dynamic and scalable text elements.

Additionally, an orthographic camera was implemented to properly render UI elements in screen space, ensuring consistent size and positioning regardless of perspective or scene depth.

Together, these features establish a complete UI workflow, from asset import and editor setup to in-game rendering, significantly expanding the engine’s capabilities and usability.

### Editor Functionality
![GIF Editor](https://github.com/user-attachments/assets/3d2d3748-a046-4854-a014-496875a36f77)


### In-Game Functionality
![GIF Game](https://github.com/user-attachments/assets/e2b79885-a5c9-435a-82ff-e2dc3a9e6cce)


---

## Video

*CUANDO ESTE EL VIDEO LO PONEMOS AQUI*

---

## LINKS

Code Repository: https://github.com/UPC-GameEngines-BCN-2025/LT-Engine

Lastst release: *PONER ENLACE CUANDO LA HAGAMOS*

---

## License

This project is distributed using the **MIT License**, wich allows free use, modification, and distribution for educational and personal purposes.

Copyright (c) 2026 UPC-GameEngines-BCN-2025, LT-Engine

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

---

## Controls

| Action                     | Control                  |
| -------------------------- | ------------------------ |
| Move Camera                | Right Click              |
| Orbit around object        | Right Click + Shift      |
| Select Object              | Left Click               |
| Move Around                | Right Click + WASD       |
| Focus on Object            | F                        |
| Mouse Pick Obj             | Middle Mouse             |
| Rebuild Octree             | R                        |
| Import / Delete Reasources | Right Click on reasource |
