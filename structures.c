#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "structures.h"

Object CreateObject(char* name)
{
    Transform objTransform = {0};
    objTransform.position.x = 0;
    objTransform.position.y = 0;
    objTransform.position.z = 1;

    objTransform.rotation.x = 0;
    objTransform.rotation.y = 0;
    objTransform.rotation.z = 0;
    objTransform.rotation.w = 1;

    Object obj;
    obj.transform = objTransform;
    obj.name = name;
    obj.mesh = NULL;

    return obj;
}

Mesh* CreateMesh(Vector3* verts, int vertexCount, int (*faces)[3], int faceCount, Color color)
{
    // Get vertex and face counts (Will have to make a solution to this)
    // int vertexCount = sizeof(verts) / sizeof(verts[0]);
    // int faceCount = sizeof(faces) / sizeof(faces[0]);

    // Make the Mesh and allocate the size
    Mesh* objMesh = malloc(sizeof(Mesh) + vertexCount * sizeof(Vector3));
    if (!objMesh) return NULL;

    // Set count variables
    objMesh->vertexCount = vertexCount;
    objMesh->facesCount = faceCount;
    objMesh->color = color;

    // memcpy the vertices and faces vectors
    objMesh->faces = malloc(faceCount * sizeof(*objMesh->faces));
    memcpy(objMesh->faces, faces, faceCount * sizeof(*objMesh->faces));
    memcpy(objMesh->vertices, verts, vertexCount * sizeof(Vector3));

    return objMesh;
}







//
// Functions for Quaternions
//

Quaternion QuaternionNormalize(Quaternion q)
{
    float len = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
    
    if (len == 0)
        return (Quaternion){0, 0, 0, 1};
    
    float inv = 1.0f / len;

    return (Quaternion){ q.x/len, q.y/len, q.z/len, q.w/len };
}


Quaternion QuaternionFromAxisAngle(float ax, float ay, float az, float angle)
{
    float half = angle * 0.5f;
    float s = sinf(half);

    return QuaternionNormalize((Quaternion){
        ax * s,
        ay * s,
        az * s,
        cosf(half)
    });
}


Quaternion QuaternionMultiply(Quaternion a, Quaternion b)
{
    return (Quaternion){
        a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y,
        a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x,
        a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w,
        a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z
    };
}


// Vector3 RotateVectorByQuaternion(Vector3 v, Quaternion q)
// {
//     // v' = q * v * q^-1
//     Quaternion vq = {v.x, v.y, v.z, 0};
//     Quaternion qi = {-q.x, -q.y, -q.z, q.w};

//     Quaternion r = QuaternionMultiply(QuaternionMultiply(q, vq), qi);
//     return (Vector3){ r.x, r.y, r.z };
// }

Vector3 RotateVectorByQuaternion(Vector3 v, Quaternion q)
{
    // Extract the vector part of the quaternion
    Vector3 u = {q.x, q.y, q.z};
    float s = q.w;

    // Standard formula: v + 2.0 * cross(u, cross(u, v) + s * v)
    
    // 1. dot(u, v)
    float dot_uv = u.x * v.x + u.y * v.y + u.z * v.z;
    // 2. dot(u, u)
    float dot_uu = u.x * u.x + u.y * u.y + u.z * u.z;
    
    // 3. Scale v by (s*s - dot_uu)
    float w_sq_minus_dot_uu = (s * s) - dot_uu;
    
    Vector3 result;
    result.x = (w_sq_minus_dot_uu * v.x) + (2.0f * dot_uv * u.x) + (2.0f * s * (u.y * v.z - u.z * v.y));
    result.y = (w_sq_minus_dot_uu * v.y) + (2.0f * dot_uv * u.y) + (2.0f * s * (u.z * v.x - u.x * v.z));
    result.z = (w_sq_minus_dot_uu * v.z) + (2.0f * dot_uv * u.z) + (2.0f * s * (u.x * v.y - u.y * v.x));
    
    return result;
}


float GetPitchFromQuaternion(Quaternion q)
{
    // Standard formula: pitch = arcsin(2*(w*y - z*x))
    float sinp = 2.0f * (q.x + q.y * q.z * q.w);
    float cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
    return atan2f(sinp, cosp);
}


Quaternion QuaternionInverse(Quaternion q)
{
    float lenSq = q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z;

    if (lenSq == 0.0f)
        return (Quaternion){0, 0, 0, 1}; // identity fallback

    return (Quaternion){
       -q.x / lenSq,
       -q.y / lenSq,
       -q.z / lenSq,
       q.w / lenSq
    };
}



Vector3 Vector3Sub(Vector3 a, Vector3 b)
{
    Vector3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}


Vector3 Vector3Normalize(Vector3 v)
{
    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length == 0.0f) return (Vector3){0, 0, 0}; // avoid division by zero
    Vector3 result;
    result.x = v.x / length;
    result.y = v.y / length;
    result.z = v.z / length;
    return result;
}

Vector3 Vector3Cross(Vector3 a, Vector3 b)
{
    Vector3 result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

float Vector3Dot(Vector3 a, Vector3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Vector3Scale(Vector3 v, float s)
{
    return (Vector3){ v.x * s, v.y * s, v.z * s };
}

Vector3 Vector3Lerp(Vector3 start, Vector3 end, float t) {
    return (Vector3){
        start.x + (end.x - start.x) * t,
        start.y + (end.y - start.y) * t,
        start.z + (end.z - start.z) * t
    };
}






Vector3 GetCameraForward(Camera* cam)
{
    // Forward in camera space is +Z or -Z depending on convention; here we use -Z
    return RotateVectorByQuaternion((Vector3){0, 0, 1}, cam->rotation);
}

Vector3 GetCameraRight(Camera* cam)
{
    return RotateVectorByQuaternion((Vector3){1, 0, 0}, cam->rotation);
}






void Camera_Move(Camera* cam, int forward, int right, int up, float speed)
{
    Vector3 fwd = GetCameraForward(cam);
    Vector3 rgt = GetCameraRight(cam);
    Vector3 worldUp = {0, 1, 0};

    // Forward/backward
    cam->transform.position.x += fwd.x * speed * forward;
    cam->transform.position.y += fwd.y * speed * forward;
    cam->transform.position.z += fwd.z * speed * forward;

    // Left/right
    cam->transform.position.x += rgt.x * speed * right;
    cam->transform.position.y += rgt.y * speed * right;
    cam->transform.position.z += rgt.z * speed * right;

    // Up/down (optional, e.g., flying)
    cam->transform.position.x += worldUp.x * speed * up;
    cam->transform.position.y += worldUp.y * speed * up;
    cam->transform.position.z += worldUp.z * speed * up;
}









//
// Main function for rotating the camera
//
void Camera_MouseLook(Camera* cam, float dx, float dy, float sensitivity)
{
    Quaternion q = cam->rotation;

    float yaw = atan2f(2.0f * (q.w * q.y + q.x * q.z),
                       1.0f - 2.0f * (q.y * q.y + q.x * q.x));
    

    float pitch = asinf(2.0f * (q.w * q.x - q.z * q.y));

    yaw   += dx * sensitivity;  // mouse left/right
    pitch += dy * sensitivity;  // mouse up/down

    const float pitch_limit = 1.55f;
    if (pitch > pitch_limit) pitch = pitch_limit;
    if (pitch < -pitch_limit) pitch = -pitch_limit;

    Quaternion yawQ   = QuaternionFromAxisAngle(0.0f, 1.0f, 0.0f, yaw);    // world up
    Quaternion pitchQ = QuaternionFromAxisAngle(1.0f, 0.0f, 0.0f, pitch);  // local right

    // Order matters: yaw in world space, pitch in local space
    cam->rotation = QuaternionMultiply(yawQ, pitchQ);

    cam->rotation = QuaternionNormalize(cam->rotation);
}






void RotateObjectX(Object* obj, float angle)
{
    Vector3 axis = {1.0f, 0.0f, 0.0f};
    Quaternion rotStep = QuaternionFromAxisAngle(axis.x, axis.y, axis.z, angle);
    
    // Apply new rotation ON TOP of existing rotation
    obj->transform.rotation = QuaternionMultiply(obj->transform.rotation, rotStep);
    
    // Normalize occasionally to keep it clean
    obj->transform.rotation = QuaternionNormalize(obj->transform.rotation);
}


void RotateObjectY(Object* obj, float angle)
{
    Vector3 axis = {0.0f, 1.0f, 0.0f};
    Quaternion rotStep = QuaternionFromAxisAngle(axis.x, axis.y, axis.z, angle);
    
    obj->transform.rotation = QuaternionMultiply(obj->transform.rotation, rotStep);
    obj->transform.rotation = QuaternionNormalize(obj->transform.rotation);
}


void RotateObjectZ(Object* obj, float angle)
{
    Vector3 axis = {0.0f, 0.0f, 1.0f};
    Quaternion rotStep = QuaternionFromAxisAngle(axis.x, axis.y, axis.z, angle);
    
    obj->transform.rotation = QuaternionMultiply(obj->transform.rotation, rotStep);
    obj->transform.rotation = QuaternionNormalize(obj->transform.rotation);
}












///
/// Function compares triangles for depth testing
/// 
int CompareTris(const void* a, const void* b)
{
    const RenderTriangle* t1 = a;
    const RenderTriangle* t2 = b;

    if (t1->depth < t2->depth) return 1;
    if (t1->depth > t2->depth) return -1;

    return 0;
}


///
/// This function converts a vector into camera space for projection
///
Vector3 CameraSpace(const Camera* cam, Vector3 world)
{
    // Step 1: translate a vector so that the camera is a (0, 0, 0)
    Vector3 v = {
        world.x - cam->transform.position.x,
        world.y - cam->transform.position.y,
        world.z - cam->transform.position.z
    };

    // Step 2: rotate by inverse camera rotation so movement feels natural
    Quaternion invRot = QuaternionInverse(cam->rotation);
    v = RotateVectorByQuaternion(v, invRot);

    return v;
}


///
/// Scales a color by a certain brightness value
///
Color ColorScale(Color color, float brightness)
{
    // Clamp brightness to [0, 1] just in case
    if (brightness < 0.0f) brightness = 0.0f;
    if (brightness > 1.0f) brightness = 1.0f;

    Color result;
    result.r = (uint8_t)((float)color.r * brightness);
    result.g = (uint8_t)((float)color.g * brightness);
    result.b = (uint8_t)((float)color.b * brightness);
    result.a = color.a; // alpha stays the same

    return result;
}