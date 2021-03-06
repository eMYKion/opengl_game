//Mayank Mali

#include <GL/glew.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <stdio.h>
#include <assert.h>


//taken from https://www.khronos.org/opengl/wiki/OpenGL_Error#Catching_errors_.28the_easy_way.29 
//error message callbacks

void GLAPIENTRY
MessageCallback(    GLenum source,
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

// During init, enable debug output
// glEnable              ( GL_DEBUG_OUTPUT );
// glDebugMessageCallback( MessageCallback, 0 );

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
    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    //vertex array object, stores all the links (format) between the attributes of a program and
    //the vbo with raw vertex data

    GLuint vao;
    glGenVertexArrays(1, &vao);

    //just call glBindVertexArray(GLuint vao); before every call to glVertexAttribPointer
    //and the format will be rememebered in that vao
    glBindVertexArray(vao);

    //needs to be bound before vbo are bound

    //our triangle
    float vertices[] = {//vertex followed by (x,y)
        0.0f,  0.5f, //vert 1
        0.5f, -0.5f, //vert 2
        -0.5f, -0.5f  //vert 3 
    };

    GLuint vbo;//vertex buffer object, refers to buffer on GPU memory, makes fast

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);//make the object active, before uploading data to it.
    //this makes vbo the active array buffer, need to switch to others before using them
    
    //interesting, sizeof returns total size of stack allocated array...
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//load data onto active buffer

    //our vertex shader

    //c++11 raw string literal
    const char* vertexSource = R"glsl(
        #version 150 core
        in vec2 position;

        void main(){
            gl_Position = vec4(position.x, position.y, 0.0, 1.0);
        }
    )glsl";


    //our fragment shader
    const char* fragmentSource = R"glsl(
        #version 150 core
        out vec4 outColor;

        void main(){
            outColor = vec4(1.0, 1.0, 1.0, 1.0);
        }
    )glsl";

    //NOTE: we leave out optional geometry shader for this one

    //create shader objects
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
      
    //load shader data 
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL); 
    //number of sources, source list, array of lengths  

    //compile shader (graphics card compiles shader)
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    //check to see if shaders compiled correctly
    
    //to get a parameter from shader object:
    GLint vstatus;
    GLint fstatus;

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vstatus);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fstatus);

    if(vstatus != GL_TRUE){
        fprintf(stderr, "vertex shader compilation failed!\n");
    }else{
        printf("vertex shader compiled successfully\n");
    }
    
    if(fstatus != GL_TRUE){
        fprintf(stderr, "fragment shader compilation failed!\n");
    }else{
        printf("fragment shader compiled successfully\n");
    }
    
    char vinfo_buff[512];// 511 chars + nul terminator of log
    char finfo_buff[512];// 511 chars + nul terminator of log

    //getting shader log info
    glGetShaderInfoLog(vertexShader, 512, NULL, vinfo_buff);
    glGetShaderInfoLog(fragmentShader, 512, NULL, finfo_buff);

    printf("vertex shader log info:\n%s\n", vinfo_buff);
    printf("fragment shader log info:\n%s\n", finfo_buff);
    
    //if compilation failed for any shader, exit
    if(!(vstatus == GL_TRUE && fstatus == GL_TRUE)){
        exit(-1);
    }

    //combine shaders into a program

    //create an empty program
    GLuint shaderProgram = glCreateProgram();
    //attach vertex shader to program
    glAttachShader(shaderProgram, vertexShader);
    //attach fragment shader to program
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
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");


    //now, give format of our buffer
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    //location of input data, 
    //num of vec components (1,2,3, or 4), 
    //data type, to be normalized, 
    //stride (bytes in between) (int), 
    //offset from start of buffer (pointer)

    //vertex attribute array needs to be enabled

    glEnableVertexAttribArray(posAttrib);

    //create sdl event to handle events
    SDL_Event windowEvent;

    //main loop
    while(true){

        if(SDL_PollEvent(&windowEvent)){//get next event, if it exists
        
            if(windowEvent.type == SDL_QUIT){//if user exists window

                break;

            }
        }

        //draw the triangle (uses the currently bound VAO)
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //primitive type, vertex offset, and number of vertices
        
        SDL_GL_SwapWindow(window);//swaps front and back buffers
    }

    SDL_GL_DeleteContext(context);//free the context resources
    SDL_Quit();

    return 0;
}
