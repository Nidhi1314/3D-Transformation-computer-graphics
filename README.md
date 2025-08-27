# 3D Transformation (Graphics & Non‑Graphics)

Two C++ programs show basic 3D transformations on a cube:
1. `transformation_with_graphics.cpp` – OpenGL visual (red = original, green = transformed).
2. `transformation.cpp` – console only (math / matrix output).

## Core Idea: Hybrid Transformation Matrix
All operations are expressed as 4×4 matrices in homogeneous coordinates (x,y,z,w=1). Each time you choose a transformation its matrix M is pre‑multiplied:

Hybrid = M_new * Hybrid

So the order you enter is the order applied to the cube points.

Why homogeneous coordinates? They let translation, rotation, scaling, shear, reflection all combine into one matrix so you only multiply each point once at the end.

## Supported Transformations (numbers you enter)
1 Translate (Tx Ty Tz)
2 Scale (Sx Sy Sz)
3 RotateX (angle°)
4 RotateY (angle°)
5 RotateZ (angle°)
6 ReflectX
7 ReflectY
8 ReflectZ
9 ShearXY (shx shy)
10 ShearXZ (shx shz)
11 ShearYZ (shy shz)

## Build & Run (Graphics)
PowerShell:
```
g++ transformation_with_graphics.cpp -o hybrid -lfreeglut -lopengl32 -lglu32
./hybrid
```
Enter how many transformations, then each type + its parameters. Window shows both cubes.

## Build & Run (Console)
```
g++ transformation.cpp -o transform
./transform
```
Same input format; inspect printed coordinates/matrix.

## Quick Manual Checks
Original vertex example: v0 = (-0.5,-0.5,-0.5)

Translation (1 2 2 2): v0 -> (1.5,1.5,1.5)
Scale (2 2 1 0.5): v0 -> (-1.0,-0.5,-0.25)
RotateZ 90° (5 90): (x,y,z)->(-y,x,z); v1 (0.5,-0.5,-0.5)->(0.5,0.5,-0.5)

Composition example (Scale then Translate):
N=2
2 2 1 1   (scale x by 2)
1 2 0 0   (translate +2 x)
Point (0.5,0.5,0.5) -> after scale (1,0.5,0.5) -> after translate (3,0.5,0.5)

## Minimal Code Flow (graphics version)
1. Build identity Hybrid.
2. Read user choices; accumulate Hybrid.
3. Render: draw original; apply Hybrid to each vertex; draw transformed.

That’s it—focus is on how multiple 4×4 matrices combine into one (the Hybrid) via pre‑multiplication.
