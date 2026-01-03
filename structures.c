#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "structures.h"

Object CreateObject(char* name)
{
    Transform objTransform = {{0.0, 0.0, 1.0}};

    Object obj;
    obj.transform = objTransform;
    obj.name = name;
    obj.mesh = NULL;

    return obj;
}

Mesh* CreateMesh(Point* verts, int vertexCount, int (*faces)[3], int faceCount)
{
    // Get vertex and face counts (Will have to make a solution to this)
    // int vertexCount = sizeof(verts) / sizeof(verts[0]);
    // int faceCount = sizeof(faces) / sizeof(faces[0]);

    // Make the Mesh and allocate the size
    Mesh* objMesh = malloc(sizeof(Mesh) + vertexCount * sizeof(Point));
    if (!objMesh) return NULL;

    // Set count variables
    objMesh->vertexCount = vertexCount;
    objMesh->facesCount = faceCount;

    // memcpy the vertices and faces vectors
    objMesh->faces = malloc(faceCount * sizeof(*objMesh->faces));
    memcpy(objMesh->faces, faces, faceCount * sizeof(*objMesh->faces));
    memcpy(objMesh->vertices, verts, vertexCount * sizeof(Point));

    return objMesh;
}







//
// Functions for Quaternions
//

Quaternion QuatNormalize(Quaternion q)
{
    float len = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
    return (Quaternion){ q.x/len, q.y/len, q.z/len, q.w/len };
}


Quaternion QuaternionFromAxisAngle(float ax, float ay, float az, float angle)
{
    float half = angle * 0.5f;
    float s = sinf(half);

    return QuatNormalize((Quaternion){
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


Vector3 RotateVecByQuat(Vector3 v, Quaternion q)
{
    // v' = q * v * q^-1
    Quaternion vq = {v.x, v.y, v.z, 0};
    Quaternion qi = {-q.x, -q.y, -q.z, q.w};

    Quaternion r = QuaternionMultiply(QuaternionMultiply(q, vq), qi);
    return (Vector3){ r.x, r.y, r.z };
}


float GetPitchFromQuat(Quaternion q)
{
    // Standard formula: pitch = arcsin(2*(w*y - z*x))
    float sinp = 2.0f * (q.x + q.y * q.z * q.w);
    float cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
    return atan2f(sinp, cosp);
}





Vector3 GetCameraForward(Camera* cam)
{
    // Forward in camera space is +Z or -Z depending on convention; here we use -Z
    return RotateVecByQuat((Vector3){0, 0, 1}, cam->rotation);
}

Vector3 GetCameraRight(Camera* cam)
{
    return RotateVecByQuat((Vector3){1, 0, 0}, cam->rotation);
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

    cam->rotation = QuatNormalize(cam->rotation);
}