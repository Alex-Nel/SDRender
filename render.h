#ifndef RENDER_H
#define RENDER_H

#include "structures.h"
#include "SDL3/SDL.h"


// // Global variables
// RenderTriangle* triangleBuffer;
// int triCount = 0;



//////////////////////
// Rendering functions
//////////////////////


// Renders a screenpoint at the position specified
void RenderPoint(SDL_Renderer* renderer, ScreenPoint p);



// Renders a line between two points
void DrawLine(SDL_Renderer* renderer, ScreenPoint p1, ScreenPoint p2, WindowInfo wi, Color color);



// Convert 2D vector to a point on a screen
// Changes a vector to a pixel position
ScreenPoint Screen(Vector2 p, WindowInfo wi);



// Converts a 3D point to a 2d Vector on a screen
// x' = x / z
// y' = y / z
// Within the range [-1 to 1]
// Vector2 Project(Transform* obj, Point p)
// {
//     Vector2 v = {
//         (obj->x + p.x) / (obj->z + p.z),
//         (obj->y + p.y) / (obj->z + p.z)
//     };
//     // Vector2 v = {p.x / p.z, p.y / p.z};
//     return v;
// }


Vector2 Project(Camera* cam, Transform* obj, Vector3 p);

void Render(SDL_Renderer* renderer, WindowInfo program, Camera* cam, Object* obj);

int ClipTriangleAgainstNearPlane(Vector3 inV[3], Vector3 outTris[2][3]);
void AddRenderTriangles(Object* GlobalObjects, int numObjects, Camera* cam, Vector3 lightDirCamera);
void RenderTriangles(SDL_Renderer* renderer, WindowInfo program);





//////////////////////
// Rotation functions
//////////////////////


void rotate_xz(Mesh* obj, Vector3* p, float angle);

void rotate_xy(Mesh* obj, Vector3* p, float angle);

void rotate_yz(Mesh* obj, Vector3* p, float angle);


void translateObjectX(Object* obj, float x);

void translateObjectY(Object* obj, float y);

void translateObjectZ(Object* obj, float z);



void MoveCameraForward(Camera* cam, float amount);


#endif