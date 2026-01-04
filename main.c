#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "SDL3/SDL.h"

#include "structures.h"
#include "render.h"
#include "penguin.h"
#include "cube.h"



Object* GlobalObjects = NULL;
int GlobalObjectCount = 0;
int GlobalObjectCapacity = 0;



void AddGlobalObject(Object obj)
{
    if (GlobalObjectCount >= GlobalObjectCapacity)
    {
        // Increase capacity
        GlobalObjectCapacity = (GlobalObjectCapacity == 0) ? 8 : GlobalObjectCapacity * 2;
        GlobalObjects = realloc(GlobalObjects, sizeof(Object) * GlobalObjectCapacity);
    }
    
    GlobalObjects[GlobalObjectCount++] = obj;
}







int main(int argc, char *argv[])
{
    // SDL initialization
    if (!SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS))
    {
        printf("Error init");
        return 0;
    }
    printf("SDL Init Success\n");

    // Window information, width, height, and FPS
    WindowInfo program = {800, 800, 100};
    
    // SDL Window and Renderer creation
    SDL_Window* window = SDL_CreateWindow("SDRenderer", program.width, program.height, SDL_WINDOW_RESIZABLE);
    printf("SDL window Success\n");
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    printf("SDL renderer Success\n");

    SDL_SetWindowRelativeMouseMode(window, true);


    



    // Creating an object
    // -----------------------------------------------------------------------
    printf("Creating object\n");
    int vertexCount = sizeof(verts) / sizeof(verts[0]);
    int faceCount = sizeof(faces) / sizeof(faces[0]);

    Object obj1 = CreateObject("Penger");

    Color red = {240, 10, 10, 255};

    obj1.mesh = CreateMesh(verts, vertexCount, faces, faceCount, red);
    printf("Objects name is: %s\n", obj1.name);

    AddGlobalObject(obj1);

    // Creating Second Object
    // -----------------------------------------------------------------------
    vertexCount = sizeof(Cverts) / sizeof(Cverts[0]);
    faceCount = sizeof(Cfaces) / sizeof(Cfaces[0]);

    Object obj2 = CreateObject("Cube");

    Color green = {10, 245, 10, 255};

    obj2.mesh = CreateMesh(Cverts, vertexCount, Cfaces, faceCount, green);
    obj2.transform.position.x = 1.5f;
    printf("Objects2 name is: %s\n", obj2.name);

    AddGlobalObject(obj2);
    // -----------------------------------------------------------------------
    
    // Creating camera
    // -----------------------------------------------------------------------
    Camera cam;
    cam.transform.position = (Vector3){0, 0, 0};
    cam.rotation = (Quaternion){0, 0, 0, 1};
    // -----------------------------------------------------------------------


    vertexCount = sizeof(verts) / sizeof(verts[0]);
    faceCount = sizeof(faces) / sizeof(faces[0]);





    // Delta time constant
    const float dt = 1.0/program.FPS;

    // Light direction
    Vector3 lightDirWorld = Vector3Normalize((Vector3){0.4f, -1.0f, 0.5f});
    

    // Variables for animation
    float dz = 1;
    float angle = 0;

    // Values for mouse movement
    float dx, dy;

    // Values for camera speed
    float speed = 0.02f;
    int forward = 0, right = 0, up = 0;

    // Values for keyboard input:
    bool key_w = false;
    bool key_s = false;
    bool key_a = false;
    bool key_d = false;
    bool key_space = false;
    bool key_ctrl = false;


    // Main Loop
    printf("Starting main loop\n");
    int quit = 0;
    int mouseGrabbed = 1;
    SDL_Event event;
    while (quit == 0)
    {
        while (SDL_PollEvent(&event))
        {
            // If the window gets closed
            if (event.type == SDL_EVENT_QUIT)
                quit = 1;
            
            // If the window gets resized, update the program struct
            if (event.type == SDL_EVENT_WINDOW_RESIZED)
            {
                SDL_GetWindowSizeInPixels(window, &program.width, &program.height);
            }

            // If the button gets pressed (specifically the escape key)
            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.scancode == SDL_SCANCODE_ESCAPE)
                {
                    printf("Escape pressed\n");
                    mouseGrabbed = false;
                    SDL_SetWindowRelativeMouseMode(window, mouseGrabbed);
                }
            }


            // If the mouse get clicked 
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                // Capture the mouse if it isn't already
                if (!SDL_GetWindowRelativeMouseMode(window))
                {
                    SDL_SetWindowRelativeMouseMode(window, true);
                    mouseGrabbed = true;
                }
            }

            // If the mouse moves and the mouse is grabbed, move the camera
            if (event.type == SDL_EVENT_MOUSE_MOTION)
            {
                if (mouseGrabbed == true)
                {
                    SDL_GetRelativeMouseState(&dx, &dy);
                    Camera_MouseLook(&cam, dx, dy, 0.002f);
                }
            }
        }

        // Get keyboard state and determine input
        const bool* state = SDL_GetKeyboardState(NULL);
        forward = state[SDL_SCANCODE_W] - state[SDL_SCANCODE_S];
        right = state[SDL_SCANCODE_D] - state[SDL_SCANCODE_A];
        // up = state[SDL_SCANCODE_SPACE] - state[SDL_SCANCODE_LCTRL];
        up = state[SDL_SCANCODE_SPACE];

        // Change speed if left shift or left control are held
        if (state[SDL_SCANCODE_LSHIFT] == true)
            speed = 0.06f;
        else if (state[SDL_SCANCODE_LCTRL] == true)
            speed = 0.005f;
        else
            speed = 0.03f;

        // Move camera
        Camera_Move(&cam, forward, right, up, speed);

        // Setting animation variables
        angle = 3.14159265 / 128;


        /////////////////////////
        /// Rendering section ///
        /////////////////////////


        Vector3 lightDirCamera = RotateVecByQuat(lightDirWorld, QuaternionInverse(cam.rotation));

        // Set Background
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderClear(renderer);

        // rotating/translating the object
        for (int i = 0; i < obj1.mesh->vertexCount; i++)
        {
            // Uncomment these to rotate the object around an axis
            // rotate_xz(obj1.mesh, &obj1.mesh->vertices[i], -angle);
            // rotate_xy(obj1.mesh, &obj1.mesh->vertices[i], -angle);
            // rotate_yz(obj1.mesh, &obj1.mesh->vertices[i], angle);

            // Uncomment to Translate Object in the z direction
            // translateObjectZ(&obj1, 0.00002);


            // Uncomment if you want to render the individual points
            // RenderPoint(renderer, Screen(Project(obj1, obj1->vertices[i])));
        }

        // Render objects
        // Render(renderer, program, &cam, &obj1);
        Render(renderer, program, &cam, &obj2);


        // printf("Starting triangle rendering\n");

        AddRenderTriangles(GlobalObjects, GlobalObjectCount, &cam, lightDirCamera);
        // printf("Rendering triangles now\n");
        RenderTriangles(renderer, program);




        ////// 
        ////// Triangle Making of obj1
        //////


        // RenderTriangle* tris = malloc(sizeof(RenderTriangle) * (obj1.mesh->facesCount + obj2.mesh->facesCount));
        // int triCount = 0;
        // // Geometry rendering
        // for (int i = 0; i < obj1.mesh->facesCount; ++i)
        // {
        //     int* row = obj1.mesh->faces[i];

        //     Vector3 camVerts[3];

        //     for (int k = 0; k < 3; ++k)
        //     {
        //         Vector3 world = {
        //             obj1.transform.position.x + obj1.mesh->vertices[row[k]].x,
        //             obj1.transform.position.y + obj1.mesh->vertices[row[k]].y,
        //             obj1.transform.position.z + obj1.mesh->vertices[row[k]].z
        //         };

        //         camVerts[k] = CameraSpace(&cam, world);
        //     }


        //     if (camVerts[0].z <= 0 || camVerts[1].z <= 0 || camVerts[2].z <= 0)
        //         continue;
            
            
        //     Vector3 ab = Vector3Sub(camVerts[1], camVerts[0]);
        //     Vector3 ac = Vector3Sub(camVerts[2], camVerts[0]);
        //     Vector3 normal = Vector3Normalize(Vector3Cross(ab, ac));

        //     // Back-face culling
        //     if (normal.z >= 0) continue;

        //     // Compute brightness (directional light)
        //     float brightness = Vector3Dot(normal, Vector3Scale(lightDirCamera, -1.0f));
        //     if (brightness < 0) brightness = 0;
        //     brightness = 0.3f + 0.9f * brightness;
        //     if (brightness > 1) brightness = 1;


            
        //     float depth = (camVerts[0].z + camVerts[1].z + camVerts[2].z) / 3.0f;

        //     tris[triCount++] = (RenderTriangle){
        //         { camVerts[0], camVerts[1], camVerts[2] },
        //         depth,
        //         ColorScale(obj1.mesh->color, brightness)
        //     };

        // }


        // ////// 
        // ////// Triangle making of obj2
        // //////

        // // Geometry rendering
        // for (int i = 0; i < obj2.mesh->facesCount; ++i)
        // {
        //     int* row = obj2.mesh->faces[i];

        //     Vector3 camVerts[3];

        //     for (int k = 0; k < 3; ++k)
        //     {
        //         Vector3 world = {
        //             obj2.transform.position.x + obj2.mesh->vertices[row[k]].x,
        //             obj2.transform.position.y + obj2.mesh->vertices[row[k]].y,
        //             obj2.transform.position.z + obj2.mesh->vertices[row[k]].z
        //         };

        //         camVerts[k] = CameraSpace(&cam, world);
        //     }


        //     if (camVerts[0].z <= 0 || camVerts[1].z <= 0 || camVerts[2].z <= 0)
        //         continue;


        //     Vector3 ab = Vector3Sub(camVerts[1], camVerts[0]);
        //     Vector3 ac = Vector3Sub(camVerts[2], camVerts[0]);
        //     Vector3 normal = Vector3Normalize(Vector3Cross(ab, ac));

        //     // Back-face culling
        //     if (normal.z >= 0) continue;

        //     // Compute brightness (directional light)
        //     float brightness = Vector3Dot(normal, Vector3Scale(lightDirCamera, -1.0f));
        //     if (brightness < 0) brightness = 0;
        //     brightness = 0.3f + 0.9f * brightness;
        //     if (brightness > 1) brightness = 1;


            
        //     float depth = (camVerts[0].z + camVerts[1].z + camVerts[2].z) / 3.0f;

        //     tris[triCount++] = (RenderTriangle){
        //         { camVerts[0], camVerts[1], camVerts[2] },
        //         depth,
        //         ColorScale(obj2.mesh->color, brightness)
        //     };

        //     // SDL_RenderGeometry(renderer, NULL, triangle, 3, NULL, 0);
        // }

        // qsort(tris, triCount, sizeof(RenderTriangle), CompareTris);


        ////// 
        ////// Rendering geometry
        //////

        // for (int i = 0; i < triCount; ++i)
        // {
        //     RenderTriangle* t = &tris[i];

        //     SDL_Vertex triangle[3];

        //     for (int k = 0; k < 3; ++k)
        //     {
        //         Vector2 projected = {
        //             t->v[k].x / t->v[k].z,
        //             t->v[k].y / t->v[k].z
        //         };

        //         ScreenPoint sp = Screen(projected, program);

        //         triangle[k].position.x = sp.x;
        //         triangle[k].position.y = sp.y;
        //         triangle[k].color.a = (float)t->color.a / (float)255;
        //         triangle[k].color.b = (float)t->color.b / (float)255;
        //         triangle[k].color.g = (float)t->color.g / (float)255;
        //         triangle[k].color.r = (float)t->color.r / (float)255;
        //         triangle[k].tex_coord.x = 0;
        //         triangle[k].tex_coord.y = 0;
        //     }

        //     SDL_RenderGeometry(renderer, NULL, triangle, 3, NULL, 0);
        // }

        ////// 
        ////// Rendering geometry
        //////












        


        // Needed to keep image
        SDL_RenderPresent(renderer);

        SDL_Delay(1000/program.FPS);
    }


    // Exiting functions
    printf("Quitting SDL\n");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();


    return 0;
}