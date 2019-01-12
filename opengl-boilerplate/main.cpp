//Mayank Mali

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <stdio.h>

int main(int argc, char* argv[]){
    
    //necessary to use modern opengl method for checking function availability
    glewExperimental = GL_TRUE;

    //SDL init
    SDL_Init(SDL_INIT_EVERYTHING);
    
    //tell SDL we want a forward-compatible opengl 3.2 context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);//for stencil buffer

    //create sdl window
    SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
    //last parameter can also be
    //SDL_WINDOW_RESIZEABLE
    //SDL_WINDOW_FULLSCREEN
    

    //create sdl context
    SDL_GLContext context = SDL_GL_CreateContext(window);
    
    //init glew after window creation after all function entry points created
    glewInit();

    GLuint vertexBuffer;
    (void)vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    printf("%u\n", vertexBuffer);

    //create sdl event to handle events
    SDL_Event windowEvent;

    //main loop
    while(true){

        if(SDL_PollEvent(&windowEvent)){//get next event, if it exists
        
            if(windowEvent.type == SDL_QUIT){//if user exists window

                break;

            }
        }

        SDL_GL_SwapWindow(window);//swaps front and back buffers
    }

    SDL_GL_DeleteContext(context);//free the context resources
    SDL_Quit();

    return 0;
}
