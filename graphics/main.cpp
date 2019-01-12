//Mayank Mali

#include <GL/glew.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <stdio.h>
#include <assert.h>
#include <chrono>

//our vertex shader

//c++11 raw string literal
const GLchar* vertexSource = R"glsl(
    #version 150 core
    in vec2 position;
    in vec3 color;

    out vec3 Color;

    void main(){
        Color = color;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";

//NOTE: we leave out optional geometry shader for this one

//our fragment shader
const GLchar* fragmentSource = R"glsl(
    #version 150 core

    in vec3 Color;

    out vec4 outColor;

    void main(){
        outColor = vec4(Color, 1.0);
    }
)glsl";


//our shape
const float vertices[] = {//vertex followed by (x, y, r, g, b) 
    -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
    0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
    -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
};

//indices of vertex, in an order, allows us to copy each vertex once
const GLuint elements[] = {
    0, 1, 2,
    2, 3, 0
};

//taken from https://www.khronos.org/opengl/wiki/OpenGL_Error#Catching_errors_.28the_easy_way.29 
//error message callbacks

void GLAPIENTRY
message_callback(    GLenum source,
                    GLenum type,
                    GLuint id,
                    GLenum severity,
                    GLsizei length,
                    const GLchar* message,
                    const void* userParam )
{
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

void init(SDL_Window** window, SDL_GLContext* context){
    assert(window != NULL);
    assert(context != NULL);

    //necessary to use modern opengl method for checking function availability
    glewExperimental = GL_TRUE;

    //SDL init
    SDL_Init(SDL_INIT_EVERYTHING);
    
    //tell SDL we want a forward-compatible opengl 3.2 context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);//for stencil buffer

    //set sdl window
    *window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);

    //set sdl context
    *context = SDL_GL_CreateContext(*window);
    
    //init glew after window creation after all function entry points created
    glewInit();
    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(message_callback, 0);
}

void compile_shader(GLint shader, const char* name){
    assert(name != NULL);

    //compile shader (graphics card compiles shader)
    glCompileShader(shader);

    //check to see if shader compiled correctly
    
    //to get a parameter from shader object:
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if(status != GL_TRUE){
        fprintf(stderr, "%s shader (%i) compilation failed!\n", name, shader);
    }else{
        printf("%s shader (%i) compiled successfully\n", name, shader);
    }
    
    char buffer[512];// 511 chars + nul terminator of log

    //getting shader log info
    glGetShaderInfoLog(shader, 512, NULL, buffer);

    printf("%s shader (%i) log info:\n%s\n", name, shader, buffer);
    
    //if compilation failed for any shader, exit
    if(status != GL_TRUE){
        fprintf(stderr, "exiting...\n");
        exit(-1);
    }
}

int main(int argc, char* argv[]){
    
    SDL_Window* window;
    SDL_GLContext context;

    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    GLuint vertexShader;
    GLuint fragmentShader;

    GLuint shaderProgram;
    
    GLint posAttrib;
    GLint colAttrib;

    init(&window, &context);
   
    //generate vertex array
    glGenVertexArrays(1, &vao);
   
    //generate element and vertex buffes
    glGenBuffers(1, &ebo);
    glGenBuffers(1, &vbo);

    //bind objects (simple since only one of each)
    glBindVertexArray(vao);
    
    //bind element and vertex buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);//make the object active, before uploading data to it.
    //this makes vbo the active array buffer, need to switch to others before using them

    //load data to active element array buffer and active vertex buffer NOTICE THAT ebo AND vbo NOT ARGUMENTS
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//load data onto active buffer

    //create shader objects
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
      
    //load shader data 
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL); 
    //number of sources, source list, array of lengths  
   
    //compile shaders
    compile_shader(vertexShader, "vertex");
    compile_shader(fragmentShader, "fragment");

    //combine shaders into a program

    //create an empty program
    shaderProgram = glCreateProgram();

    //attach shaders to program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    //need to explicitly state which output of fragment is to which buffer
    //since fragment can hav multiple outputs
    glBindFragDataLocation(shaderProgram, 0, "outColor");//this is name of variable

    //link the program
    glLinkProgram(shaderProgram);

    //to actually use shaders in program
    glUseProgram(shaderProgram);//if of many? only one program can be active at a time

    //now, need to specify our data format in buffer
    
    //get location of input position of fragment shader from program
    posAttrib = glGetAttribLocation(shaderProgram, "position");
    //now, give format of our buffer
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);//remember, this is in BYTES
    glEnableVertexAttribArray(posAttrib);

    colAttrib = glGetAttribLocation(shaderProgram, "color");
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(colAttrib);

    //create sdl event to handle events
    SDL_Event windowEvent;

    //main loop
    while(true){

        if(SDL_PollEvent(&windowEvent)){//get next event, if it exists
        
            if(windowEvent.type == SDL_QUIT){//if user exists window

                break;

            }
        }

        //draw the triangle (uses the currently bound VAO) with elements
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //primitive type, vertex offset, and number of vertices
        
        SDL_GL_SwapWindow(window);//swaps front and back buffers
    }

    //free graphics resources
    glDeleteProgram(shaderProgram);

    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);

    SDL_GL_DeleteContext(context);//free the context resources
    SDL_Quit();

    return 0;
}
