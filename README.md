# Real-Time Rendering Engine using OpenGL and C++
## Overview
This project is a real-time rendering engine built using OpenGL and C++ capable of importing and rendering geometry. Geometry, shaders and light sources can be changed using a built-in GUI written in ImGui.

The engine is capable of rendering complex geometry and simulate lighting on the surfaces.
![Cow](images/Cow.png?raw=true)

## Interface
- Select models and textures can be selected from the Menu.
![GUI](images/GUI.png?raw=true)

- The number, position, colour and orbit for the lightsource/s can be adjusted in the Lights menu.

- Phong and Blinn-Phong shaders can be switched between using the GUI. The Ambient, Diffuse, and Specular components can be adjusted using the sliders.


| Phong Ambient | Phong Diffuse | Phong Specular | Phong Total |
|---------------|---------------|----------------|-------------|
|![PhongAmbient](images/PhongAmbient.png?raw=true) | ![PhongDiffuse](images/PhongDiffuse.png?raw=true) | ![PhongSpecular](images/PhongSpecular.png?raw=true) | ![PhongTotal](images/PhongTotal.png?raw=true)|
