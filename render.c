#include <math.h>
#include "structures.h"
#include "render.h"
#include "SDL3/SDL.h"

//////////////////////
// Rendering functions
//////////////////////


// Renders a screenpoint at the position specified
void RenderPoint(SDL_Renderer* renderer, ScreenPoint p)
{
    if (p.x == NAN && p.y == NAN)
        return;

    const float s = 5.0;
    SDL_SetRenderDrawColor(renderer, 255, 0, 80, 255);
    SDL_FRect rect = {p.x - s/2, p.y - s/2, s, s};
    SDL_RenderFillRect(renderer, &rect);
}



// Renders a line between two points
void DrawLine(SDL_Renderer* renderer, ScreenPoint p1, ScreenPoint p2, WindowInfo wi)
{
    // skip if either point is invalid
    if (p1.x != p1.x || p1.y != p1.y || p2.x != p2.x || p2.y != p2.y)
        return;
        
    if ((p1.x < 0 && p2.x < 0) || (p1.x >= wi.width && p2.x >= wi.width) ||
        (p1.y < 0 && p2.y < 0) || (p1.y >= wi.height && p2.y >= wi.height))
        return;

    SDL_SetRenderDrawColor(renderer, 255, 0, 80, 255);
    SDL_RenderLine(renderer, p1.x, p1.y, p2.x, p2.y);
}



// Convert 2D vector to a point on a screen
// Changes a vector to a pixel position
ScreenPoint Screen(Vector2 p, WindowInfo wi)
{
    // if (p.x == NAN && p.y == NAN)
    //     return (ScreenPoint){NAN, NAN};

    if (p.x != p.x || p.y != p.y)
        return (ScreenPoint){NAN, NAN};

    // Find aspect ratio
    float aspectRatio;
    if (wi.width < wi.height)
        aspectRatio = wi.width / 2.0f;
    else
        aspectRatio = wi.height / 2.0f;
    
    /* Old Formula for converting points*/
    // ScreenPoint sp = {
    //     (((p.x + 1)/2)*wi.width),
    //     ((1-((p.y + 1)/2))*wi.height)
    // };

    /* New Formula for converting points (Accounts for aspect ratio) */
    ScreenPoint sp = {
        (p.x * aspectRatio + wi.width/2.0f),
        (-p.y * aspectRatio + wi.height/2.0f)  // negative because y axis flips in SDL
    };
    return sp;
}



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


Vector2 Project(Camera* cam, Transform* obj, Point p)
{
    // World position
    Vector3 world = {
        obj->position.x + p.x,
        obj->position.y + p.y,
        obj->position.z + p.z
    };

    // Camera-relative position (view space)
    Vector3 rel = {
        world.x - cam->transform.position.x,
        world.y - cam->transform.position.y,
        world.z - cam->transform.position.z
    };

    Quaternion inv = {
        -cam->rotation.x,
        -cam->rotation.y,
        -cam->rotation.z,
        cam->rotation.w 
    };
    
    Vector3 view = RotateVecByQuat(rel, inv);

    if (view.z <= 0.00001f)
        return (Vector2){NAN, NAN};

    Vector2 v = {
        view.x / view.z,
        view.y / view.z
    };
    // Vector2 v = {p.x / p.z, p.y / p.z};
    return v;
}





//////////////////////
// Rotation functions
//////////////////////


void rotate_xz(Mesh* obj, Point* p, float angle)
{
    const float c = cosf(angle);
    const float s = sinf(angle);
    float originalX = p->x;
    float originalY = p->y;
    float originalZ = p->z;
    // p->x = obj->x + (((originalX - obj->x) * c) - ((originalZ - obj->z) * s));
    // p->z = obj->z + (((originalX - obj->x) * s) + ((originalZ - obj->z) * c));
    p->x = (((originalX) * c) - ((originalZ) * s));
    p->z = (((originalX) * s) + ((originalZ) * c));
}

void rotate_xy(Mesh* obj, Point* p, float angle)
{
    const float c = cosf(angle);
    const float s = sinf(angle);
    float originalX = p->x;
    float originalY = p->y;
    float originalZ = p->z;
    // p->x = obj->x + (((originalX - obj->x) * c) - ((originalZ - obj->z) * s));
    // p->z = obj->z + (((originalX - obj->x) * s) + ((originalZ - obj->z) * c));
    p->x = (((originalX) * c) - ((originalY) * s));
    p->y = (((originalX) * s) + ((originalY) * c));
}

void rotate_yz(Mesh* obj, Point* p, float angle)
{
    const float c = cosf(angle);
    const float s = sinf(angle);
    float originalX = p->x;
    float originalY = p->y;
    float originalZ = p->z;
    // p->x = obj->x + (((originalX - obj->x) * c) - ((originalZ - obj->z) * s));
    // p->z = obj->z + (((originalX - obj->x) * s) + ((originalZ - obj->z) * c));
    p->y = (((originalY) * c) - ((originalZ) * s));
    p->z = (((originalY) * s) + ((originalZ) * c));
}


void translateObjectX(Object* obj, float x)
{
    obj->transform.position.x += x;
}

void translateObjectY(Object* obj, float y)
{
    obj->transform.position.y += y;
}

void translateObjectZ(Object* obj, float z)
{
    obj->transform.position.z += z;
}



void MoveCameraForward(Camera* cam, float amount)
{
    cam->transform.position.x += amount;
}