# Real-Time Rendering Engine using OpenGL and C++
## Overview
This project is a real-time rendering engine built using OpenGL and C++ capable of importing and rendering geometry. Geometry, shaders and light sources can be changed using a built-in GUI written in ImGui.

The engine is capable of rendering complex geometry and simulate lighting on the surfaces.
<img src="images/Cow.png?raw=true" width="500" />

## Interface
- Select models and textures can be selected from the Menu.
<img src="images/GUI.png?raw=true" width="500" />

- The number, position, colour and orbit for the lightsource/s can be adjusted in the Lights menu.
<img src="images/TwoLights.png?raw=true" width="500" />

- Phong and Blinn-Phong shaders can be switched between using the GUI. The Ambient, Diffuse, and Specular components can be adjusted using the sliders.

Ambient                                            |  Diffuse                                           | Specular                                            | Total
:-------------------------------------------------:|:--------------------------------------------------:|:---------------------------------------------------:|:---------------------------------------------
![PhongAmbient](images/PhongAmbient.png?raw=true)  |  ![PhongDiffuse](images/PhongDiffuse.png?raw=true) | ![PhongSpecular](images/PhongSpecular.png?raw=true) | ![PhongTotal](images/PhongTotal.png?raw=true)
