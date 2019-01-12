#include <SDL.h>
#include <SDL_opengl.h>

int main(int argc, char* argv[]){


    SDL_Init(SDL_INIT_EVERYTHING);
    
    //tell SDL we want a forward-compatible opengl 3.2 context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);//for stencil buffer

    SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
    //last parameter can also be
    //SDL_WINDOW_RESIZEABLE
    //SDL_WINDOW_FULLSCREEN
    
    SDL_GLContext context = SDL_GL_CreateContext(window);


    SDL_Event windowEvent;

    while(true){

        if(SDL_PollEvent(&windowEvent)){
        
            if(windowEvent.type == SDL_QUIT){//if user presses X on window

                break;

            }
        
        }


        SDL_GL_SwapWindow(window);//swaps front and back buffers
    }


    SDL_GL_DeleteContext(context);//free the context resources
    SDL_Quit();

    return 0;
}
