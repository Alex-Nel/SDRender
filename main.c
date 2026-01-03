#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "SDL3/SDL.h"

#include "structures.h"
#include "render.h"
#include "penguin.h"
// #include "cube.h"



int main()
{
    // SDL initialization
    if (!SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS))
    {
        printf("Error init");
        return 0;
    }
    printf("SDL Init Success\n");

    // Window information, width, height, and FPS
    WindowInfo program = {800, 800, 60};
    
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

    obj1.mesh = CreateMesh(verts, vertexCount, faces, faceCount);
    printf("Objects name is: %s\n", obj1.name);
    // -----------------------------------------------------------------------
    
    Camera cam;
    cam.transform.position = (Vector3){0, 0, 0};
    cam.rotation = (Quaternion){0, 0, 0, 1};

    // Delta time constnat
    const float dt = 1.0/program.FPS;

    // Variables for animation
    float dz = 1;
    float angle = 0;

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
            if (event.type == SDL_EVENT_QUIT)
                quit = 1;
            
            if (event.type == SDL_EVENT_WINDOW_RESIZED)
            {
                SDL_GetWindowSizeInPixels(window, &program.width, &program.height);
            }

            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.scancode == SDL_SCANCODE_ESCAPE)
                {
                    printf("Escape pressed\n");
                    mouseGrabbed = false;
                    SDL_SetWindowRelativeMouseMode(window, mouseGrabbed);
                }
            }


            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (!SDL_GetWindowRelativeMouseMode(window))
                {
                    SDL_SetWindowRelativeMouseMode(window, true);
                    mouseGrabbed = true;
                }
            }

            if (event.type == SDL_EVENT_MOUSE_MOTION)
            {
                if (mouseGrabbed == true)
                {
                    SDL_GetRelativeMouseState(&dx, &dy);
                    Camera_MouseLook(&cam, dx, dy, 0.002f);
                }
            }
        }

        const bool* state = SDL_GetKeyboardState(NULL);
        forward = state[SDL_SCANCODE_W] - state[SDL_SCANCODE_S];
        right = state[SDL_SCANCODE_D] - state[SDL_SCANCODE_A];
        // up = state[SDL_SCANCODE_SPACE] - state[SDL_SCANCODE_LCTRL];
        up = state[SDL_SCANCODE_SPACE];

        if (state[SDL_SCANCODE_LSHIFT] == true)
            speed = 0.04f;
        else if (state[SDL_SCANCODE_LCTRL] == true)
            speed = 0.005f;
        else
            speed = 0.03f;

        Camera_Move(&cam, forward, right, up, speed);


        // Set Background
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderClear(renderer);


        // Setting animation variables
        angle = 3.14159265 / 128;

        // printf("Vertex Count: %f\n", obj1.mesh->vertexCount);
        // rotating/translating the object
        for (int i = 0; i < obj1.mesh->vertexCount; i++)
        {
            // Rotating object around an axis
            // rotate_xz(obj1.mesh, &obj1.mesh->vertices[i], -angle);
            // rotate_xy(obj1.mesh, &obj1.mesh->vertices[i], -angle);
            // rotate_yz(obj1.mesh, &obj1.mesh->vertices[i], angle);

            // Translate Object in the z direction
            // translateObjectZ(&obj1, 0.00002);


            // Uncomment if you want to render the individual points
            // RenderPoint(renderer, Screen(Project(obj1, obj1->vertices[i])));
        }

        // Rendering loop
        for (int i = 0; i < obj1.mesh->facesCount; ++i)
        {
            int* row = obj1.mesh->faces[i];

            // int length = sizeof(row) / sizeof(row[0]);
            for (int j = 0; j < 3; ++j)
            {
                int a = row[j];
                int b = row[(j+1)%3];
                DrawLine(renderer,
                    Screen(Project(&cam, &obj1.transform, obj1.mesh->vertices[a]), program),
                    Screen(Project(&cam, &obj1.transform, obj1.mesh->vertices[b]), program),
                    program
                );
            }
        }


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


/*
Compile command:
gcc main.c -o Program (-Llib -Iinclude) -lSDL3
*/