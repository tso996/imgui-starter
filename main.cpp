//
//  main.cpp
//  OpenGLExperiments
//
//  Created by Joe on 18/06/2022.
//
#include <iostream>
#include <GL/glew.h>//this needs to be included before any other gl header
#include <GLFW/glfw3.h>




 static void CheckStatus( GLuint obj )
    {
        GLint status = GL_FALSE;
        if( glIsShader(obj) ) glGetShaderiv( obj, GL_COMPILE_STATUS, &status );
        if( glIsProgram(obj) ) glGetProgramiv( obj, GL_LINK_STATUS, &status );
        if( status == GL_TRUE ) return;
        GLchar log[ 1 << 15 ] = { 0 };
        if( glIsShader(obj) ) glGetShaderInfoLog( obj, sizeof(log), NULL, log );
        if( glIsProgram(obj) ) glGetProgramInfoLog( obj, sizeof(log), NULL, log );
        std::cerr << log << std::endl;
        std::exit( EXIT_FAILURE );
    }

    static void AttachShader( GLuint program, GLenum type, const char* src )
    {
        GLuint shader = glCreateShader( type );
        glShaderSource( shader, 1, &src, NULL );
        glCompileShader( shader );
        CheckStatus( shader );
        glAttachShader( program, shader );
        glDeleteShader( shader );
    }

    static GLuint Load( const char* shader, ... )
    {
        GLuint prog = glCreateProgram();
        va_list args;
        va_start( args, shader );
        while( shader )
        {
            const GLenum type = va_arg( args, GLenum );
            AttachShader( prog, type, shader );
            shader = va_arg( args, const char* );
        }
        va_end( args );
        glLinkProgram( prog );
        CheckStatus( prog );
        return prog;
    }






int main(int argc, const char * argv[]) {
    
    #ifdef DEBUG
        std::cout<<"debug build"<<std::endl;
    #else
        std::cout<<"release build"<<std::endl;
    #endif
    
    if (!glfwInit())
    {
        std::cout<<"glfw did not initialise properly.."<<std::endl;
        return -1;
    }else{
        std::cout<<"glfw initialised correctly.."<<std::endl;
    }
    
    //The following is required to change the opengl version from 2.1 to 4.1 in macs..
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //===============================================================


    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    if(glewInit()!=GLEW_OK){//this needs to happen after the context is made
        std::cout<<"glew did not initialize properly.."<<std::endl;
        return -1;
    }else{
        std::cout<<"glew initialised correctly.."<<std::endl;
    }
    
    std::cout<<"gl version: "<<glGetString(GL_VERSION)<<std::endl;
    
    GLuint vao = 0;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );


    float positions[ 6 ] =
    {
        -0.5f, -0.5f,
        0.0f,  0.5f,
        0.5f, -0.5f,
    };
    
    unsigned int buffer;//variable to store the id of the buffer
    glGenBuffers(1/*could be more than one*/, &buffer);//creates a buffer and returns the id of the buffer..
    glBindBuffer(GL_ARRAY_BUFFER,buffer);//selecting or binding the target buffer
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), positions, GL_STATIC_DRAW);
    //At this point the gpu just knows we have provided some data, it does not know the type of data, number of bytes of data, or that 2 elements from the positions array actually represent one point/vertex of the triangle..
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);//tell the gpu details about the bytes it has in the buffer with id
    std::cout<<"buffer id: "<<buffer<<std::endl;
    glEnableVertexAttribArray(0);//enable the vertex

    std::string vertexShader =
        "#version 410 core\n"
        "layout(location = 0) in vec4 position;\n"
        "void main(){\n"
        "   gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 410 core\n"
        "layout(location = 0) out vec4 color;\n"
        "void main(){\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";
    unsigned int shader = Load( vertexShader.c_str(), GL_VERTEX_SHADER, fragmentShader.c_str(), GL_FRAGMENT_SHADER, NULL );
    glUseProgram( shader );
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //leagcy OpenGL way
//        glBegin(GL_TRIANGLES);
//
//        glVertex2f(-0.5f, -0.5f);
//        glVertex2f(0.0f,0.5f);
//        glVertex2f(0.5f, -0.5f);
//
//        glEnd();
        
        glDrawArrays(GL_TRIANGLES, 0, 3);//drawcall. 0 is the starting index of array, 3 is number of vertices to render..
        
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

//#if defined DEBUG
//    std::cout<<"debug build"<<std::endl;
//#else
//    std::cout<<"release build"<<std::endl;
//    std::filesystem::current_path(Game::exeParentPath);//if it is release build, then we change the current working directory to the parent path of where the executable file is located
//    std::cout<<"the current working directory has been changed to the executable file path.."<<std::endl;
//#endif
