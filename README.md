# 3D Transformation (With & Without Graphics)

This project demonstrates fundamental 3D geometric transformations on a cube using two C++ programs:

1. `transformation_with_graphics.cpp` – interactive visualizer using OpenGL (FreeGLUT). Shows the original cube (red) and the transformed cube (green) simultaneously.
2. `transformation.cpp` – non‑graphics / console version (pure matrix + coordinate math) useful for quick verification or running where OpenGL is not available.

## 🔧 Supported Transformations
Each transformation is composed into a single 4×4 "Hybrid" matrix (homogeneous coordinates). Supported types (enter the number when prompted):

1. Translate (Tx, Ty, Tz)  
2. Scale (Sx, Sy, Sz)  
3. Rotate X (angle°)  
4. Rotate Y (angle°)  
5. Rotate Z (angle°)  
6. Reflect across X axis (flips Y)  
7. Reflect across Y axis (flips X)  
8. Reflect across Z axis (flips Z)  
9. Shear XY (shx, shy) – X,Y depend on Z  
10. Shear XZ (shx, shz) – X,Z depend on Y  
11. Shear YZ (shy, shz) – Y,Z depend on X  

Composition order matters: the code pre‑multiplies ( `Hybrid = M_new * Hybrid` ), so transformations act in the SAME order you enter them.

## 🧠 Key Concepts
| Concept | Used For |
|--------|----------|
| Homogeneous coordinates (x,y,z,w=1) | Allow translation + linear ops in one matrix |
| 4×4 matrices | Unified representation & composition |
| Pre‑multiplication order | Ensures intuitive sequencing (first entered = first applied) |
| Rotation matrices (right‑handed) | X, Y, Z axis rotations using sin/cos |
| Shear | Slanting object by coupling one axis to another |
| Reflection | Sign inversion across an axis plane |
| Perspective projection (`gluPerspective`) | Gives depth foreshortening (near objects appear bigger) |
| Camera (`gluLookAt`) | Places eye at an oblique position to reveal 3D shape |
| Depth test (`GL_DEPTH_TEST`) | Correct line visibility in 3D |

### Why a Translated Cube Looks Larger
In perspective, moving a cube closer to the eye reduces distance → larger angular size. No scale occurred; it is a visual effect of perspective. To compare true sizes, keep similar distance along the view direction or switch temporarily to orthographic projection.

## 📦 Dependencies
Tested on Windows with MinGW / g++ and FreeGLUT.

You need the following libraries (DLLs in PATH and import libs for linking):
- `freeglut` (or `freeglut.dll` + `libfreeglut.a`)
- System OpenGL: `opengl32`, `glu32`

## ▶️ Build & Run (Graphics Version)
PowerShell in project root:

```
g++ transformation_with_graphics.cpp -o hybrid -lfreeglut -lopengl32 -lglu32
./hybrid
```

Enter the number of transformations, then each transformation type with its parameters. A window opens showing:
- Red wireframe: original cube (unit size centered at origin)
- Green wireframe: transformed cube

Close the window to exit.

### Example Session
```
Enter number of transformations: 2
Choose transformation: ... : 2
2 1 1      # Scale x by 2
Choose transformation: ... : 1
1 2 0 0    # Translate +2 along X
```
Interpretation: First scale, then translate. A vertex (0.5,0.5,0.5) -> scale (1,0.5,0.5) -> translate (3,0.5,0.5).

## 💻 Build & Run (Non‑Graphics Version)
If `transformation.cpp` simply prints transformed coordinates:

```
g++ transformation.cpp -o transform
./transform
```

Use same input format; inspect printed coordinates to verify math.

## ✅ Manual Test Cases
Original cube vertices (v0..v7):
(-0.5,-0.5,-0.5)
( 0.5,-0.5,-0.5)
( 0.5, 0.5,-0.5)
(-0.5, 0.5,-0.5)
(-0.5,-0.5, 0.5)
( 0.5,-0.5, 0.5)
( 0.5, 0.5, 0.5)
(-0.5, 0.5, 0.5)

1. Translation: input `1` then `2 2 2` → every coord + (2,2,2). v0 -> (1.5,1.5,1.5).  
2. Scale: input `2` then `2 1 0.5` → multiply axes. v6 -> (1.0,0.5,0.25).  
3. RotateX 90°: input `3` then `90` → (x, y, z) → (x, -z, y). v4 (-0.5,-0.5,0.5) → (-0.5,-0.5,-0.5).  
4. RotateY 90°: input `4` then `90` → (x, y, z) → (z, y, -x). v5 (0.5,-0.5,0.5) → (0.5,-0.5,-0.5).  
5. RotateZ 90°: input `5` then `90` → (x, y, z) → (-y, x, z). v1 (0.5,-0.5,-0.5) → (0.5,0.5,-0.5).  
6. Composition check: N=2; `1 2 0 0` (translate) then `5 90` (rotateZ) → center (0,0,0) → (2,0,0) → (0,2,0).  

## 🔍 Inspecting / Verifying the Matrix
Print or watch the `Hybrid` matrix after each step (add a loop) to confirm multiplication order. For translation (2,2,2) only the last column (except bottom row) changes.

## 🖥 Camera & Projection Details
```
gluPerspective(60.0, aspect, 0.1, 50.0)
gluLookAt(2.5,2.0,4.0,  0,0,0,  0,1,0)
```
Adjust the first three eye values to change viewpoint, e.g. `(3,3,6)` for a higher angle.

## 🧩 Possible Extensions
- Keyboard controls to rotate the camera (arrow keys)
- Toggle perspective / orthographic
- Save transformed coordinates to a file
- Add faces & simple shading
- Animation of transformation steps

## ❗ Troubleshooting
| Issue | Cause | Fix |
|-------|-------|-----|
| Cube appears larger after translation | Closer to camera in perspective | Move along axis maintaining distance (e.g. +X & -Z), or move camera back |
| Nothing renders | Missing FreeGLUT / wrong link order | Ensure `-lfreeglut -lopengl32 -lglu32` and DLL in PATH |
| Distorted aspect | Window resized | Handled by `reshape`; ensure callback active |

## 📄 License
Educational example; adapt freely.

---
Feel free to ask for an orthographic toggle or keyboard interaction patch.
