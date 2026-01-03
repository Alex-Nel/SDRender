#ifndef STRUCTURES_H
#define STRUCTURES_H

// 
// Structs related to displaying on a 2D screen
//
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




//
// Structs related to 3D objects
//
typedef struct Point
{
    float x;
    float y;
    float z;
} Point;

// Unused Currently
typedef struct Vector3
{
    float x;
    float y;
    float z;
} Vector3;

typedef struct Transform
{
    Vector3 position;
} Transform;

typedef struct Mesh
{
    int vertexCount;
    int facesCount;
    int (*faces)[3];
    Point vertices[];
} Mesh;

typedef struct Object
{
    Transform transform;
    char* name;
    Mesh* mesh;
} Object;

typedef struct Quaternion
{
    float x;
    float y;
    float z;
    float w;
} Quaternion;

typedef struct Camera
{
    Transform transform;
    Quaternion rotation;
} Camera;







Object CreateObject(char* name);

Mesh* CreateMesh(Point* verts, int vertexCount, int (*faces)[3], int faceCount);


Quaternion QuatNormalize(Quaternion q);

Quaternion QuaternionFromAxisAngle(float ax, float ay, float az, float angle);

Quaternion QuaternionMultiply(Quaternion a, Quaternion b);

Vector3 RotateVecByQuat(Vector3 v, Quaternion q);

float GetPitchFromQuat(Quaternion q);

Vector3 GetCameraForward(Camera* cam);
Vector3 GetCameraRight(Camera* cam);

void Camera_Move(Camera* cam, int forward, int right, int up, float speed);

void Camera_MouseLook(Camera* cam, float dx, float dy, float sensitivity);


#endif