#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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


    // Creating an object
    // -----------------------------------------------------------------------
    printf("Creating object\n");
    int vertexCount = sizeof(verts) / sizeof(verts[0]);
    int faceCount = sizeof(faces) / sizeof(faces[0]);
    
    // Creating Transform
    Transform obj1Transform = {0.0, 0.0, 1.5};

    // Creating Mesh
    Mesh* obj1Mesh = malloc(sizeof(Mesh) + vertexCount * sizeof(Point));
    obj1Mesh->vertexCount = vertexCount;
    obj1Mesh->facesCount = faceCount;
    obj1Mesh->faces = malloc(faceCount * sizeof *obj1Mesh->faces);
    memcpy(obj1Mesh->faces, faces, faceCount * sizeof(*obj1Mesh->faces));
    memcpy(obj1Mesh->vertices, verts, vertexCount * sizeof(Point));

    // Creating Object using the transform and Mesh.
    Object obj1;
    obj1.transform = obj1Transform;
    obj1.mesh = obj1Mesh;
    // -----------------------------------------------------------------------
    

    // Delta time constnat
    const float dt = 1.0/program.FPS;

    // Variables for animation
    float dz = 1;
    float angle = 0;

    // Main Loop
    printf("Starting main loop\n");
    int quit = 0;
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
        }

        // Set Background
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderClear(renderer);


        // Setting animation variables
        angle = 3.14159265 / 128;

        // rotating/translating the object
        for (int i = 0; i < obj1.mesh->vertexCount; i++)
        {
            // Rotating object around an axis
            rotate_xz(obj1Mesh, &obj1Mesh->vertices[i], -angle);
            // rotate_xy(obj1Mesh, &obj1Mesh->vertices[i], -angle);
            // rotate_yz(obj1Mesh, &obj1Mesh->vertices[i], angle);

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
                DrawLine(renderer, Screen(Project(&obj1.transform, obj1.mesh->vertices[a]), program), Screen(Project(&obj1.transform, obj1.mesh->vertices[b]), program));
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