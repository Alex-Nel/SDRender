#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdint.h>

////////////////////////////////////////////////////
/// Structs related to displaying on a 2D screen ///
////////////////////////////////////////////////////

typedef struct WindowInfo
{
    int width;
    int height;
    int FPS;
} WindowInfo;

typedef struct ScreenPoint
{
    float x;
    float y;
} ScreenPoint;

typedef struct Vector2
{
    float x;
    float y;
} Vector2;




/////////////////////////////////////
/// Structs related to 3D objects ///
/////////////////////////////////////


// A single point in 3D space
// Legacy, replaced with Vector3
typedef struct Point
{
    float x;
    float y;
    float z;
} Point;


// A RGBA representation of a color
typedef struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} Color;


// A 3D vector
typedef struct Vector3
{
    float x;
    float y;
    float z;
} Vector3;






typedef struct RenderTriangle
{
    Vector3 v[3];
    float depth;
    Color color;
} RenderTriangle;








// A transform of an object
typedef struct Transform
{
    Vector3 position;
} Transform;


// The mesh of an object that you would render
typedef struct Mesh
{
    int vertexCount;
    int facesCount;
    Color color;
    int (*faces)[3];
    Vector3 vertices[];
} Mesh;


// An object that encapsulates a transform, name, and mesh
typedef struct Object
{
    Transform transform;
    char* name;
    Mesh* mesh;
} Object;


// Used to represent rotation
typedef struct Quaternion
{
    float x;
    float y;
    float z;
    float w;
} Quaternion;


// A camera used to determine rendering
typedef struct Camera
{
    Transform transform;
    Quaternion rotation;
} Camera;




/////////////////////////////////////////
/// Functions that use the structures ///
/////////////////////////////////////////



// Creating an object
Object CreateObject(char* name);

// Creating a mesh
Mesh* CreateMesh(Vector3* verts, int vertexCount, int (*faces)[3], int faceCount, Color color);


// Quaternion operation
Quaternion QuatNormalize(Quaternion q);
Quaternion QuaternionFromAxisAngle(float ax, float ay, float az, float angle);
Quaternion QuaternionMultiply(Quaternion a, Quaternion b);

Vector3 RotateVecByQuat(Vector3 v, Quaternion q);
float GetPitchFromQuat(Quaternion q);

Quaternion QuaternionInverse(Quaternion q);

Vector3 Vector3Sub(Vector3 a, Vector3 b);
Vector3 Vector3Normalize(Vector3 v);
Vector3 Vector3Cross(Vector3 a, Vector3 b);
float Vector3Dot(Vector3 a, Vector3 b);
Vector3 Vector3Scale(Vector3 v, float s);
Vector3 Vector3Lerp(Vector3 start, Vector3 end, float t);


// Camera operations
Vector3 GetCameraForward(Camera* cam);
Vector3 GetCameraRight(Camera* cam);
void Camera_Move(Camera* cam, int forward, int right, int up, float speed);
void Camera_MouseLook(Camera* cam, float dx, float dy, float sensitivity);





int CompareTris(const void* a, const void* b);
Vector3 CameraSpace(const Camera* cam, Vector3 world);
Color ColorScale(Color color, float brightness);


#endif