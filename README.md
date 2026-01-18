# PrismCore
### A complete 3D engine built using SDL.

The 'structures.h' header is independent from everything else, so it can be used as a library. <br>
Both hardwareRender.h and softwareRender.h both need SDL to function properly.

- "structures.h" defines all the 3D and 2D structs which can be used for anything. <br>
- "hardwareRender.h" defines rendering functions using openGL (3.3). Dependent on SDL <br>
- "softwareRender.h" defines rendering functions using SDL's built in renderer. Dependent on SDL <br>

Fully supports mesh and wireframe rendering (and dots rendering in openGL mode).

### Examples <br>
- Full Mesh rendering <br>
<img width="600" height="630" alt="Full Mesh Rendering" src="https://github.com/Alex-Nel/SDRender/blob/main/images/Full-Mesh-Multiple-Objects.PNG" />

- Wireframe rendering <br>
<img width="600" height="630" alt="Wireframe penger" src="https://github.com/user-attachments/assets/2e13656d-497b-44f9-af8c-732fc61fc2b7" />

- Another Wireframe rendering <br>
<img width="600" height="630" alt="Wireframe cube" src="https://github.com/user-attachments/assets/65377f91-470f-4905-83fe-065775556a08" />
